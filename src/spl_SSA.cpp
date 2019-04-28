#include "spl_SSA.hpp"
#include "spl_exception.hpp"
#include <set>
#include <fstream>
#include <queue>
#include <assert.h>

void SPL_SSA::OptimizeIR(std::vector<Instruction>& ins) {
    genCFGNode(ins);

    // generate CFG
    generateCFG();

    // compute idom
    computeTreeIdom();

    // compute DF
    generateDF();

    // insert Phi
    insertPhiFunction();

    // rename variable
     renameVariable();

    // output optimized IR
    // removeUnusedVariable();

    outputIdom();

    outputPhiInstruction();

    outputDUChain();

    // constantPropagation();
}

void SPL_SSA::constantPropagation() {

}


void SPL_SSA::removeUnusedVariable() {
    for(auto& variable: duChain) {
        if(variable.second.empty()) {
            // 没有使用过
            // 追踪到定义处
            auto pos = definition.find(variable.first)->second;

        }
    }
}


void SPL_SSA::generateDF() {
    for(auto& node: nodeSet){
        if(node->parentSet.size() >= 2) {
            for(auto& parent : node->parentSet) {
                int runner = node->parentSet[&parent - &node->parentSet[0]];
                while (runner != node->idom) {
                    nodeSet[runner]->DF.push_back(&node - &nodeSet[0]);
                    runner = nodeSet[runner]->idom;
                }
            }
        }
    }
}



void SPL_SSA::genCFGNode(std::vector<Instruction> &insSet) {
    SSANode* current = nullptr;
    for(Instruction& ins : insSet){
        if(ins.op == OP_ASSIGN && ins.res->cl == VAR){
            auto it = variableListBlock.find(ins.res->name);

            // 记录每一个变量出现的node列表
            if(it == variableListBlock.end()) {
                variableListBlock.insert({ins.res->name, {nodeSet.size() - 1}});
            } else{
                it->second.push_back(nodeSet.size() - 1);
            }

            current->instruSet.push_back(&ins);
        }
        // 开始新的node
        else if(!ins.label.empty()) {
            auto newNode = new SSANode();
            current = newNode;
            nodeSet.push_back(newNode);
            current->label = &ins.label;
            labelIndexMap.insert({*current->label, nodeSet.size() - 1});
        } else if (ins.op == OP_IF || ins.op == OP_IF_Z || ins.op == OP_GOTO) {
            // 添加子节点
            if ( current == nullptr) throw splException{0, 0, "no label at start of target."};
            current->LabelSet.push_back(&ins.res->name);
            current->instruSet.push_back(&ins);
        } else {current->instruSet.push_back(&ins);}
    }
}


void SPL_SSA::generateCFG() {
    for(int index = 0 ; index < nodeSet.size() ; index ++) {
        int currentNodeOffset = index;
        SSANode* node = nodeSet[index];
        for(auto label : node->LabelSet) {
            int offset = labelIndexMap.find(*label)->second; // 找到子女的label对应的offset
//            node->childSet.push_back(offset); // child set
            nodeSet[offset]->parentSet.push_back(currentNodeOffset); // parent set
        }
    }
}


// 插入phi 函数
void SPL_SSA::insertPhi(int nodeIndex, const string& variableName) {
    auto it = nodeSet[nodeIndex]->instruSet.begin();
    nodeSet[nodeIndex]->instruSet.insert(it
            , new PhiInstruction{new Operand(UNKNOWN, variableName, VAR)});

    // 更新有定义phi函数的node的index
    phiBlock.insert(nodeIndex);
}

void SPL_SSA::insertPhiFunction() {

    for(auto &pair : variableListBlock) {
        vector<bool> PhiInserted(nodeSet.size(), false);
        vector<bool> added(nodeSet.size(), false);
        for(auto& nodeIndex : pair.second) {
            added[nodeIndex] = true;
        }
        while (!pair.second.empty()) {
            int b = pair.second.back();
            pair.second.pop_back();
            // get DF(b)
            auto DFb = nodeSet[b]->DF;
            for(auto& nodeIndex : DFb) {
                if(!PhiInserted[nodeIndex]) {
                    // add phiFunction
                    insertPhi(nodeIndex, pair.first);

                    PhiInserted[nodeIndex] = true;

                    if(!added[nodeIndex]) {

                        added[nodeIndex] = true;
                        pair.second.push_back(nodeIndex);

                    }
                }
            }
        }
    }
}


void addVersionToVariable(std::string& variable, int version) {
    variable = variable + "." + std::to_string(version);
}


void SPL_SSA::updateUsage(std::vector<map<std::string, int>>& def,
        string& variableName,
        int& nodeIndex,
        Instruction* ins) {

    int index = nodeIndex;
    do {
        auto it = def[index].find(variableName);
        if(it != def[index].end()) {
            addVersionToVariable(variableName, it->second);
            // 添加u-d链指针
            auto it = duChain.find(variableName);
            it->second.push_back(ins);
            return;
        } else {
            // 向上寻找
            index = nodeSet[index]->idom;
        }
    } while (index > 0);
}

void SPL_SSA::updateDefinition(map<std::string, int>& currentDef,
        std::vector<map<std::string, int>>& closestDef,
        string& variableName,
        int& nodeIndex,
        Instruction* ins) {

    auto it = currentDef.find(variableName); // 更新定义
    auto it1 = closestDef[nodeIndex].find(variableName);

    if(it1 == closestDef[nodeIndex].end()) {
        closestDef[nodeIndex].insert({variableName, it->second});
    } else {
        it1->second = it->second;
    }
    addVersionToVariable(variableName, it->second ++ );

    // 添加 d - u 链入口
    duChain.insert({variableName, {}});
    definition.insert({variableName, ins});
}

void SPL_SSA::renameVariable() {
    std::queue<int> walkDTree;
    walkDTree.push(0);
    std::map<std::string, int> currentDef;


    for(auto& pair : variableListBlock) {
        currentDef.insert({pair.first, 0});
    }

    // 定义最近的def的位置
    std::vector<map<std::string, int>> closestDef(nodeSet.size(), map<std::string, int>{});
    closestDef[0] = currentDef;
    // 遍历D tree
    while(!walkDTree.empty()) {
        int index = walkDTree.front();
        walkDTree.pop();

        // 拷贝子节点到列表中用于之后遍历
        for(auto& childIndex : nodeSet[index]->DSet) {
            walkDTree.push(childIndex);
        }

        // 遍历原有的指令
        for(auto& ins : nodeSet[index]->instruSet) {
            if((ins->op == OP_ASSIGN || ins->op == OP_PHI ) && ins->res->cl == VAR) {
                updateDefinition(currentDef, closestDef, ins->res->name, index, ins);
            }
            if(ins->arg1 != nullptr && ins->arg1->cl == VAR)
                updateUsage(closestDef, ins->arg1->name, index, ins);
            if(ins->arg2 != nullptr && ins->arg2->cl == VAR)
                updateUsage(closestDef, ins->arg2->name, index, ins);
        }
    }


    // 重新命名phi变量的参数()
    for(const int& nodeIndex : phiBlock) {
        for(auto& ins : nodeSet[nodeIndex]->instruSet) {
            if(ins->op != OP_PHI) break;
            auto pos = ins->res->name.rfind('.');
            std::string variableName = ins->res->name.substr(0, pos);
            for(auto& parent : nodeSet[nodeIndex]->parentSet) {
                // 寻找最近的变量定义
                int index = parent;
                do {
                    auto it = closestDef[index].find(variableName);
                    if(it != closestDef[index].end()) {
                        auto temp = variableName + "." + std::to_string(it->second);
                        ins->addVariable(temp);
                        // 添加u-d链指针
                        auto it1 = duChain.find(temp);
                        it1->second.push_back(ins);
                        break;
                    } else {
                        // 向上寻找
                        index = nodeSet[index]->idom;
                    }
                } while (index > 0);
            }
        }
    }

}


void SPL_SSA::computeTreeIdom() {
    for(auto index = 0; index < nodeSet.size() ; index++ ) {
        computeIdom(index, nodeSet);
    }
}



void SPL_SSA::computeIdom(int index, std::vector<SSANode*>& nodeSet){
    auto current = nodeSet[index];
    if(current->idom != -1) return; // 已经算出来了
    if(current->parentSet.empty()) {
        current->idom = index;
        return; // 根节点没有idom
    }
    if(current->parentSet.size() == 1) {
        current->idom = current->parentSet[0];
        nodeSet[current->idom]->DSet.push_back(index);
        return;
    }
    if(current->parentSet.size() > 1) {
        std::set<int> s;
        std::vector<int> temp(current->parentSet);
        while(true) {
            for(auto parent : temp) {
                if (parent < index ) {
                    if(nodeSet[parent]->idom == -1) computeIdom(parent, nodeSet);
                    s.insert(nodeSet[parent]->idom);
                }
            }
            if(s.size() != 1) {
                temp.clear();
                for(auto elem : s) temp.push_back(elem);
                s.clear();
            } else {
                current->idom = *s.begin();
                nodeSet[current->idom]->DSet.push_back(index);
                return;
            }
        }
    }
}

// debug function

void SPL_SSA::outputIdom() {
    // 输出idom
    for(auto& node: nodeSet) {
        std::cout << &node - &nodeSet[0]  << ": " << node->idom << "\n";
    }

    std::cout << "-----------\n";
    // 输出DF
    for(auto& node: nodeSet){
        std::cout << &node - &nodeSet[0] << ": ";
        for(auto& df :node->DF){
            std::cout << df << " ";
        }
        std::cout << "\n";
    }
}


void SPL_SSA::outputPhiInstruction() {
    std::ofstream outfile;
    outfile.open("out.optimized.bc", std::ios::out);
    for(auto &node : nodeSet) {
        for(auto ins = node->instruSet.begin(); ins != node->instruSet.end(); ins ++) {
            if(*ins == nullptr) continue;
            (*ins)->output(std::cout);
            (*ins)->output(outfile);
        }

    }
    outfile.close();
}


void SPL_SSA::outputDUChain() {

    std::cout << "------------------DU Chain-----------------\n";
    for(auto& variable : duChain) {
        std::cout << variable.first << ":\n";
        for(auto& ins: variable.second) {
            ins->output(std::cout);
        }
        std::cout << "\n";
    }
}