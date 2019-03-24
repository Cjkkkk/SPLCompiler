/*
 * @file        spl_ast.hpp
 * @brief       Abstract syntax tree announcement.
 * @details     .
 * @author      Yin Junlin
 * @date        3/20/2019
 * @version     1.0.0
 * @par         Copyright(c): Zuiqiang Xiaozu(Best Group)
 */

#ifndef _SPL_AST_H_
#define _SPL_AST_H_

#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <vector>
#include "spl_symtab.hpp"
#include "spl_compiler.h"

#define ERROR_VAL -1

namespace SPL
{

// abstract class: base
class AST
{
  public:
    virtual int calculate() = 0;    //pure virtual function
    virtual ~AST() = 0;             //pure virtual function
    //virtual void print(void) = 0;       //pure virtual function
  protected:
    int nodeType;
};

// abstract class: ast trees that represent an expression with a value
class AST_Exp : public AST
{
  public:
    virtual int calculate() = 0;    //pure virtual function
    virtual ~AST_Exp() = 0;         //pure virtual function
    //virtual void print(void) = 0;       //pure virtual function
};

// abstract class : ast trees that represent a statement without a value
 class AST_Stmt : public AST
 {
   public:
     virtual int calculate() = 0;    //pure virtual function
     virtual ~AST_Stmt() = 0;        //pure virtual function
     //virtual void print(void) = 0;       //pure virtual function
 };

// ast node for math expression
class AST_Math : public AST_Exp
{
  public:
    AST_Math(int opType, class AST_Exp* left, class AST_Exp* right);
    ~AST_Math() override;
    int calculate() override ;
    void print();

  protected:
    /* op: operator, including:
     * + - * / % & | -(neg) !(not) 
     * < <= > >= == <> 
     */
    int opType;
    AST_Exp *left;
    AST_Exp *right;
};

// ast node for constant expression
class AST_Const : public AST_Exp
{
  public:
    explicit AST_Const(int val);
    explicit AST_Const(double val);
    explicit AST_Const(char val);
    explicit AST_Const(char* val);
    ~AST_Const() override;
    int calculate() override ;
    void print();
  protected:
    /* valType: type of the constant, including:
     * integer, real, boolean, char, string
     */
    int valType;
    void *valPtr = nullptr;
};

// ast node for symbols, including variables, function/procedure name
class AST_Sym : public AST_Exp
{
  public:
    AST_Sym(std::string& id_, SymbolTable* scope_);
    ~AST_Sym() override;
    int calculate() override ;
    //void print(void);
  protected:
    std::string id;
    SymbolTable *scope;
};

// ast node for arrray element, such as a[1], a[exp1+exp2] and so on
class AST_Array : public AST_Exp
{
  public:
   AST_Array(AST_Sym* sym_, AST_Exp* exp_);
   ~AST_Array() override;
   int calculate(void) override;
   //void print(void);
 protected:
   AST_Sym* sym;
   AST_Exp* exp;
};

// ast node for field reference in a record, such as point.x, node.next and so on
class AST_Dot : public AST_Exp
{
 public:
   AST_Dot(AST_Sym* record_, AST_Sym* field_);
   ~AST_Dot() override;
   int calculate(void) override;
   //void print(void);
 protected:
   AST_Sym* record;
   AST_Sym* field;
};

// ast node for assignment
// note that this kind of statement ifself contains a value
class AST_Assign : public AST_Exp
{
  public:
    AST_Assign(AST_Sym* sym_, AST_Exp* exp_);
    ~AST_Assign() override;
    int calculate() override ;
    void print();
  protected:
    AST_Sym *sym;
    AST_Exp *exp;
};

// ast node for if statement
// if there's no 'else' case, 'doElse' will be nullptr
class AST_If : public AST_Stmt
{
  public:
      AST_If(AST_Exp* cond_, AST_Stmt* doIf_, AST_Stmt* doElse_);
      ~AST_If() override;
      int calculate() override ;
      void print();
  protected:
      AST_Exp* cond;
      AST_Stmt* doIf;
      AST_Stmt* doElse;
};

// ast node for case statement
//class AST_Case : public AST_Stmt
//{
//
//};

} // namespace SPL
#endif