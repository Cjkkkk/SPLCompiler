//
// Created by DELL on 2019/4/21.
//

#ifndef SPLCOMPILER_SPL_IR_H
#define SPLCOMPILER_SPL_IR_H

#include <iostream>
#include <vector>
#include "spl_compiler.hpp"
#include "spl_symtab.hpp"


class Operand {
public:
    Operand(SPL_TYPE type_, string name_) : type(type_), name(name_) {}
    SPL_TYPE type;
    std::string name;

    char    valChar;
    bool    valBool;
    int     valInt;
    double  valDouble;
    std::string* valString;
};

class Instruction {
public:
    Instruction() = default;
    Instruction(
            std::string label_,
            SPL_OP op_,
            Operand* arg1_ = nullptr,
            Operand* arg2_ = nullptr,
            Operand* res_ = nullptr)
    :label(label_), op(op_), arg1(arg1_), arg2(arg2_), res(res_)
    {}
    virtual void addVariable(std::string name) {};
    virtual void output(ostream& s) {
        s << label << "\t" << SPL_OPToString(op);
        if(arg1)
            s << "\t" << arg1->name << "[" + typeToString(arg1->type) + "]";
        if(arg2)
            s << "\t" << arg2->name << "[" + typeToString(arg2->type) + "]";
        if(res)
            s << "\t" << res->name << "[" + typeToString(res->type) + "]";
        s << "\n";
    }

    SPL_OP op;
    std::string label;
    Operand* arg1;
    Operand* arg2;
    Operand* res;

    //Position pos;

};


class PhiInstruction : public Instruction {
public:
    explicit PhiInstruction(Operand* res)
    : Instruction("", OP_PHI, nullptr, nullptr, res) {}
    std::vector<std::string> variableCluster;
    void addVariable(std::string name) override {variableCluster.push_back(name);}
    void output(ostream& s) override {
        s << label << "\t" << SPL_OPToString(op) << "\t" << res->name;
        s << "(";
        for(auto& variable : variableCluster) {
            (&variable - &variableCluster[0]) == 0 ? s << variable : s << " ," << variable;
        }
        s << ")\n";
    }

};


class SPL_IR {
public:
    explicit SPL_IR(SymbolTable* table):symbolTable(table), tempCount(0), labelCount(0){}

    void addInstruction(Instruction ins) {
        if(!ins.label.empty() && getLastInstruction()->op != OP_GOTO) {
            // need a trivial goto
            IR.push_back({"", OP_GOTO, nullptr, nullptr, new Operand(UNKNOWN, ins.label)});
        }
        IR.push_back(ins);
    }

    Operand* genTempVariable(SPL_TYPE type) {
        return new Operand(type, "_t" + std::to_string(tempCount ++));
    }

    void decreaseTempCount(Operand* name) {
        //if(name[0] == '_') tempCount -= 1;
    }

    Instruction* getLastInstruction() {
        return &IR[IR.size() - 1];
    }

    Operand* genLabel(){
        return new Operand(UNKNOWN, "L" + std::to_string(labelCount ++));
    }

    std::vector<Instruction> IR;
    SymbolTable* symbolTable;
    unsigned int tempCount;
    unsigned int labelCount;
};


#endif //SPLCOMPILER_SPL_IR_H
