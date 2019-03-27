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

AST::~AST() {}

AST_Exp::~AST_Exp() {}

AST_Stmt::~AST_Stmt() {}

// ast node for math expression
AST_Math::AST_Math(int opType, AST_Exp *left, AST_Exp *right)
{
    this->opType = opType;
    this->left = left;
    this->right = right;
}

AST_Math::~AST_Math() {}

int AST_Math::calculate()
{
    switch (opType)
    {
    case PLUS_:
        return left->calculate() + right->calculate();
    case MINUS_:
        return left->calculate() - right->calculate();
    case MUL_:
        return left->calculate() * right->calculate();
    case DIV_:
        return left->calculate() / right->calculate();
    case AND_:
        if (left->calculate() != 0 && right->calculate() != 0)
            return 1;
        return 0;
    case OR_:
        if (left->calculate() == 0 && right->calculate() == 0)
            return 0;
        return 1;
    case GT_:
        return left->calculate() > right->calculate();
    case GE_:
        return left->calculate() >= right->calculate();
    case LT_:
        return left->calculate() < right->calculate();
    case LE_:
        return left->calculate() <= right->calculate();
    case NOT_:
        return left->calculate() == 0 ? 1 : 0;
    case MINUS__:
        return -left->calculate();
    default:
        return -ERROR_VAL;
    }
}

// ast node for constant expression
AST_Const::AST_Const(int val)
{
    this->valType = INT;
    this->valPtr = new int();
    *((int *)this->valPtr) = val;
}

AST_Const::AST_Const(double val)
{
    this->valType = REAL;
    this->valPtr = new double();
    *((double *)this->valPtr) = val;
}

AST_Const::AST_Const(char val)
{
    this->valType = CHAR;
    this->valPtr = new char();
    *((char *)this->valPtr) = val;
}

AST_Const::AST_Const(bool val)
{
    this->valType = CHAR;
    this->valPtr = new bool();
    *((bool *)this->valPtr) = val;
}

AST_Const::AST_Const(std::basic_string<char> &val)
{
    this->valType = STRING;
    this->valPtr = new std::basic_string<char>();
    *((std::basic_string<char> *)this->valPtr) = val;
}

int AST_Const::calculate()
{
    if (valType == INT)
        return *(int *)valPtr;
    return 0;
}

AST_Const::~AST_Const() {}

AST_Sym::AST_Sym(std::string &id_, SymbolTable *scope_) : id(id_), scope(scope_) {}
int AST_Sym::calculate()
{
    return 1;
    // add search symbol table
}

AST_Sym::~AST_Sym() {}

AST_Array::AST_Array(AST_Sym *sym_, AST_Exp *exp_) : sym(sym_), exp(exp_) {}

AST_Array::~AST_Array() {}

int AST_Array::calculate()
{
    return ERROR_VAL;
}

AST_Dot::AST_Dot(AST_Sym *record_, AST_Sym *field_) : record(record_), field(field_) {}

AST_Dot::~AST_Dot() {}

int AST_Dot::calculate()
{
    return ERROR_VAL;
}

AST_Assign::AST_Assign(SPL::AST_Exp *lhs_, SPL::AST_Exp *rhs_) : lhs(lhs_), rhs(rhs_) {}

int AST_Assign::calculate()
{
    return rhs->calculate();
}

AST_Assign::~AST_Assign() {}

AST_If::AST_If(SPL::AST_Exp *cond_, SPL::AST_Stmt *doIf_, SPL::AST_Stmt *doElse_)
    : cond(cond_), doIf(doIf_), doElse(doElse_){};

AST_If::~AST_If() {}

int AST_If::calculate()
{
    if (cond->calculate() != 0)
    {
        std::cout << "if statement executed!";
    }
    else
    {
        if (doElse != nullptr)
        {
            std::cout << "else statement executed!";
        }
    }
    return 1;
}

void AST_If::addRight(AST_Stmt *doElse_)
{
    this->doElse = doElse_;
}

AST_Stmt *AST_If::getDoElse(void)
{
    return this->doElse;
}

AST_While::AST_While(AST_Exp *cond_, AST_Stmt *stmt_) : cond(cond_), stmt(stmt_) {}

AST_While::~AST_While() {}

int AST_While::calculate()
{
    return ERROR_VAL;
}

AST_Repeat::AST_Repeat(std::vector<AST_Stmt *> *stmtList_, AST_Exp *exp_) : stmtList(stmtList_), exp(exp_) {}

AST_Repeat::~AST_Repeat() {}

int AST_Repeat::calculate()
{
    return ERROR_VAL;
}

AST_For::AST_For(AST_Assign *init_, bool dir_, AST_Exp *fin_, AST_Stmt *stmt_) : init(init_), dir(dir_), fin(fin_), stmt(stmt_) {}

AST_For::~AST_For() {}

int AST_For::calculate()
{
    return ERROR_VAL;
}

AST_Goto::AST_Goto(int label_) : label(label_) {}

AST_Goto::~AST_Goto() {}

int AST_Goto::calculate()
{
    return ERROR_VAL;
}

AST_Compound::AST_Compound(std::vector<AST_Stmt *> *stmtList_) : stmtList(stmtList_) {}

AST_Compound::~AST_Compound() {}

int AST_Compound::calculate()
{
    return ERROR_VAL;
}

AST_Func::AST_Func(bool isProc_, std::string &funcId_, std::vector<AST_Exp *> *argList_) : isProc(isProc_), funcId(funcId_), argList(argList_) {}

AST_Func::AST_Func(int sysFuncId_, std::vector<AST_Exp *> *argList_) : argList(argList_)
{
    switch (sysFuncId_)
    {
    case 0:
        isProc = false;
        funcId = "abs";
    case 1:
        isProc = false;
        funcId = "chr";
    case 2:
        isProc = false;
        funcId = "odd";
    case 3:
        isProc = false;
        funcId = "ord";
    case 4:
        isProc = false;
        funcId = "pred";
    case 5:
        isProc = false;
        funcId = "sqr";
    case 6:
        isProc = false;
        funcId = "sqrt";
    case 7:
        isProc = false;
        funcId = "succ";
    case 8:
        isProc = true;
        funcId = "write";
    case 9:
        isProc = true;
        funcId = "writeln";
    case 10:
        isProc = true;
        funcId = "read";
    default:
        isProc = true;
        funcId = "";
    }
}

AST_Func::~AST_Func() {}

int AST_Func::calculate()
{
    if (isProc)
    {
        return ERROR_VAL;
    }
    else
    {
        return 0;
    }
}
