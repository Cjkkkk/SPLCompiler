//
// Created by DELL on 2019/4/22.
//

#include <set>
#include "spl_SSA.hpp"
#include <fstream>
#include <stack>
#include <queue>

void SPL_SSA::OptimizeIR(std::vector<Instruction>& ins) {
    genSSATree(ins);
    // generate CFG
    generateCFG();
    // compute SD
    computeTreeIdom();
    // output debug info
    debug();

    // insert Phi
    insertPhiFunction();

    // rename variable
    renameVariable();

    // output optimized IR
    outputPhiInstruction();
}

void SPL_SSA::debug() {
    for(int index = 0 ; index < nodeSet.size() ; index ++) {
        std::cout << index  << ": " << nodeSet[index]->idom << "\n";
    }
    std::cout << "-----------\n";
    for(int index = 0 ; index < nodeSet.size(); index ++){
        SSANode* b = nodeSet[index];
        if(b->parentSet.size() >= 2) {
            for(int parent_index = 0 ; parent_index < b->parentSet.size() ; parent_index ++) {
                int runner = b->parentSet[parent_index];
                while (runner != b->idom) {
                    nodeSet[runner]->DF.push_back(index);
                    runner = nodeSet[runner]->idom;
                }
            }
        }
    }

    for(int index = 0 ; index < nodeSet.size() ; index ++){
        std::cout << index << ": ";
        for(int i = 0 ; i < nodeSet[index]->DF.size() ; i ++){
            std::cout << nodeSet[index]->DF[i] << " ";
        }
        std::cout << "\n";
    }
}
void SPL_SSA::genSSATree(std::vector<Instruction> &insSet) {
    SSANode* current;
    for(Instruction& ins : insSet){
        if(ins.op == OP_ASSIGN && ins.result[0] != '_') {
            auto it = variableListBlock.find(ins.result);
            if(it == variableListBlock.end()) {
                variableListBlock.insert({ins.result, {nodeSet.size() - 1}});
            } else{
                it->second.push_back(nodeSet.size() - 1);
            }
        }
        else if(ins.label != "") {
            // start target
            SSANode* newNode = new SSANode();
            nodeSet.push_back(newNode);
            current = newNode;
            current->label = &ins.label;
            labelIndexMap.insert({*current->label, nodeSet.size() - 1});
        } else if (ins.op == OP_IF || ins.op == OP_IF_Z || ins.op == OP_GOTO) {
            // 添加子节点
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
        for(auto ins : node->instruSet) {
            ins->output(std::cout);
            ins->output(outfile);
        }
    }
    outfile.close();
}

// 插入phi 函数
void SPL_SSA::insertPhi(int nodeIndex, const string& variableName) {
    nodeSet[nodeIndex]->instruSet.insert(nodeSet[nodeIndex]->instruSet.begin() + 1, new PhiInstruction{variableName});
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
void SPL_SSA::renameVariable() {
    std::queue<int> walkDTree;
    walkDTree.push(0);
    std::map<std::string, int> currentDef;


    for(auto& pair : variableListBlock) {
        currentDef.insert({pair.first, 0});
    }

    // 定义最近的def的位置
    std::vector<map<std::string, int>> closestDef;
    for(auto i = 0 ; i < nodeSet.size(); i++) {
        closestDef.push_back(currentDef);
    }

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


        for(auto& ins : nodeSet[index]->instruSet) {
            if(ins->op == OP_ASSIGN || ins->op == OP_PHI) {
                auto it = currentDef.find(ins->result); // 更新定义
                if(it != currentDef.end()){
                    closestDef[index].find(ins->result)->second = it->second;
                    ins->result = ins->result + "." + std::to_string(it->second);
                    it->second ++;
                }
            }

            auto it1 = closestDef[index].find(ins->arg1);
            if(it1 != closestDef[index].end()) {
                ins->arg1 = ins->arg1 + "." + std::to_string(it1->second);

            }
            auto it2 = closestDef[index].find(ins->arg2);
            if(it2 != closestDef[index].end()) {
                ins->arg2 = ins->arg2 + "." + std::to_string(it2->second);
            }
        }
    }


    //
    for(int index = 0 ; index < nodeSet.size() ; index ++ ) {
        for(auto& ins : nodeSet[index]->instruSet) {
            if (ins->op == OP_PHI) {
                auto pos = ins->result.rfind('.');
                std::string variableName = ins->result.substr(0, pos);
                for(auto& parent : nodeSet[index]->parentSet) {
                    auto it = closestDef[parent].find(variableName);
                    ins->addVariable(variableName + std::to_string(it->second));
                }
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
    if(current->parentSet.size() == 0) {
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
        while(1) {
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
