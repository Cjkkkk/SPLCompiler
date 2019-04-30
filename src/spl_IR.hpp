//
// Created by DELL on 2019/4/21.
//

#ifndef SPLCOMPILER_SPL_IR_H
#define SPLCOMPILER_SPL_IR_H

#include <iostream>
#include <vector>
#include "spl_compiler.hpp"
#include "spl_symtab.hpp"

union Value {
    char    valChar;
    bool    valBool;
    int     valInt;
    double  valDouble;
    std::string* valString;
};

class Operand {
public:
    Operand(SPL_TYPE type_, string name_, SPL_CLASS cl_) : type(type_), name(name_), cl(cl_) {}
    SPL_TYPE type;
    std::string name;
    SPL_CLASS cl;
    Value value;

    void evalute(SPL_OP op, Operand* left, Operand* right);
};


inline bool checkOperandClass(Operand* operand, SPL_CLASS cl) {
    return operand->cl == cl;
}

inline bool checkOperandType(Operand* operand, SPL_TYPE type) {
    return operand->type == type;
}



class Instruction {
public:
    Instruction() {};
    Instruction(std::string label_,
            SPL_OP op_,
            Operand* arg1_ = nullptr,
            Operand* arg2_ = nullptr,
            Operand* res_ = nullptr)
    :label(label_), op(op_), arg1(arg1_), arg2(arg2_), res(res_) {}

    virtual std::vector<std::string>* getVariable();
    virtual void addVariable(std::string name);
    virtual void outputOperand(Operand* operand, ostream& s);
    virtual void output(ostream& s);

    std::string label;
    SPL_OP op;
    Operand* arg1;
    Operand* arg2;
    Operand* res;

    //Position pos;

};


class PhiInstruction : public Instruction {

public:

    explicit PhiInstruction(Operand* res) : Instruction("", OP_PHI, nullptr, nullptr, res) {}

    std::vector<std::string>* getVariable() override;
    void addVariable(std::string name) override;
    void output(ostream& s) override;

    std::vector<std::string> variableCluster;

};


class SPL_IR {
public:
    SPL_IR(SymbolTable* table):symbolTable(table), tempCount(0), labelCount(0){}
    SPL_IR():symbolTable(nullptr), tempCount(0), labelCount(0){}

    void addInstruction(Instruction ins);

    Operand* genTempVariable(SPL_TYPE type) ;

    void decreaseTempCount(Operand* name);

    Instruction* getLastInstruction() ;

    Operand* genLabel();

    std::vector<Instruction> IR;
    SymbolTable* symbolTable;
    unsigned int tempCount;
    unsigned int labelCount;
};


#endif //SPLCOMPILER_SPL_IR_H
