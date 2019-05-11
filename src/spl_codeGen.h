//
// Created by DELL on 2019/4/30.
//

#ifndef SPLCOMPILER_SPL_CODEGEN_H
#define SPLCOMPILER_SPL_CODEGEN_H

#include <map>
#include <list>
#include "spl_IR.hpp"
class SPL_CodeGen {
public:
    explicit SPL_CodeGen( SPL_IR* ir_): ir(ir_){};
    void GenerateMachineCode(){};
    std::map<std::string, int> name_to_memory;
    SPL_IR* ir;
};


#endif //SPLCOMPILER_SPL_CODEGEN_H
