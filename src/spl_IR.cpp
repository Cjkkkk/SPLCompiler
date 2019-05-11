//
// Created by DELL on 2019/4/21.
//

#include "spl_IR.hpp"

bool compareValue(Operand* l, Operand* r) {
    if(checkOperandType(l, INT)) {
        return l->value.valInt == r->value.valInt;
    }
    if(checkOperandType(l, REAL)) {
        return l->value.valDouble == r->value.valDouble;
    }
    if(checkOperandType(l, CHAR)) {
        return l->value.valChar == r->value.valChar;
    }
    if(checkOperandType(l, BOOL)) {
        return l->value.valBool == r->value.valBool;
    }
    if(checkOperandType(l, STRING)) {
        return *l->value.valString == *r->value.valString;
    }
    return false;
}


Operand* evalute(SPL_OP op, Operand* left, Operand* right) {
//    name.clear();
    auto new_operand = new Operand();
    new_operand->cl = CONST;
    new_operand->name = "";
    switch(op) {
        case PLUS_:
            new_operand->value.valInt = left->value.valInt + right->value.valInt;
            new_operand->type = INT;
            
            break;
        case MINUS_:
            new_operand->value.valInt = left->value.valInt - right->value.valInt;
            new_operand->type = INT;
            
            break;
        case MUL_:
            new_operand->value.valInt = left->value.valInt * right->value.valInt;
            new_operand->type = INT;
            
            break;
        case DIV_:
            new_operand->value.valInt = left->value.valInt / right->value.valInt;
            new_operand->type = INT;
            break;
        case MOD_:
            new_operand->value.valInt = left->value.valInt % right->value.valInt;
            new_operand->type = INT;
            
            break;
        case MINUS__:
            new_operand->value.valInt = - left->value.valInt;
            new_operand->type = INT;
            
            break;
        case AND_:
            new_operand->value.valBool = left->value.valBool && right->value.valBool;
            new_operand->type = BOOL;
            
            break;
        case OR_:
            new_operand->value.valBool = left->value.valBool || right->value.valBool;
            new_operand->type = BOOL;
            
            break;
        case NOT_:
            new_operand->value.valBool = !left->value.valBool;
            new_operand->type = BOOL;
            
            break;
        case EQUAL_:
            new_operand->value.valBool = left->value.valInt == right->value.valInt;
            new_operand->type = BOOL;
            
            break;
        case UNEQUAL_:
            new_operand->value.valBool = left->value.valInt != right->value.valInt;
            new_operand->type = BOOL;
            
            break;
        case GT_:
            new_operand->value.valBool = left->value.valInt > right->value.valInt;
            new_operand->type = BOOL;
            
            break;
        case GE_:
            new_operand->value.valBool = left->value.valInt >= right->value.valInt;
            new_operand->type = BOOL;
            break;
        case LT_:
            new_operand->value.valBool = left->value.valInt < right->value.valInt;
            new_operand->type = BOOL;
            
            break;
        case LE_:
            new_operand->value.valBool = left->value.valInt <= right->value.valInt;
            new_operand->type = BOOL;
            break;
        default:
            break;
    }
    return new_operand;
}
void Instruction::addVariable(Operand* name, int index) {};
std::list<pair<Operand*, int>>* Instruction::getVariable() {
    return nullptr;
}
void Instruction::outputOperand(Operand* operand, ostream& s) {
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



void Instruction::output(ostream& s) {
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
    s << "\n";
}




void PhiInstruction::addVariable(Operand* name, int index) {variableCluster.push_back({name, index});}
void PhiInstruction::output(ostream& s) {
    if(op == OP_ASSIGN) {
        Instruction::output(s);
        return;
    }
    s << label << "\t" << SPL_OPToString(op) << "\t" << res->name;
    s << "(";
    for(auto it = variableCluster.begin() ; it != variableCluster.end() ; it++) {
        if(std::distance(variableCluster.begin(), it) > 0) s << ", ";
        outputOperand((*it).first, s);
    }
    s << ")\n";
}

std::list<pair<Operand*, int>>* PhiInstruction::getVariable() {return &variableCluster;}


void SPL_IR::addInstruction(Instruction* ins) {
    if(!ins->label.empty() && !getCurrentIR().empty()
    && getLastInstruction()->op != OP_GOTO && getLastInstruction()->op != OP_RET) {
        // need a trivial goto
        auto go = new Instruction{"", OP_GOTO, nullptr, nullptr, new Operand(UNKNOWN, ins->label, LABEL)};
        go->unique_id = idCount++;
        getCurrentIR().push_back(go);
    }
    ins->unique_id = idCount++;
    getCurrentIR().push_back(ins);
}



Operand* SPL_IR::genTempVariable(SPL_TYPE type) {
    return new Operand(type, "_t" + std::to_string(tempCount ++), TEMP);
}


Operand* SPL_IR::genLabel(){
    return new Operand(UNKNOWN, "L" + std::to_string(labelCount ++), LABEL);
}

unsigned int SPL_IR::getIdCount() {
    return idCount ++ ;
}

void SPL_IR::decreaseTempCount(Operand* name) {
    //if(name[0] == '_') tempCount -= 1;
}

Instruction* SPL_IR::getLastInstruction() {
    return getCurrentIR().back();
}

vector<Instruction*>& SPL_IR::getCurrentIR() {
    return IR[current];
}

vector<vector<Instruction*>>& SPL_IR::getIRSet() {
    return IR;
}

void SPL_IR::backFill(Operand* operand, int index) {
    IR[current][index]->res = operand;
}

void SPL_IR::setCurrent(unsigned int index) {
    current = index;
}

int SPL_IR::getIRSetSize() {
    return IR.size();
}