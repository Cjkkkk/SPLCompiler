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
    Operand(SPL_TYPE type_, string name_, SPL_CLASS cl_, Symbol* symbol_= nullptr) : type(type_), name(name_), cl(cl_), symbol(symbol_) {}
    Operand(const Operand& op) {
        this->type = op.type;
        this->name = op.name;
        this->cl = op.cl;
        this->value = op.value;
        this->symbol = op.symbol;
    }
    x86_size getSize();
    SPL_TYPE type;
    std::string name;
    SPL_CLASS cl;
    Value value;
    Symbol* symbol;
};

Operand* evaluate(SPL_OP op, Operand* left, Operand* right);
bool compareValue(Operand* l, Operand* r);

inline bool checkOperandClass(Operand* operand, SPL_CLASS cl) {
    return operand->cl == cl;
}

inline bool checkOperandIsLiteral(Operand* operand) {
    return operand->cl == KNOWN;
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
    explicit SPL_IR(SymbolTable* table):symbolTable(table), tempCount(0), labelCount(0), idCount(0), current(0){}
    SPL_IR():symbolTable(nullptr), tempCount(0), labelCount(0), idCount(0), current(0){}

    // 设置当前中间代码的存放位置
    void setCurrent(unsigned int index);

    void setToMain();

    // 添加一条中间代码
    void addInstruction(Instruction* ins);

    // 回填技术
    void backFill(Operand* operand, int index);

    // 一些访问私有变量的函数
    vector<Instruction*>& getCurrentIR();

    vector<vector<Instruction*>>& getIRSet();

    Instruction* getLastInstruction() ;

    void outputInstruction(ostream& f);
    // 生成临时变量
    Operand* genTempVariable(SPL_TYPE type);

    // 生成标签
    Operand* genLabel();

    //为每一条指令生成唯一id用于指令的对比
    unsigned int getIdCount();

    // 获取有多少个IRSet的大小
    int getIRSetSize();

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
