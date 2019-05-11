//
// Created by DELL on 2019/4/21.
//

#ifndef SPLCOMPILER_SPL_IR_H
#define SPLCOMPILER_SPL_IR_H

#include <iostream>
#include <vector>
#include <list>
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
    Operand(){};
    Operand(SPL_TYPE type_, string name_, SPL_CLASS cl_) : type(type_), name(name_), cl(cl_) {}
    Operand(const Operand& op) {
        this->type = op.type;
        this->name = op.name;
        this->cl = op.cl;
        this->value = op.value;
    }
    SPL_TYPE type;
    std::string name;
    SPL_CLASS cl;
    Value value;
};

Operand* evalute(SPL_OP op, Operand* left, Operand* right);
bool compareValue(Operand* l, Operand* r);

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
    :label(label_), op(op_), arg1(arg1_), arg2(arg2_), res(res_), unique_id(0) {}

    virtual std::list<pair<Operand*, int>>* getVariable();
    virtual void addVariable(Operand* name, int index);
    virtual void outputOperand(Operand* operand, ostream& s);
    virtual void output(ostream& s);

    std::string label;
    SPL_OP op;
    Operand* arg1;
    Operand* arg2;
    Operand* res;

    unsigned int unique_id;
    //Position pos;

};

inline bool checkInstructionOp(Instruction* ins, SPL_OP op) {
    return ins->op == op;
}

class PhiInstruction : public Instruction {

public:

    explicit PhiInstruction(Operand* res) : Instruction("", OP_PHI, nullptr, nullptr, res) {}

    std::list<pair<Operand*, int>>* getVariable() override;
    void addVariable(Operand* name, int index) override;
    void output(ostream& s) override;

    std::list<pair<Operand*, int>> variableCluster;

};


class SPL_IR {
public:
    SPL_IR(SymbolTable* table):symbolTable(table), tempCount(0), labelCount(0), idCount(0), current(0){}
    SPL_IR():symbolTable(nullptr), tempCount(0), labelCount(0){}


    void setCurrent(int index);
    void addInstruction(Instruction* ins);
    void backFill(Operand* operand, int index);
    vector<Instruction*>& getCurrentIR();
    vector<vector<Instruction*>>& getIRSet();
    Instruction* getLastInstruction() ;

    Operand* genTempVariable(SPL_TYPE type);
    Operand* genLabel();
    unsigned int getIdCount();

    int getIRSize();

    void decreaseTempCount(Operand* name);
    SymbolTable* symbolTable;

private:
    std::vector<vector<Instruction*>> IR;
    unsigned int tempCount;
    unsigned int labelCount;
    unsigned int idCount;
    unsigned int current;
};


#endif //SPLCOMPILER_SPL_IR_H
