//
// Created by DELL on 2019/4/22.
//

#ifndef SPLCOMPILER_SPL_SSA_H
#define SPLCOMPILER_SPL_SSA_H

#include <vector>
#include "spl_IR.hpp"
class SSANode {
public:
    SSANode():idom (-1) {};
    std::vector<Instruction*> instruSet;
    std::vector<int> childSet;
    std::vector<int> parentSet;
    std::vector<int> DF;
    int idom;
    std::vector<std::string*> LabelSet;
    std::string* label;
};
class SPL_SSA {
public:
    std::vector<SSANode*> set;
    void genSSATree(std::vector<Instruction>& ins);
    void computeIdom(int index, std::vector<SSANode*>& nodeSet);
};


#endif //SPLCOMPILER_SPL_SSA_H
