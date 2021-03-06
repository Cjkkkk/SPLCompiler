/*
 * @file        spl_ast.hpp
 * @brief       Abstract syntax tree definition
 * @details     .
 * @author      Yin Junlin
 * @date        3/20/2019
 * @version     1.0.0
 * @par         Copyright(c): Zuiqiang Xiaozu(Best Group)
 */

#ifndef _SPL_AST_H_
#define _SPL_AST_H_

#include <cstdlib>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include "spl_symtab.hpp"
#include "spl_compiler.hpp"
#include "spl_IR.hpp"

#define ERROR_VAL -1

namespace SPL
{
// abstract class: base
class AST
{
  public:
    virtual void checkSemantic() = 0;
    virtual int calculate() = 0; //pure virtual function
    virtual void emit(SPL_IR* ir) = 0; //pure virtual function
    virtual void print(std::fstream& fout) = 0;   //pure virtual function
    virtual ~AST() = 0;          //pure virtual function

    int nodeType;
};

// abstract class: ast trees that represent an expression with a value
class AST_Exp : virtual public AST
{
  public:
    virtual void checkSemantic() = 0;
    virtual int calculate() = 0; //pure virtual function
    virtual void print(std::fstream& fout) = 0; //pure virtual function
    virtual ~AST_Exp() = 0;      //pure virtual function
    virtual void emit(SPL_IR* ir) = 0;
    SPL_TYPE valType;
    Operand* tempVariable = nullptr;
    Operand* getTempVariable() {
        if(tempVariable == nullptr) throw invalid_argument{"debug info > tempvariable is null"};
        return new Operand(*tempVariable);
    }
};

// abstract class : ast trees that represent a statement without a value
class AST_Stmt : virtual public AST
{
  public:
    virtual void checkSemantic() = 0;
    virtual int calculate() = 0; //pure virtual function
    virtual void print(std::fstream& fout) = 0; //pure virtual function
    virtual void emit(SPL_IR* ir) = 0;
    virtual ~AST_Stmt() = 0;     //pure virtual function
                                 //virtual void print(void) = 0;       //pure virtual function
};

// ast node for math expression
class AST_Math : public AST_Exp
{
  public:
    AST_Math(SPL_OP opType, class AST_Exp *left, class AST_Exp *right);
    ~AST_Math() override;
    int calculate() override;
    void checkSemantic() override;
    void emit(SPL_IR* ir) override;
    void print(std::fstream& fout) override;
  protected:
    /* op: operator, including:
     * + - * / % & | -(neg) !(not) 
     * < <= > >= == <> 
     */
    SPL_OP opType;
    AST_Exp *left;
    AST_Exp *right;
};

union valueUnion{
  char    valChar;
  bool    valBool;
  int     valInt;
  double  valDouble;
  std::string* valString;
  valueUnion(){}
  ~valueUnion(){}
};

// ast node for constant expression
class AST_Const : public AST_Exp
{
  public:
    explicit AST_Const(int val);
    explicit AST_Const(double val);
    explicit AST_Const(char val);
    explicit AST_Const(bool val);
    explicit AST_Const(std::string &val);
    ~AST_Const() override;
    int calculate() override;
    void print(std::fstream& fout) override;
    void checkSemantic() override;
    void emit(SPL_IR* ir) override;
    valueUnion getValue() {
        return value;
    }

    /* valType: type of the constant, including:
     * integer, real, boolean, char, string
     */
  protected:
    valueUnion value;
};

// ast node for symbols, including variables, function/procedure name
class AST_Sym : public AST_Exp
{
  public:
    AST_Sym(std::string &id_, unsigned int scopeIndex_, Symbol*);
    ~AST_Sym() override;
    int calculate() override;
    void print(std::fstream& fout) override;
    void checkSemantic() override;
    void emit(SPL_IR* ir) override;
    Symbol* get_symbol() {
        if(!symbol) throw invalid_argument{"access void pointer in symbol: " + id};
        return symbol;
    }

    std::string id;
    unsigned int scopeIndex;
    Symbol* symbol;
};

// ast node for arrray element, such as a[1], a[exp1+exp2] and so on
class AST_Array : public AST_Exp
{
  public:
    AST_Array(AST_Sym *sym_, AST_Exp *exp_);
    ~AST_Array() override;
    int calculate(void) override;
    void print(std::fstream& fout) override;
    void checkSemantic() override;
    void emit(SPL_IR* ir) override;
    Symbol* get_symbol() {
        if(!symbol) throw invalid_argument{"access void pointer in symbol: " + sym->id};
        return symbol;
    }
    //void print(void);
    Symbol* symbol;
    AST_Sym *sym;
    AST_Exp *exp;
};

// ast node for field reference in a record, such as point.x, node.next and so on
class AST_Dot : public AST_Exp
{
  public:
    AST_Dot(AST_Sym *record_, AST_Sym *field_);
    ~AST_Dot() override;
    int calculate(void) override;
    void print(std::fstream& fout) override;
    void checkSemantic() override;
    void emit(SPL_IR* ir) override;
    //void print(void);
  protected:
    AST_Sym *record;
    AST_Sym *field;
};

// ast node for assignment
// note that this kind of statement ifself contains a value
class AST_Assign : public AST_Exp, public AST_Stmt
{
  public:
    AST_Assign(AST_Exp *lhs_, AST_Exp *rhs_);
    ~AST_Assign() override;
    int calculate() override;
    void print(std::fstream& fout) override;
    void checkSemantic() override;
    void emit(SPL_IR* ir) override;
    void print();

  protected:
    AST_Exp *lhs;
    AST_Exp *rhs;
};

// ast node for if statement and case statement
// if there's no 'else' case, 'doElse' will be nullptr
class AST_If : public AST_Stmt
{
  public:
    AST_If(AST_Exp *cond_, AST_Stmt *doIf_, AST_Stmt *doElse_);
    ~AST_If() override;
    int calculate() override;
    void print(std::fstream& fout) override;
    void addRight(AST_Stmt *doElse_);
    AST_Stmt *getDoElse(void);
    void checkSemantic() override;
    void emit(SPL_IR* ir) override;
    void print();

  protected:
    AST_Exp *cond;
    AST_Stmt *doIf;
    AST_Stmt *doElse;
};

// auxiliary structure for case_expr
class caseUnit
{
  public:
    AST_Exp *val;
    AST_Stmt *stmt;
    inline caseUnit(AST_Exp *val_, AST_Stmt *stmt_) : val(val_), stmt(stmt_) {}
};

class AST_While : public AST_Stmt
{
  public:
    AST_While(AST_Exp *cond_, AST_Stmt *stmt_);
    ~AST_While() override;
    int calculate() override;
    void print(std::fstream& fout) override;
    void checkSemantic() override;
    void emit(SPL_IR* ir) override;

  protected:
    AST_Exp *cond;
    AST_Stmt *stmt;
};

class AST_Repeat : public AST_Stmt
{
  public:
    AST_Repeat(std::vector<AST_Stmt *> *stmtList_, AST_Exp *exp_);
    ~AST_Repeat() override;
    int calculate() override;
    void print(std::fstream& fout) override;
    void checkSemantic() override;
    void emit(SPL_IR* ir) override;

  protected:
    std::vector<AST_Stmt *> *stmtList;
    AST_Exp *exp;
};

class AST_For : public AST_Stmt
{
  public:
    AST_For(AST_Assign *init_, bool dir_, AST_Exp *fin_, AST_Stmt *stmt_);
    ~AST_For() override;
    int calculate() override;
    void print(std::fstream& fout) override;
    void checkSemantic() override;
    void emit(SPL_IR* ir) override;

  protected:
    AST_Assign *init;
    bool dir;
    AST_Exp *fin;
    AST_Stmt *stmt;
};

class AST_Goto : public AST_Stmt
{
  public:
    AST_Goto(int label_);
    ~AST_Goto() override;
    int calculate() override;
    void print(std::fstream& fout) override;
    void checkSemantic() override;
    void emit(SPL_IR* ir) override;

  protected:
    int label;
};

class AST_Compound : public AST_Stmt
{
  public:
    AST_Compound(std::vector<AST_Stmt *> *stmtList_);
    ~AST_Compound() override;
    int calculate() override;
    void print(std::fstream& fout) override;
    void checkSemantic() override;
    void emit(SPL_IR* ir) override;

  protected:
    std::vector<AST_Stmt *> *stmtList;
};

class AST_Func : public AST_Exp, public AST_Stmt
{
  public:
    AST_Func(bool isProc_, std::string &funcId_, std::vector<AST_Exp *> *argList_, unsigned int scopeIndex, Symbol*);
    AST_Func(int sysFuncId_, std::vector<AST_Exp *> *argList_);
    ~AST_Func() override;
    int calculate() override;
    void print(std::fstream& fout) override;
    void checkSemantic() override;
    void emit(SPL_IR* ir) override;
    Symbol* get_symbol() {
        if(!symbol) throw invalid_argument{"access void pointer in function: " + funcId};
        return symbol;
    }
    bool isProc;
    bool isSys;
    std::string funcId;
    std::vector<AST_Exp *> *argList;
    unsigned int scopeIndex;
    Symbol* symbol;
};

class AST_Manager
{
  private:

  public:
    std::vector<AST*> *functions = nullptr;
    std::vector<unsigned int> *scopes = nullptr;
    std::vector<unsigned int> *defined_scopes = nullptr;
    //std::vector<std::string> *name = nullptr;
    AST_Manager(){
      functions = new std::vector<AST*>();
      scopes = new std::vector<unsigned int>();
      defined_scopes = new std::vector<unsigned int>();
      //name = new std::vector<std::string>();
//      functions->push_back({nullptr, nullptr});  //reserve for main()
    }
    void addFunc(AST* func, unsigned int scope, unsigned int preScope){
      functions->emplace_back(func);
      scopes->emplace_back(scope);
      defined_scopes->emplace_back(preScope);
    }
    void printAST(std::fstream& fout){
      for(std::vector<AST*>::iterator it = functions->begin(); it != functions->end(); ++it){
        AST* ast = *it;
        ast->print(fout);
        fout << std::endl;
      }
    }
//    void addMain(AST* func, string* id, SymbolTable* scope){
//      functions->at(0) = {func,id};
//    }
    ~AST_Manager(){}
};

} // namespace SPL
#endif