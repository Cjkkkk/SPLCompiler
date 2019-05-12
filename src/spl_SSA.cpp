#include "spl_SSA.hpp"
#include "spl_exception.hpp"
#include <set>
#include <fstream>
#include <algorithm>
#include <queue>
#include <stack>

void SPL_SSA::OptimizeIR() {
    genCFGNode();

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

    auto prefix = "byte_code/" + *nodeSet[0]->label + "/";
    outputInstruction(prefix +"origin.bc");

    // --------------------------------------------
    // output optimized IR
    copyPropagation();

//    outputDUChain();

    outputInstruction(prefix + "copy_propagation.bc");

    constantPropagation();

    outputInstruction(prefix + "const_propagation.bc");

    removeUnusedVariable();

    outputInstruction(prefix + "remove_var.bc");

    backToTAC(insSet);


//    std::ofstream outfile;
//    outfile.open("byte_code/opt.bc", std::ios::out);
//    for(auto& instr:ir->IR) {
//        instr->output(outfile);
//    }
//    outfile.close();

// ---------------------------------------------
//    outputIdom();
//
//    outputDUChain();
}

void SPL_SSA::printIR(ostream& s) {
    for(auto& ins:insSet) {
        ins->output(s);
    }
}

void removeSubScript(Operand* var) {
    if(var && checkOperandClass(var, VAR)) {
        auto pos = var->name.rfind('.');
        var->name = var->name.substr(0, pos);
    }
}

bool isSameVariable(Operand* a, Operand* b) {
    auto a_pos = a->name.rfind('.');
    auto b_pos = b->name.rfind('.');
    if(a_pos == std::string::npos || b_pos == std::string::npos) {
        throw invalid_argument{"debug info > isSameVariable expect var.\n"};
    }
    return a->name.substr(0, a_pos) == b->name.substr(0, b_pos);
}

void SPL_SSA::backToTAC(std::vector<Instruction*>& ins){
    for(auto index : phiBlock) {
        for(auto ins: nodeSet[index]->instruSet) {
            if( ! checkInstructionOp(ins, OP_PHI) ) break;
            for(auto var: *ins->getVariable()) {
                if(checkOperandClass(var.first, CONST)
                || checkOperandClass(var.first, TEMP)
                || (checkOperandClass(var.first, VAR) && !isSameVariable(var.first, ins->res))) {
                    // write definition
                    auto pre = var.second;
                    auto ins_it = nodeSet[pre]->instruSet.end();
                    while(ins_it != nodeSet[pre]->instruSet.begin()) {
                        ins_it --;
                        if(checkInstructionOp(*ins_it, OP_GOTO) || ins_it == nodeSet[pre]->instruSet.begin()) {
                            Operand* new_operand = new Operand{var.first->type, ins->res->name, VAR};
                            nodeSet[pre]->instruSet.insert(ins_it, new Instruction{"", OP_ASSIGN, var.first, nullptr, new_operand});
                            break;
                        }
                    }
                }
            }
        }
    }

    std::vector<Instruction*> temp;
    for(auto it = nodeSet.begin(); it != nodeSet.end() ; it++) {
        temp.push_back(new Instruction{(*(*it)->label), OP_NULL, nullptr, nullptr, nullptr});
        for(auto& instruction: (*it)->instruSet) {
            if(instruction->op == OP_GOTO) {
                if(it + 1 != nodeSet.end() && *((*(it + 1))->label) == instruction->res->name) continue;
            }
            if(instruction->op == OP_PHI) continue;
            removeSubScript(instruction->arg1);
            removeSubScript(instruction->arg2);
            removeSubScript(instruction->res);
            temp.push_back(instruction);
//            ins[index++] = instruction;
        }
    }
    std::copy(temp.begin(), temp.end(), ins.begin());
    ins.resize(temp.size());
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



void SPL_SSA::genCFGNode() {
    SSANode* current = nullptr;

    // 返回当前作用域能访问到的所有变量与常量
//    auto index = ir->symbolTable->getCurrentScopeIndex();
//
//    while(ir->symbolTable->getCurrentScopeIndex() >= 0) {
//        auto symVec = ir->symbolTable->getVariableByScopeIndex();
//        for(auto it = symVec->begin() ; it != symVec->end() ; it ++) {
//            if(it->second->symbolClass == VAR || it->second->symbolClass == CONST) {
//                variableListBlock.insert(
//                        {std::to_string(ir->symbolTable->getCurrentScopeIndex()) +"."+ it->first, {}});
//            }
//        }
//        if(ir->symbolTable->getCurrentScopeIndex() == 0) break;
//        ir->symbolTable->setToPrevScopeIndex();
//    }
//    // 重置ScopeIndex为当前index
//
//    ir->symbolTable->setCurrentScopeIndex(index);

    for(Instruction* ins : insSet){
        if(checkInstructionOp(ins, OP_ASSIGN) && checkOperandClass(ins->res, VAR)){
            auto it = variableListBlock.find(ins->res->name);

            // 记录每一个变量出现的node列表
            if(it == variableListBlock.end()) {
                variableListBlock.insert({ins->res->name, {static_cast<int>(nodeSet.size() - 1)}});
            } else{
                it->second.push_back(static_cast<int>(nodeSet.size() - 1));
            }

            current->instruSet.push_back(ins);
        }
            // 开始新的node
        else if(!ins->label.empty()) {

            auto newNode = new SSANode();
            current = newNode;
            nodeSet.push_back(newNode);
            current->label = &ins->label;
            labelIndexMap.insert({*current->label, nodeSet.size() - 1});

        } else if (checkInstructionOp(ins, OP_IF)
        || checkInstructionOp(ins, OP_IF_Z) || checkInstructionOp(ins, OP_GOTO)) {
            // 添加子节点
            if ( current == nullptr) throw splException{0, 0, "no label at start of target."};
            current->LabelSet.push_back(&ins->res->name);
            current->instruSet.push_back(ins);
        } else {current->instruSet.push_back(ins);}
    }

//    for(auto var: variableListBlock){
//        std::cout << var.first << "\n";
//        for(auto pos: var.second) {
//            std::cout << pos << " ";
//        }
//        std::cout << "\n";
//    }
//    std::cout << "--------------";
}


void SPL_SSA::generateCFG() {
    for(auto& node : nodeSet) {
        auto currentNodeOffset = &node - &nodeSet[0];
        for(auto& label : node->LabelSet) {
            auto it = labelIndexMap.find(*label);
            if(it == labelIndexMap.end()) {
                throw invalid_argument{"debug info > generateCFG encounter a non exist label."};
            }
            int offset = it->second; // 找到子女的label对应的offset
            nodeSet[offset]->parentSet.push_back(static_cast<int>(currentNodeOffset)); // parent set
        }
    }
}


// 插入phi 函数
void SPL_SSA::insertPhi(int nodeIndex, const string& variableName) {
    auto it = nodeSet[nodeIndex]->instruSet.begin();
    auto phi = new PhiInstruction{new Operand(UNKNOWN, variableName, VAR)};
    phi->unique_id = ir->getIdCount();
    nodeSet[nodeIndex]->instruSet.insert(it, phi);

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


void SPL_SSA::updateUsage(Operand* operand,
                          int& nodeIndex,
                          Instruction* ins) {
    if(checkOperandClass(operand, TEMP)) {
        auto it = nameUsageMap.find(operand->name);
        if(it == nameUsageMap.end()){
            throw invalid_argument{"debug info> can not find a temp variable definition in updateUsage method."};
            return;
        }
        else {
            it->second.push_back(ins);
        }
    } else if(checkOperandClass(operand, VAR)) {
        int index = nodeIndex;
        do {
            auto it = closestDef[index].find(operand->name);
            if(it != closestDef[index].end()) {
                addVersionToVariable(operand->name, it->second);
                // 添加u-d链指针
                nameUsageMap.find(operand->name)->second.push_back(ins);
                return;
            } else {
                // 向上寻找
                auto pre = index;
                index = nodeSet[index]->idom;
                if(pre == index) {
                    //root
                    throw invalid_argument{"debug info > can not find a real variable definition in updateUsage method."};
                    return;
                }
            }
        } while (index >= 0);
    }
}


void SPL_SSA::updateDefinition(Operand* operand,
                               int& nodeIndex,
                               Instruction* ins) {
    if(checkOperandClass(operand, TEMP)) {

        nameUsageMap.insert({operand->name, {}});
        nameDefinitionMap.push_back({operand->name, ins});

    } else if(checkOperandClass(operand, VAR)) {

        auto it = currentDef.find(operand->name); // 更新定义

        if(it == currentDef.end()) {
            throw invalid_argument{"debug info > can not find a variable definition in updateDefinition method."};
        }
        auto it1 = closestDef[nodeIndex].find(operand->name);

        if(it1 == closestDef[nodeIndex].end()) {
            closestDef[nodeIndex].insert({operand->name, it->second});
        } else {
            it1->second = it->second;
        }
        addVersionToVariable(operand->name, it->second ++ );

        // 添加 d - u 链入口
        nameUsageMap.insert({operand->name, {}});
        nameDefinitionMap.push_back({operand->name, ins});
    }
}


void SPL_SSA::renameVariable() {
    std::queue<int> walkDTree;
    walkDTree.push(0);


    for(auto& pair : variableListBlock) {
        currentDef.insert({pair.first, 0});
    }

    // 定义最近的def的位置
    for(auto i = 0 ; i < nodeSet.size() ; i ++) {
        closestDef.push_back({});
    }
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
            if(ins->res)
                updateDefinition(ins->res, index, ins);
            if(ins->arg1)
                updateUsage(ins->arg1, index, ins);
            if(ins->arg2)
                updateUsage(ins->arg2, index, ins);
        }
    }


    // 重新命名phi变量的参数()
    for(const int& nodeIndex : phiBlock) {
        for(auto& ins : nodeSet[nodeIndex]->instruSet) {
            if(! checkInstructionOp(ins, OP_PHI) ) break;
            auto temp = ins->res->name;
            for(auto& parent : nodeSet[nodeIndex]->parentSet) {
                // 寻找最近的变量定义
                ins->res->name = ins->res->name.substr(0, ins->res->name.rfind('.'));
                updateUsage(ins->res, parent, ins);
                ins->addVariable(new Operand(*ins->res), parent);
            }
            ins->res->name = temp;
        }
    }

}


void SPL_SSA::computeTreeIdom() {
    for(auto& node: nodeSet ) {
        computeIdom(static_cast<int>(&node - &nodeSet[0]), nodeSet);
    }
}



void SPL_SSA::computeIdom(int index, std::vector<SSANode*>& nodeSet){
    auto current = nodeSet[index];
    if(current->idom != -1) return; // 已经算出来了
    if(current->parentSet.empty()) {
        current->idom = 0;
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


void SPL_SSA::outputInstruction(std::string filename) {
    std::ofstream outfile;
    outfile.open(filename, std::ios::out);
    for(auto &node : nodeSet) {
        outfile << *node->label << "\n";
//        std::cout << *node->label << "\n";
        for(auto ins : node->instruSet) {
            //ins->output(std::cout);
            ins->output(outfile);
        }

    }
    outfile.close();
}


void SPL_SSA::outputDUChain() {

    std::cout << "------------------DU Chain-----------------\n";
    for(auto& def : nameDefinitionMap) {
        auto variable = nameUsageMap.find(def.first)->second;
        std::cout << def.first << ":\n";
        for(auto& ins: variable) {
            ins->output(std::cout);
        }
        std::cout << "\n";
    }
}
