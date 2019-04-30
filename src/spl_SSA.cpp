#include "spl_SSA.hpp"
#include "spl_exception.hpp"
#include <set>
#include <fstream>
#include <queue>

void SPL_SSA::OptimizeIR(std::vector<Instruction>& ins) {
    genCFGNode(ins);

    // generate CFG
    generateCFG();

    // compute Immediate dom
    computeTreeIdom();

    // compute DF
    generateDF();

    // insert Phi
    insertPhiFunction();

    // rename variable
    renameVariable();


    outputPhiInstruction("out.bc");

    // --------------------------------------------
    // output optimized IR
    copyPropagation();

    outputPhiInstruction("out.copy_propagation.bc");

    constantPropagation();

    outputPhiInstruction("out.const_propagation.bc");

    removeUnusedVariable();

    outputPhiInstruction("out.remove_var.bc");
    //

    // ---------------------------------------------
    outputIdom();

    outputDUChain();
}

void propagateAlongDuchain(std::vector<Instruction*> usage, Operand* res, const string& target) {
    for(auto& use : usage) {
        if(use->op == OP_PHI) {
            for(auto& var : *use->getVariable()) {
                if(var->name == target) {
                    *var = *res;
                }
            }

//            if(use->getVariable()->size() == 1 && use->getVariable()->at(0)->cl == CONST){
//                auto t = use->res->name;
//                *use->res = *res;
//                propagateAlongDuchain(usage, use->res, t);
//            }
        }
        if(use->arg1 && use->arg1->name == target) {
            *use->arg1 = *res;
        }
        if(use->arg2 && use->arg2->name == target) {
            *use->arg2 = *res;
        }
    }
}
void SPL_SSA::constantPropagation() {
    map<std::string, Operand*> constMap;
    for(auto& node: nodeSet) {
        for(auto ins_it = node->instruSet.begin() ; ins_it != node->instruSet.end(); ins_it ++) {
            if(isCalculateOp((*ins_it)->op)) {
                if(checkOperandClass((*ins_it)->arg1,CONST)
                   && checkOperandClass((*ins_it)->arg2,CONST)) {
                    SPL_OP op = (*ins_it)->op;
                    auto it = duChain.find((*ins_it)->res->name);
                    (*ins_it)->res->evalute(op,(*ins_it)->arg1, (*ins_it)->arg2);
                    propagateAlongDuchain(it->second, (*ins_it)->res, it->first);
                    ins_it = node->instruSet.erase(ins_it);
                    --ins_it;
                }
                continue;
            }
            switch((*ins_it)->op) {
                case OP_ASSIGN:
                    if(checkOperandClass((*ins_it)->arg1, CONST)
                       && checkOperandType((*ins_it)->arg1, INT)) {
                        auto it = duChain.find((*ins_it)->res->name);
                        *((*ins_it)->res) = *((*ins_it)->arg1);
                        propagateAlongDuchain(it->second, (*ins_it)->res, it->first);
                        ins_it = node->instruSet.erase(ins_it);
                        --ins_it;
                    }
            }
        }
    }
}


// algorithm: copy propagation
// loop through all definition from top to bottom
void SPL_SSA::copyPropagation() {
    for(auto& var : definition ) {
        auto ins = var.second;
        if(ins->op != OP_ASSIGN || ins->res->cl != VAR ) continue;

        auto usage = duChain.find(ins->res->name)->second;

        for(auto& use : usage) {
            if(use->arg1 && use->arg1->name == ins->res->name) {
                use->arg1 = ins->arg1;
            }
            if(use->arg2 && use->arg2->name == ins->res->name) {
                use->arg2 = ins->arg2;
            }
        }
    }

}

void SPL_SSA::removeUnusedVariable() {
    set<std::string> usage; // 有使用的变量
    set<std::string> deleted; // 删除的变量

    auto node_it = nodeSet.end();
    while (node_it != nodeSet.begin()) {
        -- node_it;
        auto node = *node_it;
        auto ins_it = node->instruSet.end();
        while(ins_it != node->instruSet.begin()) {
            -- ins_it;
            // res 字段不为空说明产生了赋值的操作
            if((*ins_it)->res && ((*ins_it)->res->cl == VAR || (*ins_it)->res->cl == TEMP)) {

                // 查看变量是否使用过
                auto whether_used = usage.find((*ins_it)->res->name);

                if(whether_used == usage.end()) {
                    // 没有使用过这个变量 删除
                    deleted.insert((*ins_it)->res->name);
                    ins_it = node->instruSet.erase(ins_it);

                } else {
                    // 添加赋值的参数数到使用的变量集合中
                    if((*ins_it)->op == OP_PHI) {
                        auto varList = (*ins_it)->getVariable();
                        if(!varList) continue;
                        for(auto var_it = varList->begin() ; var_it != varList->end() ; var_it ++) {
                            if(deleted.find((*var_it)->name) != deleted.end()) {
                                varList->erase(var_it--);
                            } else {
                                usage.insert((*var_it)->name);
                            }
                        }
                    } else {
                        if((*ins_it)->arg1) usage.insert((*ins_it)->arg1->name);
                        if((*ins_it)->arg2) usage.insert((*ins_it)->arg2->name);
                    }
                }
            }
            else {
                // 不是赋值的语句 添加参数到使用变量集合中

                if((*ins_it)->arg1) usage.insert((*ins_it)->arg1->name);

                if((*ins_it)->arg2) usage.insert((*ins_it)->arg2->name);
            }
        }
    }
}


void SPL_SSA::generateDF() {
    for(auto& node: nodeSet){
        if(node->parentSet.size() >= 2) {
            for(auto& parent : node->parentSet) {
                int runner = node->parentSet[&parent - &node->parentSet[0]];
                while (runner != node->idom) {
                    nodeSet[runner]->DF.push_back(static_cast<int>(&node - &nodeSet[0]));
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
                it->second.push_back(static_cast<int>(nodeSet.size() - 1));
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
    for(auto index = 0 ; index < nodeSet.size() ; index ++) {
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
                          Operand* operand,
                          int& nodeIndex,
                          Instruction* ins) {
    if(operand->cl == TEMP) {
        auto it = duChain.find(operand->name);
        if(it == duChain.end())
            return;
        else {
            it->second.push_back(ins);
        }
    } else if(operand->cl == VAR) {
        int index = nodeIndex;
        do {
            auto it = def[index].find(operand->name);
            if(it != def[index].end()) {
                addVersionToVariable(operand->name, it->second);
                // 添加u-d链指针
                duChain.find(operand->name)->second.push_back(ins);
                return;
            } else {
                // 向上寻找
                index = nodeSet[index]->idom;
            }
        } while (index > 0);
    }
}


void SPL_SSA::updateDefinition(map<std::string, int>& currentDef,
                               std::vector<map<std::string, int>>& closestDef,
                               Operand* operand,
                               int& nodeIndex,
                               Instruction* ins) {
    if(operand->cl == TEMP) {

        duChain.insert({operand->name, {}});
        definition.push_back({operand->name, ins});

    } else if(operand->cl == VAR) {
        auto it = currentDef.find(operand->name); // 更新定义
        auto it1 = closestDef[nodeIndex].find(operand->name);

        if(it1 == closestDef[nodeIndex].end()) {
            closestDef[nodeIndex].insert({operand->name, it->second});
        } else {
            it1->second = it->second;
        }
        addVersionToVariable(operand->name, it->second ++ );

        // 添加 d - u 链入口
        duChain.insert({operand->name, {}});
        definition.push_back({operand->name, ins});
    }
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
            if(ins->res != nullptr) {
                updateDefinition(currentDef, closestDef, ins->res, index, ins);
            }
            if(ins->arg1 != nullptr)
                updateUsage(closestDef, ins->arg1, index, ins);
            if(ins->arg2 != nullptr)
                updateUsage(closestDef, ins->arg2, index, ins);
        }
    }


    // 重新命名phi变量的参数()
    for(const int& nodeIndex : phiBlock) {
        for(auto& ins : nodeSet[nodeIndex]->instruSet) {
            if(ins->op != OP_PHI) break;
            auto temp = ins->res->name;
            for(auto& parent : nodeSet[nodeIndex]->parentSet) {
                // 寻找最近的变量定义
                ins->res->name = ins->res->name.substr(0, ins->res->name.rfind('.'));
                updateUsage(closestDef, ins->res, parent, ins);
                ins->addVariable(new Operand(*ins->res));
            }
            ins->res->name = temp;
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


void SPL_SSA::outputPhiInstruction(std::string filename) {
    std::ofstream outfile;
    outfile.open(filename, std::ios::out);
    for(auto &node : nodeSet) {
        outfile << *node->label << "\n";
        std::cout << *node->label << "\n";
        for(auto ins : node->instruSet) {
            ins->output(std::cout);
            ins->output(outfile);
        }

    }
    outfile.close();
}


void SPL_SSA::outputDUChain() {

    std::cout << "------------------DU Chain-----------------\n";
    for(auto& def : definition) {
        auto variable = duChain.find(def.first)->second;
        std::cout << def.first << ":\n";
        for(auto& ins: variable) {
            ins->output(std::cout);
        }
        std::cout << "\n";
    }
}
