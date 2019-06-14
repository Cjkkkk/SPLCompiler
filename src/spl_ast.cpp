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
    if(left->tempVariable == nullptr) {
        left->emit(ir);
    }
    if(right && right->tempVariable == nullptr) {
        right->emit(ir);
    }

    if(opType == MINUS__) {
//        ir->decreaseTempCount(left->tempVariable);
        tempVariable = ir->genTempVariable(valType);
        ir->addInstruction(new Instruction{"", MINUS__, left->getTempVariable(), nullptr, tempVariable});
    } else {
//        ir->decreaseTempCount(left->tempVariable);
//        ir->decreaseTempCount(right->tempVariable);
        tempVariable = ir->genTempVariable(valType);
        ir->addInstruction(new Instruction{"", opType , left->getTempVariable(), right->getTempVariable(), tempVariable});
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
    // tempVariable = ir->genTempVariable(valType);
    auto literal = new Operand(valType, "", KNOWN);

    switch (valType) {
        case INT: literal->value.valInt = getValue().valInt;
        case BOOL: literal->value.valBool = getValue().valBool;
        case REAL: literal->value.valDouble = getValue().valDouble;
        case CHAR: literal->value.valChar = getValue().valChar;
        case STRING: literal->value.valString = getValue().valString;
        default:
            break;
    }
    tempVariable = literal;
    // ir->addInstruction(new Instruction{"", OP_ASSIGN, literal, nullptr, tempVariable});
}
AST_Sym::AST_Sym(std::string &id_, unsigned int scopeIndex_, Symbol* symbol_) : id(id_), scopeIndex(scopeIndex_), symbol(symbol_)
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
    tempVariable = new Operand(valType, id + "." + std::to_string(scopeIndex), VAR, get_symbol());
}

// AST_Array
AST_Array::AST_Array(AST_Sym *sym_, AST_Exp *exp_) : sym(sym_), exp(exp_)
{
    symbol = sym->symbol;
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
    // 先计算offset 存到一个临时变量中
    // exp->emit(ir);
    // 如何将offset嵌入这个Operand之中
    // tempVariable = new Operand(valType, id + "." + std::to_string(scopeIndex), VAR);
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
    if(rhs->tempVariable == nullptr){
        rhs->emit(ir);
    }
    if(lhs->tempVariable == nullptr){
        lhs->emit(ir);
    }
    ir->addInstruction(new Instruction{"", OP_ASSIGN, rhs->getTempVariable(), nullptr, lhs->getTempVariable()});
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
    auto ifLabel = ir->genLabel();
    auto elseLabel = ir->genLabel();

    ir->addInstruction(new Instruction{"", OP_IF_Z, cond->getTempVariable(), nullptr, elseLabel});
    // if 开始
    ir->addInstruction(new Instruction{ifLabel->name, OP_NULL, nullptr, nullptr, nullptr});

    doIf->emit(ir);

    ir->addInstruction(new Instruction{"", OP_GOTO, nullptr, nullptr, nullptr}); // if结束跳到exit

    auto indexOfGoto = ir->getCurrentIR().size() - 1;

    ir->addInstruction(new Instruction{elseLabel->name , OP_NULL, nullptr, nullptr, nullptr}); // else标签

    if(doElse) {
        doElse->emit(ir);
    }

    auto exitLabel = ir->genLabel();

    ir->backFill(exitLabel, indexOfGoto);

    ir->addInstruction(new Instruction{exitLabel->name, OP_NULL, nullptr, nullptr, nullptr}); // exit标签

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
    auto stmtLabel = ir->genLabel();

    ir->addInstruction(new Instruction{whileLabel->name, OP_NULL, nullptr, nullptr , nullptr}); // while判断条件


    cond->emit(ir);


    ir->addInstruction(new Instruction{"", OP_IF_Z, cond->getTempVariable(), nullptr , nullptr});
    auto indexOfGoto = ir->getCurrentIR().size() - 1;
    ir->addInstruction(new Instruction{stmtLabel->name, OP_NULL, nullptr, nullptr , nullptr}); // trival label

    stmt->emit(ir);

    auto exitLabel = ir->genLabel();
    ir->backFill(exitLabel, indexOfGoto);
    ir->addInstruction(new Instruction{"", OP_GOTO, nullptr, nullptr,whileLabel}); // if结束跳到exit

    ir->addInstruction(new Instruction{exitLabel->name , OP_NULL, nullptr, nullptr , nullptr}); // exit标签
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

    auto repeatLabel = ir->genLabel();;

    ir->addInstruction(new Instruction{repeatLabel->name, OP_NULL, nullptr, nullptr , nullptr}); // while判断条件

    for(const auto& stmt : *stmtList) {
        stmt->emit(ir);
    }

    exp->emit(ir);
    ir->addInstruction(new Instruction{"", OP_IF, exp->tempVariable,nullptr, nullptr});

    // trival exit
    auto exitLabel = ir->genLabel();
    ir->addInstruction(new Instruction{exitLabel->name, OP_NULL,nullptr, nullptr , nullptr});
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
    auto ifLabel = ir->genLabel(); // trival if label
    auto temp = ir->genTempVariable(INT);
    auto op = dir ? LE_ : GE_;
    auto plusOrMinus = dir ? PLUS_ : MINUS_;

    ir->addInstruction(new Instruction{forLabel->name, OP_NULL, nullptr, nullptr , nullptr}); // while判断条件
    // 判断语句
    ir->addInstruction(new Instruction{"", op, init->tempVariable, fin->tempVariable, temp});
    //判断失败则直接跳到exitLabel
    ir->addInstruction(new Instruction{"", OP_IF_Z, temp, nullptr, nullptr});
    auto indexOfGoto = ir->getCurrentIR().size() - 1;

    ir->addInstruction(new Instruction{ifLabel->name, OP_NULL,nullptr, nullptr , nullptr});// trival goto
    // 生成判断成功需要执行的代码
    stmt->emit(ir);
    // 更改初始值
    auto literal = new Operand(INT, "", KNOWN);
    literal->value.valInt = 1;
    ir->addInstruction(new Instruction{"", plusOrMinus, init->tempVariable , literal,  init->tempVariable});
    // 回到判断的位置
    ir->addInstruction(new Instruction{"", OP_GOTO, nullptr, nullptr , forLabel});
    // 添加exitLabel
    auto exitLabel = ir ->genLabel();
    ir->backFill(exitLabel, indexOfGoto);
    ir->addInstruction(new Instruction{exitLabel->name, OP_NULL, nullptr, nullptr , nullptr});

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
    ir->addInstruction(new Instruction{"", OP_GOTO, nullptr, nullptr, new Operand(STRING, std::to_string(label), LABEL)});
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

AST_Func::AST_Func(bool isProc_, std::string &funcId_, std::vector<AST_Exp *> *argList_, unsigned int scopeIndex_, Symbol* symbol_) :
    isProc(isProc_), funcId(funcId_), argList(argList_), scopeIndex(scopeIndex_), symbol(symbol_)
{
    isSys = false;
    this->nodeType = AST_FUNC;
}


AST_Func::AST_Func(int sysFuncId_, std::vector<AST_Exp *> *argList_) : argList(argList_), scopeIndex(0)
{
    isSys = true;
    switch (sysFuncId_)
    {
    case 0:
        isProc = false;
        funcId = "abs";
            break;
    case 1:
        isProc = false;
        funcId = "chr";
            break;
    case 2:
        isProc = false;
        funcId = "odd";
            break;
    case 3:
        isProc = false;
        funcId = "ord";
            break;
    case 4:
        isProc = false;
        funcId = "pred";
            break;
    case 5:
        isProc = false;
        funcId = "sqr";
            break;
    case 6:
        isProc = false;
        funcId = "sqrt";
            break;
    case 7:
        isProc = false;
        funcId = "succ";
            break;
    case 8:
        isProc = true;
        funcId = "write";
            break;
    case 9:
        isProc = true;
        funcId = "writeln";
            break;
    case 10:
        isProc = true;
        funcId = "read";
            break;
    default:
        isProc = true;
        funcId = "error";
            break;
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
        if(arg->tempVariable == nullptr){
            arg->emit(ir);
        }
    }
    int totalSize = 0;
    for(const auto& arg : *argList) {
        ir->addInstruction(new Instruction{"", OP_PARAM, arg->getTempVariable(), nullptr, nullptr});
        switch(arg->valType){
            case INT:
                totalSize += 4;
                break;
            case REAL:
                totalSize += 8;
                break;
            case CHAR:
                totalSize += 1;
                break;
            case BOOL:
                totalSize += 1;
                break;
            case STRING:
                totalSize += 4; // 无法计算具体的size
                break;
            default:
                totalSize += 100; //
        }
    }
    auto literal = new Operand(INT, "", KNOWN);
    literal->value.valInt = totalSize;
    Symbol* sym = isSys ? nullptr : get_symbol();
    if(isProc) {
        ir->addInstruction(new Instruction{"", OP_CALL, new Operand(valType, funcId + "." +std::to_string(scopeIndex), FUNC, sym), nullptr,nullptr});
    } else {
        tempVariable = ir->genTempVariable(valType);
        ir->addInstruction(new Instruction{"", OP_CALL, new Operand(valType, funcId + "." +std::to_string(scopeIndex), FUNC, sym), nullptr, tempVariable});
    }
    ir->addInstruction(new Instruction{"", OP_POP, literal, nullptr, nullptr});
}
