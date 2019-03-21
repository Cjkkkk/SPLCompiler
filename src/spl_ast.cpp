/*
 * @file        spl_ast.cpp
 * @brief       Abstract syntax tree defination.
 * @details     .
 * @author      Yin Junlin
 * @date        3/21/2019
 * @version     1.0.0
 * @par         Copyright(c): Zuiqiang Xiaozu(Best Group)
 */

#include "spl_ast.hpp"
using namespace SPL;

// ast node for math expression
AST_Math::AST_Math(int opType, class AST_Exp* left, class AST_Exp* right){
      this->opType = opType;
      this->left = left;
      this->right = right;
}

int AST_Math::calculate(void){
    switch(opType){
    case PLUS_:  return left->calculate() + right->calculate();
    case MINUS_: return left->calculate() - right->calculate();
    case MUL_:   return left->calculate() * right->calculate();
    case DIV_:   return left->calculate() / right->calculate();
    case AND_:
        if(left->calculate() != 0 && right->calculate() != 0) return 1;
        return 0;
    case OR_ :
        if(left->calculate() == 0 && right->calculate() == 0) return 0;
        return 1;
    case GT_ :
        return left->calculate() > right->calculate();
    case GE_:
        return left->calculate() >= right->calculate();
    case LT_ :
        return left->calculate() < right->calculate();
    case LE_:
        return left->calculate() <= right->calculate();
    case NOT_:
        return left->calculate() == 0 ? 1 : 0;
    case MINUS__:
        return - left->calculate();
    default:     return -ERROR_VAL;
    }
}

void AST_Math::print(void){
    return;
}

/****************************************************************/

// ast node for constant expression
AST_Const::AST_Const(const int val){
    this->valType = INT_;
    this->valPtr = new int();
    *((int*)this->valPtr) = val;
}

AST_Const::AST_Const(const double val){
    this->valType = REAL_;
    this->valPtr = new double();
    *((double*)this->valPtr) = val;
}

AST_Const::AST_Const(const char val){
    this->valType = CHAR_;
    this->valPtr = new char();
    *((char*)this->valPtr) = val;
}

AST_Const::AST_Const(const char* val){
    int size = strlen(val) + 1;
    this->valType = STRING_;
    this->valPtr = new char(size);
    memcpy(this->valPtr, val, sizeof(char) * size);
}

int AST_Const::calculate(void){
    if(valType == INT_)
    return *(int*)valPtr;
    return 0;
}

void AST_Const::print(void){
    return;
}

AST_Sym::AST_Sym(std::string id_, class SymbolTable *scope_) :id(id_), scope(scope_){};
int AST_Sym::calculate() {
    return 1;
    // add search symbol table
}

AST_Assign::AST_Assign(SPL::AST_Sym *sym_, SPL::AST_Exp *exp_) :sym(sym_), exp(exp_){};
int AST_Assign::calculate() {
    return exp->calculate();
}