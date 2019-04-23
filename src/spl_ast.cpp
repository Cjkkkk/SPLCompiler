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
#include "spl_IR.hpp"
#include <algorithm>
using namespace SPL;

AST::~AST() {}

AST_Exp::~AST_Exp() {}

AST_Stmt::~AST_Stmt() {}

// ast node for math expression
AST_Math::AST_Math(SPL_OP opType_, AST_Exp *left_, AST_Exp *right_)
:opType(opType_), left(left_), right(right_) {
    this->nodeType = AST_MATH;
}

AST_Math::~AST_Math()
{
    delete (left);
    delete (right);
}
void AST_Math::checkSemantic()
{

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

void AST_Math::emit(SPL_IR* ir) {
    if(left->tempVariable == "") {
        left->emit(ir);
    }
    if(right && right->tempVariable == "") {
        right->emit(ir);
    }
    tempVariable = ir->genTempVariable();
    if(opType == MINUS__) {
        ir->addInstruction({"", MINUS__, left->tempVariable, "", tempVariable});
    } else {
        ir->addInstruction({"", opType , left->tempVariable, right->tempVariable, tempVariable});
    }
}
// ast node for constant expression
AST_Const::AST_Const(int val)
{
    this->valType = INT;
    this->nodeType = AST_CONST;
    this->value.valInt = val;
}

AST_Const::AST_Const(double val)
{
    this->valType = REAL;
    this->nodeType = AST_CONST;
    this->value.valDouble = val;
}

AST_Const::AST_Const(char val)
{
    this->valType = CHAR;
    this->nodeType = AST_CONST;
    this->value.valChar = val;
}

AST_Const::AST_Const(bool val)
{
    this->valType = BOOL;
    this->nodeType = AST_CONST;
    this->value.valBool = val;
}

AST_Const::AST_Const(std::string &val)
{
    this->valType = STRING;
    this->nodeType = AST_CONST;
    this->value.valString = new std::string(val);
}

int AST_Const::calculate()
{
    if (valType == INT)
        return value.valInt;
    return 0;
}

AST_Const::~AST_Const()
{
    if(this->valType == STRING){
        delete this->value.valString;
        this->value.valString = nullptr;
    }
}

void AST_Const::checkSemantic() {}
void AST_Const::emit(SPL_IR* ir){
    tempVariable = ir->genTempVariable();
    ir->addInstruction({"", OP_ASSIGN, "const", "", tempVariable});
}
AST_Sym::AST_Sym(std::string &id_, unsigned int scopeIndex_) : id(id_), scopeIndex(scopeIndex_)
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
void AST_Sym::emit(SPL_IR* ir){
    tempVariable = std::to_string(scopeIndex) + "." + id;
    //tempVariable = ir->genTempVariable(id);
    //ir->addInstruction({"", OP_ASSIGN, std::to_string(scopeIndex) + "." + id, "", tempVariable});
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
void AST_Array::emit(SPL_IR* ir){

}
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
void AST_Dot::emit(SPL_IR* ir){}
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

void AST_Assign::emit(SPL_IR* ir){
    if(rhs->tempVariable == ""){
        rhs->emit(ir);
    }
    if(lhs->tempVariable == ""){
        lhs->emit(ir);
    }
    ir->addInstruction({"", OP_ASSIGN, rhs->tempVariable, "", lhs->tempVariable});
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
void AST_If::emit(SPL_IR* ir){
    cond->emit(ir);
    auto elseLabel = ir->genLabel();
    auto exitLabel = ir->genLabel();
    ir->addInstruction({"", OP_IF_Z, cond->tempVariable, "", elseLabel});

    doIf->emit(ir);
    ir->addInstruction({"", OP_GOTO, "", "",exitLabel}); // if结束跳到exit

    ir->addInstruction({elseLabel , OP_NULL, "", "",""}); // else标签
    if(doElse) {
        doElse->emit(ir);
    }
    ir->addInstruction({exitLabel, OP_NULL, "", "", ""}); // exit标签

}
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
void AST_While::emit(SPL_IR* ir){
    auto whileLabel = ir->genLabel();
    auto exitLabel = ir->genLabel();
    ir->addInstruction({whileLabel, OP_NULL, "", "", ""}); // while判断条件

    cond->emit(ir);


    ir->addInstruction({"", OP_IF_Z, cond->tempVariable, "", exitLabel});


    stmt->emit(ir);
    ir->addInstruction({"", OP_GOTO, "", "",whileLabel}); // if结束跳到exit

    ir->addInstruction({exitLabel , OP_NULL, "", "",""}); // exit标签
}
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
void AST_Repeat::emit(SPL_IR* ir){
    auto repeatLabel = ir->genLabel();
    auto exitLabel = ir->genLabel();
    ir->addInstruction({repeatLabel, OP_NULL, "", "", ""}); // while判断条件

    for(const auto& stmt : *stmtList) {
        stmt->emit(ir);
    }

    exp->emit(ir);
    ir->addInstruction({"", OP_IF, exp->tempVariable, "", repeatLabel});
}

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
void AST_For::emit(SPL_IR* ir) {
    init->emit(ir);
    fin->emit(ir);
    auto forLabel = ir->genLabel();
    auto exitLabel = ir ->genLabel();
    auto temp = ir->genTempVariable();

    auto op = dir ? LE_ : GE_;
    auto plusOrMinus = dir ? PLUS_ : MINUS_;


    ir->addInstruction({forLabel, OP_NULL, "", "", ""});


    // 判断语句
    ir->addInstruction({"", op, init->tempVariable, fin->tempVariable, temp});

    //判断失败则直接跳到exitLabel
    ir->addInstruction({"", OP_IF_Z, temp, "", exitLabel});

    // 生成判断成功需要执行的代码
    stmt->emit(ir);

    // 更改初始值
    ir->addInstruction({"", plusOrMinus, init->tempVariable , "const",  init->tempVariable });
    // 回到判断的位置
    ir->addInstruction({"", OP_GOTO, "", "", forLabel});

    // 添加exitLabel
    ir->addInstruction({exitLabel, OP_NULL, "", "", ""});

}
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
void AST_Goto::emit(SPL_IR* ir) {
    // todo label 真的存在吗
    ir->IR.emplace_back("", OP_GOTO, "", "", std::to_string(label));
}
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
void AST_Compound::emit(SPL_IR* ir) {
    for(const auto& stmt : *stmtList) {
        stmt->emit(ir);
    }
}
AST_Func::AST_Func(bool isProc_, std::string &funcId_, std::vector<AST_Exp *> *argList_, unsigned int scopeIndex_) :
    isProc(isProc_), funcId(funcId_), argList(argList_), scopeIndex(scopeIndex_)
{
    this->nodeType = AST_FUNC;
}


AST_Func::AST_Func(int sysFuncId_, std::vector<AST_Exp *> *argList_) : argList(argList_), scopeIndex(0)
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
void AST_Func::emit(SPL_IR* ir) {
    for(const auto& arg : *argList) {
        if(arg->tempVariable == ""){
            arg->emit(ir);
        }
    }
    for(const auto& arg : *argList) {
        ir->addInstruction({"", OP_PARAM, arg->tempVariable, "", ""});
    }
    ir->addInstruction({"", OP_CALL, std::to_string(scopeIndex) + "." + funcId, "", ""});
}

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
