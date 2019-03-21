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
    int plus = (this->left->calculate()) + (this->right->calculate());
    int minus = (this->left->calculate()) - (this->right->calculate());
    int mul = (this->left->calculate()) * (this->right->calculate());
    int div = (this->left->calculate()) / (this->right->calculate());
    switch(opType){
    case PLUS_:  return plus;
    case MINUS_: return minus;
    case MUL_:   return mul;
    case DIV_:   return div;
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
    this->valPtr = malloc(sizeof(int));
    memcpy(this->valPtr, &val, sizeof(int));
}

AST_Const::AST_Const(const double val){
    this->valType = REAL_;
    this->valPtr = malloc(sizeof(double));
    memcpy(this->valPtr, &val, sizeof(double));
}

AST_Const::AST_Const(const char val){
    this->valType = CHAR_;
    this->valPtr = malloc(sizeof(char));
    memcpy(this->valPtr, &val, sizeof(char));
}

AST_Const::AST_Const(const char* val){
    int size = strlen(val) + 1;
    this->valType = STRING_;
    this->valPtr = malloc(sizeof(char) * size);
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