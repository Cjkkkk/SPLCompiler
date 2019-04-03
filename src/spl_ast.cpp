/*
 * @file        spl_ast.cpp
 * @brief       Abstract syntax tree implementation
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
    this->nodeType = AST_MATH;
    this->opType = opType;
    this->left = left;
    this->right = right;
}

AST_Math::~AST_Math()
{
    delete (left);
    delete (right);
}
void AST_Math::checkSemantic()
{
    // left->checkSemantic();
    // if(right) right->checkSemantic();
}
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
    this->nodeType = AST_CONST;
    this->valType = INT;
    this->value.valInt = val;
}

AST_Const::AST_Const(double val)
{
    this->nodeType = AST_CONST;
    this->valType = REAL;
    this->value.valDouble = val;
}

AST_Const::AST_Const(char val)
{
    this->nodeType = AST_CONST;
    this->valType = CHAR;
    this->value.valChar = val;
}

AST_Const::AST_Const(bool val)
{
    this->nodeType = AST_CONST;
    this->valType = BOOL;
    this->value.valBool = val;
}

AST_Const::AST_Const(std::string &val)
{
    this->nodeType = AST_CONST;
    this->valType = STRING;
    this->value.valString = val;
}

int AST_Const::calculate()
{
    if (valType == INT)
        return value.valInt;
    return 0;
}

// todo add delete void*
AST_Const::~AST_Const()
{

}

void AST_Const::checkSemantic() {}
AST_Sym::AST_Sym(std::string &id_, SymbolTable *scope_) : id(id_), scope(scope_) 
{
    this->nodeType = AST_SYM;
}

int AST_Sym::calculate()
{
    return 1;
    // add search symbol table
}

// todo add symbol table destruction
AST_Sym::~AST_Sym()
{
    // delete(scope);
}

void AST_Sym::checkSemantic()
{
    // std::cout << "check symbol " + id +" semantic" << std::endl;
    //scope->lookupVariable(id.c_str());
}

// AST_Array
AST_Array::AST_Array(AST_Sym *sym_, AST_Exp *exp_) : sym(sym_), exp(exp_) 
{
    this->nodeType = AST_ARRAY;
}

AST_Array::~AST_Array()
{
    delete (sym);
    delete (exp);
}

int AST_Array::calculate()
{
    return ERROR_VAL;
}
void AST_Array::checkSemantic() {}

// AST_Dot
AST_Dot::AST_Dot(AST_Sym *record_, AST_Sym *field_) : record(record_), field(field_) 
{
    this->nodeType = AST_DOT;
}

AST_Dot::~AST_Dot()
{
    delete (record);
    delete (field);
}

int AST_Dot::calculate()
{
    return ERROR_VAL;
}
void AST_Dot::checkSemantic() {}

// AST_Assign
AST_Assign::AST_Assign(SPL::AST_Exp *lhs_, SPL::AST_Exp *rhs_) : lhs(lhs_), rhs(rhs_) 
{
    this->nodeType = AST_ASSIGN;
}

int AST_Assign::calculate()
{
    return rhs->calculate();
}

AST_Assign::~AST_Assign()
{
    delete (lhs);
    delete (rhs);
}
void AST_Assign::checkSemantic()
{
    // std::cout << "check assignment semantic" << std::endl;
    // lhs->checkSemantic();
    // rhs->checkSemantic();
}
AST_If::AST_If(SPL::AST_Exp *cond_, SPL::AST_Stmt *doIf_, SPL::AST_Stmt *doElse_)
    : cond(cond_), doIf(doIf_), doElse(doElse_)
{
    this->nodeType = AST_IF;
}

AST_If::~AST_If()
{
    delete (cond);
    delete (doElse);
    delete (doIf);
}

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
void AST_If::checkSemantic() {}

AST_While::AST_While(AST_Exp *cond_, AST_Stmt *stmt_) : cond(cond_), stmt(stmt_) 
{
    this->nodeType = AST_WHILE;
}

AST_While::~AST_While()
{
    delete (cond);
    delete (stmt);
}

int AST_While::calculate()
{
    return ERROR_VAL;
}
void AST_While::checkSemantic() {}

AST_Repeat::AST_Repeat(std::vector<AST_Stmt *> *stmtList_, AST_Exp *exp_) : 
    stmtList(stmtList_), exp(exp_) 
{
    this->nodeType = AST_REPEAT;
}

AST_Repeat::~AST_Repeat()
{
    delete exp;
    delete[] stmtList;
}

int AST_Repeat::calculate()
{
    return ERROR_VAL;
}
void AST_Repeat::checkSemantic() {}

AST_For::AST_For(AST_Assign *init_, bool dir_, AST_Exp *fin_, AST_Stmt *stmt_) : 
    init(init_), dir(dir_), fin(fin_), stmt(stmt_) 
{
    this->nodeType = AST_FOR;
}

AST_For::~AST_For()
{
    delete (init);
    delete (fin);
    delete (stmt);
}

int AST_For::calculate()
{
    return ERROR_VAL;
}
void AST_For::checkSemantic() {}

AST_Goto::AST_Goto(int label_) : label(label_) 
{
    this->nodeType = AST_GOTO;
}

AST_Goto::~AST_Goto() {}

int AST_Goto::calculate()
{
    return ERROR_VAL;
}
void AST_Goto::checkSemantic() {}

AST_Compound::AST_Compound(std::vector<AST_Stmt *> *stmtList_) : stmtList(stmtList_) 
{
    this->nodeType = AST_COMPOUND;
}

AST_Compound::~AST_Compound()
{
    delete[] stmtList;
}

int AST_Compound::calculate()
{
    return ERROR_VAL;
}
void AST_Compound::checkSemantic() {}

AST_Func::AST_Func(bool isProc_, std::string &funcId_, std::vector<AST_Exp *> *argList_) : 
    isProc(isProc_), funcId(funcId_), argList(argList_) 
{
    this->nodeType = AST_FUNC;
}

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

AST_Func::~AST_Func()
{
    delete[] argList;
}

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

void AST_Func::checkSemantic() {}

// AST_Routine::AST_Routine(vector<SPL::AST_RoutineHead *> *routine_head_, SPL::AST_Compound *routine_body_)
// {
//     routine_body = routine_body_;
//     routine_head = routine_head_;
// }
// void AST_Routine::checkSemantic()
// {
//     for (auto part : *routine_head)
//     {
//         if (part)
//             part->checkSemantic();
//     }
//     routine_body->checkSemantic();
// }

// int AST_Routine::calculate()
// {
//     for (auto part : *routine_head)
//     {
//         if (part)
//             part->calculate();
//     }
//     routine_body->calculate();
//     return -ERROR_VAL;
// }
// AST_Program::AST_Program(string &id_, SPL::AST_Routine *routine_)
// {
//     id = id_;
//     routine = routine_;
// }

// void AST_Program::checkSemantic()
// {
//     routine->checkSemantic();
// }

// int AST_Program::calculate()
// {
//     return routine->calculate();
// }