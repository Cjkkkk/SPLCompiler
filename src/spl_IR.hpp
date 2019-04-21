//
// Created by DELL on 2019/4/21.
//

#ifndef SPLCOMPILER_SPL_IR_H
#define SPLCOMPILER_SPL_IR_H

#include <iostream>
#include <vector>
#include "spl_compiler.hpp"

class Instruction {
public:
    Instruction(std::string label_, SPL_OP op_, std::string arg1_, std::string arg2_, std::string result_)
    :label(label_), op(op_), arg1(arg1_), arg2(arg2_), result(result_){}
    std::string label;
    SPL_OP op;
    std::string arg1;
    std::string arg2;
    std::string result;
};
class spl_IR {
public:
    spl_IR():tempCount(0), labelCount(0){

    }
    std::vector<Instruction> IR;
    std::string genTempVariable(){
        return "_t" + std::to_string(tempCount ++);
    }
    std::string genLabel(){
        return "L" + std::to_string(labelCount ++);
    }
    int tempCount;
    int labelCount;
};


#endif //SPLCOMPILER_SPL_IR_H
