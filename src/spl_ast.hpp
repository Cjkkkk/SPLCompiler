/*
 * @file        spl_ast.hpp
 * @brief       Abstract syntax tree announcement.
 * @details     .
 * @author      Yin Junlin
 * @date        3/20/2019
 * @version     1.0.0
 * @par         Copyright(c): Zuiqiang Xiaozu(Best Group)
 */

#ifndef __SPLAST_H_
#define __SPLAST_H_

#include <stdlib.h>
#include <string.h>
#include <iostream>
//#include "symtab.hpp"
#include "parse.h"

#define ERROR_VAL  -1

namespace SPL 
{

// abstract class: base
class AST
{
  public:
    virtual int calculate(void) = 0;    //pure virtual function
    //virtual void print(void) = 0;       //pure virtual function
    //virtual void emit(void) = 0;
  protected:
    int nodeType;
};

// abstract class: ast trees that represent an expression with a value
class AST_Exp : public AST
{
  public:
    virtual int calculate(void) = 0;    //pure virtual function
    //virtual void print(void) = 0;       //pure virtual function
};

// abstract class : ast trees that represent a statement without a value
// class AST_Stmt : public AST
// {
//   public:
//     virtual int calculate(void) = 0;    //pure virtual function
//     //virtual void print(void) = 0;       //pure virtual function
// };

// ast node for math expression
class AST_Math : public AST_Exp
{
  public:
    AST_Math(int opType, class AST_Exp* left, class AST_Exp* right);
    int calculate(void);
    void print(void);

  protected:
    /* op: operator, including:
     * + - * / % & | -(neg) !(not) 
     * < <= > >= == <> 
     */
    int opType;
    AST_Exp* left;
    AST_Exp* right;
};

// ast node for constant expression
class AST_Const : public AST_Exp
{
  public:
    AST_Const(const int val);
    AST_Const(const double val);
    AST_Const(const char val);
    AST_Const(const char* val);
    int calculate(void);
    void print(void);
  protected:
    /* valType: type of the constant, including:
     * integer, real, boolean, char, string
     */
    int valType;
    void* valPtr = nullptr;
};

/*
// ast node for symbols, including variables, function/procedure name 
class AST_Sym : public AST_Exp
{
  public:
    AST_Sym(string id, class SymbolTable* scope);
    int calculate();
    void print(void);
  protected:
    string id;
    SymbolTable* scope;
};

// ast node for arrray element, such as a[1], a[exp1+exp2] and so on
class AST_Array : public AST_Exp
{
   public:
    AST_Array(class AST_Sym* sym, class AST_Exp* exp);
    int calculate(void);
    void print(void);
  protected:
    AST_Sym* sym;
    AST_Exp* exp;
};

// ast node for field reference in a record, such as point.x, node.next and so on
class AST_Dot : public AST_Exp
{
  public:
    AST_Dot(class AST_Sym* record, class AST_Sym* field);
    int calculate(void);
    void print(void);
  protected:
    AST_Sym* record;
    AST_Sym* field;
};

// ast node for assignment
// note that this kind of statement ifself contains a value
class AST_Assign : public AST_Exp
{
  public:
    AST_Assign(class AST_Sym* sym, class AST_Exp* exp);
    int calculate(void);
    void print(void);
  protected:
    AST_Sym* sym;
    AST_Exp* exp;
};

// ast node for if statement
// if there's no 'else' case, 'doElse' will be nullptr
class AST_If : public AST_Stmt
{
  public:
    AST_If(class AST_Exp* cond, class AST_Stmt* doIf, class AST_Stmt* doElse);
    int calculate(void);
    void print(void);
  protected:
    AST_Exp* cond;
    AST_Stmt* doIf;
    AST_Stmt* doElse;
};

// ast node for case statement
class AST_Case : public AST_Stmt
{

};


*/

}
#endif