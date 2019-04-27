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
    Operand(SPL_TYPE type_, string name_, SPL_CLASS cl_) : type(type_), name(name_), cl(cl_) {}
    SPL_TYPE type;
    SPL_CLASS cl;
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
    virtual void outputOperand(Operand* operand, ostream& s) {
        if(operand == nullptr) return;
        else if(operand->cl == LABEL) {
            s << "\t" << operand->name;
        } else if(operand->cl == CONST) {
            switch (operand->type) {
                case INT:
                    s << "\t" << std::to_string(operand->valInt);
                    return;
                case CHAR:
                    s << "\t" << "'" + std::to_string(operand->valChar) + "'";
                    return;
                case REAL:
                    s << "\t" << std::to_string(operand->valDouble);
                    return;
                case BOOL:
                    if(operand->valBool) s << "\t" << "true";
                    else s << "\t" << "false";
                    return;
                case STRING:
                    s << "\t" << "\"" + *operand->valString + "\"";
                    return;
                default:
                    s << "\t" << "ERROR";
            }
        }else {
            s << "\t" << operand->name << "[" << typeToString(operand->type) << "]";
        }
    }
    virtual void output(ostream& s) {
        s << label << "\t" << SPL_OPToString(op);
        outputOperand(arg1, s);
        outputOperand(arg2, s);
        outputOperand(res, s);
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
            IR.push_back({"", OP_GOTO, nullptr, nullptr, new Operand(UNKNOWN, ins.label, LABEL)});
        }
        IR.push_back(ins);
    }

    Operand* genTempVariable(SPL_TYPE type) {
        return new Operand(type, "_t" + std::to_string(tempCount ++), TEMP);
    }

    void decreaseTempCount(Operand* name) {
        //if(name[0] == '_') tempCount -= 1;
    }

    Instruction* getLastInstruction() {
        return &IR[IR.size() - 1];
    }

    Operand* genLabel(){
        return new Operand(UNKNOWN, "L" + std::to_string(labelCount ++), LABEL);
    }

    std::vector<Instruction> IR;
    SymbolTable* symbolTable;
    unsigned int tempCount;
    unsigned int labelCount;
};


#endif //SPLCOMPILER_SPL_IR_H
