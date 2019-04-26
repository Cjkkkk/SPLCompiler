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
    Instruction() = default;
    Instruction(std::string label_, SPL_OP op_, std::string arg1_, std::string arg2_, std::string result_)
    :label(label_), op(op_), arg1(arg1_), arg2(arg2_), result(result_){}
    virtual void addVariable(std::string name) {};
    virtual void output(ostream& s) {
        s << label << "\t" << SPL_OPToString(op) << "\t" << arg1 << "\t" << arg2 << "\t" <<result <<"\n";
    }
    std::string label;
    SPL_OP op;
    std::string arg1;
    std::string arg2;
    std::string result;
};

class PhiInstruction : public Instruction {
public:
    explicit PhiInstruction(const std::string& result_) {
        op = OP_PHI;
        result = result_;
    }
    std::vector<std::string> variableCluster;
    void addVariable(std::string name) override {variableCluster.push_back(name);}
    void output(ostream& s) override {
        s << label << "\t" << SPL_OPToString(op) << "\t" <<result;
        s << "(";
        for(auto& variable : variableCluster) {
            (&variable - &variableCluster[0]) == 0 ? s << variable : s << " ," << variable;
        }
        s << ")\n";
    }
};
class SPL_IR {
public:
    SPL_IR(SymbolTable* table):symbolTable(table), tempCount(0), labelCount(0){}

    void addInstruction(Instruction ins) {
        if(ins.label != "" && getLastInstruction()->op != OP_GOTO) {
            // need a trival goto
            IR.push_back({"", OP_GOTO, "", "", ins.label});
        }
        IR.push_back(ins);
    }

    std::string genTempVariable() {
        return "_t" + std::to_string(tempCount ++);
    }

    void decreaseTempCount(const string& name) {
        if(name[0] == '_') tempCount -= 1;
    }

    Instruction* getLastInstruction() {
        return &IR[IR.size() - 1];
    }

    std::string genLabel(){
        return "L" + std::to_string(labelCount ++);
    }

    std::vector<Instruction> IR;
    SymbolTable* symbolTable;
    unsigned int tempCount;
    unsigned int labelCount;
};


#endif //SPLCOMPILER_SPL_IR_H
