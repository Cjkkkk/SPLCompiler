//
// Created by DELL on 2019/4/21.
//

#ifndef SPLCOMPILER_SPL_IR_H
#define SPLCOMPILER_SPL_IR_H

#include <iostream>
#include <vector>
#include "spl_compiler.hpp"
#include "spl_symtab.hpp"

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
class SPL_IR {
public:
    SPL_IR(SymbolTable* table):tempCount(0), labelCount(0), symbolTable(table){}

    void addInstruction(Instruction ins) {
        IR.push_back(ins);
    }
    std::string genTempVariable(){
        return "_t" + std::to_string(tempCount ++);
    }

    std::string genTempVariable(std::string name){
        auto it = tempMap.find(name);
        if(it != tempMap.end()) {
            //找到了
            return it->second;
        }
        else {
            auto temp = genTempVariable();
            tempMap.insert({name, temp});
            return temp;
        }
    }
    std::string genLabel(){
        return "L" + std::to_string(labelCount ++);
    }
    std::vector<Instruction> IR;
    std::map<std::string, std::string> tempMap;
    SymbolTable* symbolTable;
    unsigned int tempCount;
    unsigned int labelCount;
};


#endif //SPLCOMPILER_SPL_IR_H
