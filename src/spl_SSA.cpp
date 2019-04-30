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


void SPL_SSA::constantPropagation() {
    map<std::string, Operand*> constMap;
    for(auto& node: nodeSet) {
        for(auto ins_it = node->instruSet.begin() ; ins_it != node->instruSet.end(); ins_it ++) {
            if(isCalculateOp((*ins_it)->op)) {
                if(checkOperandClass((*ins_it)->res,TEMP)
                   && checkOperandClass((*ins_it)->arg1,CONST)
                   && checkOperandClass((*ins_it)->arg2,CONST)) {
                    SPL_OP op = (*ins_it)->op;
                    (*ins_it)->res->evalute(op,(*ins_it)->arg1, (*ins_it)->arg2);
                    ins_it = node->instruSet.erase(ins_it);
                    --ins_it;
                }
                continue;
            }
            switch((*ins_it)->op) {
                case OP_ASSIGN:
                    if(checkOperandClass((*ins_it)->res, TEMP)
                       && checkOperandClass((*ins_it)->arg1, CONST)
                       && checkOperandType((*ins_it)->arg1, INT)) {
                        *((*ins_it)->res) = *((*ins_it)->arg1);
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
        if(ins->op != OP_ASSIGN) continue;
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
                            if(deleted.find(*var_it) != deleted.end()) {
                                varList->erase(var_it--);
                            } else {
                                usage.insert(*var_it);
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


void SPL_SSA::updateUsage(std::vector<map<std::string, Operand*>>& def,
                          Operand* variable,
                          int& nodeIndex,
                          Instruction* ins) {

    int index = nodeIndex;
    do {
        auto it = def[index].find(variable->name);
        if(it != def[index].end()) {
            // 添加u-d链指针
            variable = it->second;
            auto du_it = duChain.find(variable->name);
            du_it->second.push_back(ins);
            return;
        } else {
            // 向上寻找
            index = nodeSet[index]->idom;
        }
    } while (index > 0);
}


void SPL_SSA::updateDefinition(map<std::string, Operand*>& currentDef,
                               std::vector<map<std::string, Operand*>>& closestDef,
                               Operand* variable,
                               int& nodeIndex,
                               Instruction* ins) {

    auto it = currentDef.find(variable->name); // 更新定义
    auto it1 = closestDef[nodeIndex].find(variable->name);
    if(it->second == nullptr) {
        addVersionToVariable(variable->name, 0);
        it->second = variable;
    } else {
        *variable = *(it->second);
        variable->name = variable->name.substr(0, variable->name.rfind('.'));
        addVersionToVariable(variable->name, 1);
        it->second = variable;
    }


    if(it1 == closestDef[nodeIndex].end()) {
        closestDef[nodeIndex].insert({variable->name, it->second});
    } else {
        it1->second = it->second;
    }

    // 添加 d - u 链入口
    duChain.insert({variable->name, {}});
    definition.push_back({variable->name, ins});
}


void SPL_SSA::renameVariable() {
    std::queue<int> walkDTree;
    walkDTree.push(0);
    std::map<std::string, Operand*> currentDef;


    for(auto& pair : variableListBlock) {
        currentDef.insert({pair.first, nullptr});
    }

    // 定义最近的def的位置
    std::vector<map<std::string, Operand*>> closestDef(nodeSet.size(), map<std::string, Operand*>{});
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
                updateDefinition(currentDef, closestDef, ins->res, index, ins);
            }
            if(ins->arg1 != nullptr && ins->arg1->cl == VAR)
                updateUsage(closestDef, ins->arg1, index, ins);
            if(ins->arg2 != nullptr && ins->arg2->cl == VAR)
                updateUsage(closestDef, ins->arg2, index, ins);
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
//                auto temp = variableName;
//                updateUsage(closestDef, temp, parent, ins);
//                ins->addVariable(temp);
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
    for(auto& variable : duChain) {
        std::cout << variable.first << ":\n";
        for(auto& ins: variable.second) {
            ins->output(std::cout);
        }
        std::cout << "\n";
    }
}
