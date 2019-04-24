//
// Created by DELL on 2019/4/22.
//

#include <set>
#include "spl_SSA.hpp"

void SPL_SSA::genSSATree(std::vector<Instruction> &insSet) {
    std::vector<SSANode*> nodeSet;
    SSANode* current;
    std::map<std::string, int> labelIndexMap;
    for(Instruction& ins : insSet){
        if(ins.label != "") {
            // start target
            SSANode* newNode = new SSANode();
            nodeSet.push_back(newNode);
            current = newNode;
            current->label = &ins.label;
            labelIndexMap.insert({*current->label, nodeSet.size() - 1});
        } else if (ins.op == OP_IF || ins.op == OP_IF_Z || ins.op == OP_GOTO) {
            // 添加子节点
            current->LabelSet.push_back(&ins.result);
        } else {
            // 添加指针
            current->instruSet.push_back(&ins);
        }
    }

    // generate CFG
    for(int index = 0 ; index < nodeSet.size() ; index ++) {
        int currentNodeOffset = index;
        SSANode* node = nodeSet[index];
        for(auto label : node->LabelSet) {
            int offset = labelIndexMap.find(*label)->second; // 找到子女的label对应的offset
            node->childSet.push_back(offset); // child set
            nodeSet[offset]->parentSet.push_back(currentNodeOffset); // parent set
        }
    }

    // compute SD
    for(int index = 0; index < nodeSet.size() ; index++ ) {
        computeIdom(index, nodeSet);
    }
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
void SPL_SSA::computeIdom(int index, std::vector<SSANode*>& nodeSet){
    auto current = nodeSet[index];
    if(current->idom != -1) return; // 已经算出来了
    if(current->parentSet.size() == 0) {
        current->idom = index;
        return; // 根节点没有idom
    }
    if(current->parentSet.size() == 1) {
        current->idom = current->parentSet[0];
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
                return;
            }
        }
    }
}
