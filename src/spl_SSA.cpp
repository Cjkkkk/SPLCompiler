#include "spl_SSA.hpp"
#include "spl_exception.hpp"
#include <set>
#include <fstream>
#include <queue>

void SPL_SSA::OptimizeIR(std::vector<Instruction>& ins) {
    genCFGNode(ins);

    // generate CFG
    generateCFG();

    // compute idom
    computeTreeIdom();

    // compute DF
    generateDF();

    // output debug info
    debug();

    // insert Phi
    insertPhiFunction();

    // rename variable
    renameVariable();

    // output optimized IR
    outputPhiInstruction();

    outputDUChain();
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


void SPL_SSA::debug() {
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


void SPL_SSA::genCFGNode(std::vector<Instruction> &insSet) {
    SSANode* current = nullptr;
    for(Instruction& ins : insSet){
        if(ins.op == OP_ASSIGN && ins.result[0] != '_') {
            auto it = variableListBlock.find(ins.result);

            // 记录每一个变量出现的node列表
            if(it == variableListBlock.end()) {
                variableListBlock.insert({ins.result, {nodeSet.size() - 1}});
            } else{
                it->second.push_back(nodeSet.size() - 1);
            }
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
            current->LabelSet.push_back(&ins.result);
        } else {}
        current->instruSet.push_back(&ins);
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


void SPL_SSA::outputPhiInstruction() {
    std::ofstream outfile;
    outfile.open("out.optimized.bc", std::ios::out);
    for(auto &node : nodeSet) {
        // 输出标签
        node->instruSet[0]->output(std::cout);
        node->instruSet[0]->output(outfile);

        for(auto& ins : node->phiInstruSet) {
            ins->output(std::cout);
            ins->output(outfile);
        }

        for(auto ins = node->instruSet.begin() + 1 ; ins != node->instruSet.end(); ins ++) {
            (*ins)->output(std::cout);
            (*ins)->output(outfile);
        }

    }
    outfile.close();
}


// 插入phi 函数
void SPL_SSA::insertPhi(int nodeIndex, const string& variableName) {
    nodeSet[nodeIndex]->phiInstruSet.push_back(new PhiInstruction{variableName});

    // 更新有定义phi函数的node的index
    phiBlock.insert(nodeIndex);
    //nodeSet[nodeIndex]->instruSet.insert(nodeSet[nodeIndex]->instruSet.begin() + 1, new PhiInstruction{variableName});
}

void SPL_SSA::insertPhiFunction() {

    for(auto &pair : variableListBlock) {
        vector<int> copy(pair.second);
        vector<bool> PhiInserted(nodeSet.size(), false);
        vector<bool> added(nodeSet.size(), false);
        for(auto& nodeIndex : pair.second) {
            added[nodeIndex] = true;
        }
        while (!copy.empty()) {
            int b = copy.back();
            copy.pop_back();
            // get DF(b)
            auto DFb = nodeSet[b]->DF;
            for(auto& nodeIndex : DFb) {
                if(!PhiInserted[nodeIndex]) {
                    // add phiFunction
                    insertPhi(nodeIndex, pair.first);
                    PhiInserted[nodeIndex] = true;
                    if(!added[nodeIndex]) {
                        added[nodeIndex] = true;
                        copy.push_back(nodeIndex);
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

void SPL_SSA::mayBeUsage(map<std::string, int>& def,
        string& variableName,
        int& nodeIndex,
        int offset) {
    auto it1 = def.find(variableName);
    if(it1 != def.end()) {
        addVersionToVariable(variableName, it1->second);
        // 添加u-d链指针
        auto it = duChain.find(variableName);
        it->second.push_back({nodeIndex, offset});
    }
}

void SPL_SSA::mayBeDefinition(map<std::string, int>& currentDef,
        map<std::string, int>& closestDef,
        string& variableName,
        int& nodeIndex,
        int offset) {
    auto it = currentDef.find(variableName); // 更新定义
    if (it != currentDef.end()) {
        closestDef.find(variableName)->second = it->second;
        addVersionToVariable(variableName, it->second);
        it->second++;
    } else {
        currentDef.insert({variableName, 1});
        closestDef.insert({variableName, 0});
        addVersionToVariable(variableName, 0);
    }

    // 添加 d - u 链入口
    duChain.insert({variableName, {}});
    definition.insert({variableName, {nodeIndex, offset}});
}

void SPL_SSA::renameVariable() {
    std::queue<int> walkDTree;
    walkDTree.push(0);
    std::map<std::string, int> currentDef;


    for(auto& pair : variableListBlock) {
        for(auto& index : pair.second) {
            if(index == 0) currentDef.insert({pair.first, 0});
        }
    }

    // 定义最近的def的位置
    std::vector<map<std::string, int>> closestDef(nodeSet.size(), currentDef);

    // 遍历D tree
    while(!walkDTree.empty()) {
        int index = walkDTree.front();
        walkDTree.pop();

        // 拷贝子节点到列表中用于之后遍历
        for(auto& childIndex : nodeSet[index]->DSet) {
            walkDTree.push(childIndex);
        }

        // 拷贝idom节点中的所有变量的定义作为最新定义
        auto idom = nodeSet[index]->idom;
        closestDef[index] = closestDef[idom];

        // 首先遍历phi定义,反正本来就是定义在开头的
        for(auto& ins : nodeSet[index]->phiInstruSet) {
            mayBeDefinition(currentDef, closestDef[index], ins->result, index, &nodeSet[index]->phiInstruSet[0] - &ins);
        }

        // 遍历原有的指令
        for(auto& ins : nodeSet[index]->instruSet) {
            if(ins->op == OP_ASSIGN && ins->result[0] != '_') {
                mayBeDefinition(currentDef, closestDef[index], ins->result, index, &ins - &nodeSet[index]->instruSet[0]);
            }
            mayBeUsage(closestDef[index], ins->arg1, index, &ins - &nodeSet[index]->instruSet[0]);
            mayBeUsage(closestDef[index], ins->arg2, index, &ins - &nodeSet[index]->instruSet[0]);
        }
    }


    // 重新命名phi变量
    for(const int& nodeIndex : phiBlock) {
        for(auto& ins : nodeSet[nodeIndex]->phiInstruSet) {
            auto pos = ins->result.rfind('.');
            std::string variableName = ins->result.substr(0, pos);
            for(auto& parent : nodeSet[nodeIndex]->parentSet) {

                auto it = closestDef[parent].find(variableName); // 寻找最近的变量定义
                ins->addVariable(variableName + "." + std::to_string(it->second));

                // 更新d-u链
                duChain.find(variableName + "." + std::to_string(it->second))->second.push_back({nodeIndex, &nodeSet[nodeIndex]->phiInstruSet[0] - &ins});

            }
        }
    }

}


void SPL_SSA::outputDUChain() {

    std::cout << "------------------DU Chain-----------------\n";
    for(auto& variable : duChain) {
        std::cout << variable.first << ":\n";
        for(auto& pair: variable.second) {
            auto ins = pair.second > 0 ? nodeSet[pair.first]->instruSet[pair.second] : nodeSet[pair.first]->phiInstruSet[-pair.second];
            ins->output(std::cout);
        }
        std::cout << "\n";
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
