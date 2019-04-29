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
    SPL_CLASS cl;
    std::string name;
    Value value;
};

class Instruction {
public:
    Instruction() {};
    Instruction(std::string label_,
            SPL_OP op_,
            Operand* arg1_ = nullptr,
            Operand* arg2_ = nullptr,
            Operand* res_ = nullptr)
    :label(label_), op(op_), arg1(arg1_), arg2(arg2_), res(res_) {}

    virtual void addVariable(std::string name) {};
    virtual std::vector<std::string>* getVariable() {
        return nullptr;
    }
    virtual void outputOperand(Operand* operand, ostream& s) {
        if(operand == nullptr) return;
        else if(operand->cl == LABEL) {
            s << operand->name;
        } else if(operand->cl == CONST) {
            switch (operand->type) {
                case INT:
                    s << std::to_string(operand->value.valInt);
                    return;
                case CHAR:
                    s << "'" + std::to_string(operand->value.valChar) + "'";
                    return;
                case REAL:
                    s << std::to_string(operand->value.valDouble);
                    return;
                case BOOL:
                    if(operand->value.valBool) s << "true";
                    else s << "false";
                    return;
                case STRING:
                    s << "\"" + *operand->value.valString + "\"";
                    return;
                default:
                    s << "ERROR";
            }
        }else {
//            s << operand->name << "[" << typeToString(operand->type) << "]";
              s << operand->name;
        }
    }
    virtual void output(ostream& s) {
        switch(op) {
            case PLUS_:
            case MINUS_:
            case MUL_:
            case DIV_:
            case MOD_:
            case EQUAL_:
            case UNEQUAL_:
            case GE_:
            case GT_:
            case LE_:
            case LT_:
            case AND_:
            case OR_:
                s << label << "\t";
                outputOperand(res, s);
                s << " = ";
                outputOperand(arg1, s);
                s << " " << SPL_OPToString(op) << " ";
                outputOperand(arg2, s);
                break;

                // 2 operand
            case OP_ASSIGN:
                s << label << "\t";
                outputOperand(res, s);
                s << " = ";
                outputOperand(arg1, s);
                break;
            case OP_IF:
            case OP_IF_Z:
                s << label << "\t";
                s << SPL_OPToString(op) << " ";
                outputOperand(arg1, s);
                s << " goto ";
                outputOperand(res, s);
                break;
            case NOT_:
            case MINUS__:
                s << label << "\t";
                outputOperand(res, s);
                s << " = " << SPL_OPToString(op) << " ";
                outputOperand(arg1, s);
                break;
            // 1 operand
            case OP_GOTO:
                s << label << "\t";
                s << SPL_OPToString(op) << " ";
                outputOperand(res, s);
                break;
            case OP_PARAM:
            case OP_POP:
            case OP_CALL:
                s << label << "\t";
                s << SPL_OPToString(op) << " ";
                outputOperand(arg1, s);
                break;
            // 0 operand
            default:
                s << label << "\t";
                s << SPL_OPToString(op) << " ";
        }
//        s << label << "\t" << SPL_OPToString(op);
//        outputOperand(arg1, s);
//        outputOperand(arg2, s);
//        outputOperand(res, s);
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
    std::vector<std::string>* getVariable() override {return &variableCluster;}
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
    SPL_IR(SymbolTable* table):symbolTable(table), tempCount(0), labelCount(0){}
    SPL_IR():symbolTable(nullptr), tempCount(0), labelCount(0){}
    void addInstruction(Instruction ins) {
        if(!ins.label.empty() && IR.size() > 0 && getLastInstruction()->op != OP_GOTO) {
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
        return &IR.back();
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
