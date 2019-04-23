//
// Created by DELL on 2019/4/22.
//

#ifndef SPLCOMPILER_SPL_SSA_H
#define SPLCOMPILER_SPL_SSA_H

#include <vector>
#include "spl_IR.hpp"
class SSANode {
public:
    std::vector<Instruction*> instruSet;
    std::vector<SSANode*> nodeSet;
};
class SPL_SSA {
public:
    SSANode* root;
};


#endif //SPLCOMPILER_SPL_SSA_H
