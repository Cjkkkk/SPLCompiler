/*
 * @file        spl_compiler.hpp
 * @brief       Global symbols and functions
 * @details     .
 * @author      Ke shanbin
 * @date        3/21/2019
 * @version     1.0.0
 * @par         Copyright(c): Zuiqiang Xiaozu(Best Group)
 */

#include "spl_compiler.hpp"

void DoAssert(const char *errorMsg)
{
    fprintf(stderr, "%s\n", errorMsg);
    exit(0);
}

std::string classToString(SPL_CLASS classId)
{
    switch (classId)
    {
        case VAR:
            return "VAR";
            break;

        case CONST:
            return "CONST";
            break;
        
        case TYPE:
            return "TYPE";
            break;

        case FUNC:
            return "FUNC";
            break;

        case LABEL:
            return "LABEL";
            break;

        default:
            return "ERROR";
            break;
    }
}

std::string typeToString(SPL_TYPE typeId)
{
    switch (typeId)
    {
        case BOOL:
            return "BOOL";
            break;

        case CHAR:
            return "CHAR";
            break;
        
        case INT:
            return "INT";
            break;

        case REAL:
            return "REAL";
            break;

        case STRING:
            return "STRING";
            break;
        
        case ENUM:
            return "ENUM";
            break;
        
        case SUBRANGE:
            return "SUBRANGE";
            break;

        case ARRAY:
            return "ARRAY";
            break;
        
        case RECORD:
            return "RECORD";
            break;

        default:
            return "";
            break;
    }
}

std::string opToString(SPL_OP op){
    switch (op)
    {
        case PLUS_:
            return "+";
            break;

        case MINUS_:
            return "-";
            break;

        case MUL_:
            return "*";
            break;

        case DIV_:
            return "/";
            break;

        case AND_:
            return "&&";
            break;

        case OR_:
            return "||";
            break;

        case NOT_:
            return "!";
            break;

        case MINUS__:
            return "-";
            break;

        case MOD_:
            return "%";
            break;
        case EQUAL_:
            return "==";
            break;
        case UNEQUAL_:
            return "!=";
            break;
        case GE_:
            return ">=";
            break;
        case GT_:
            return ">";
            break;
        case LE_:
            return "<=";
            break;
        case LT_:
            return "<";
            break;
        case OP_ASSIGN:
            return "=";
            break;
        case OP_GOTO:
            return "goto";
            break;
        case OP_IF_Z:
            return "ifFalse";
            break;
        case OP_IF:
            return "if";
            break;
        case OP_CALL:
            return "call";
            break;
        case OP_PARAM:
            return "param";
            break;
        case OP_POP:
            return "pop";
            break;
        case OP_NULL:
            return "";
            break;
        case OP_RET:
            return "ret";
            break;
        case OP_PHI:
            return "phi";
            break;
        default:
            return "error";
            break;
    }
}

SPL_OP_NUM getOperandNum(SPL_OP op) {
    switch (op)
    {
        case PLUS_:
            return THREE;

        case MINUS_:
            return THREE;

        case MUL_:
            return THREE;

        case DIV_:
            return THREE;

        case AND_:
            return THREE;

        case OR_:
            return THREE;

        case NOT_:
            return TWO;

        case MINUS__:
            return TWO;

        case MOD_:
            return THREE;

        case EQUAL_:
            return THREE;

        case UNEQUAL_:
            return THREE;

        case GE_:
            return THREE;

        case GT_:
            return THREE;

        case LE_:
            return THREE;

        case LT_:
            return THREE;

        case OP_ASSIGN:
            return TWO;

        case OP_GOTO:
            return ONE;

        case OP_IF_Z:
            return TWO;

        case OP_IF:
            return TWO;

        case OP_CALL:
            return ONE;

        case OP_PARAM:
            return ONE;

        case OP_POP:
            return ONE;

        case OP_NULL:
            return ZERO;

        case OP_RET:
            return ZERO;

        case OP_PHI:
            return ZERO;

        default:
            return ZERO;
    }
}

std::string opTox86Ins(SPL_OP op) {
    switch(op) {
        case PLUS_:
            return "add";
        case MINUS_:
            return "sub";
        case MUL_:
            return "mul";
        case DIV_:
            return "div";
        default:
            return "error";
    }
}

bool isCalculateOp(SPL_OP op) {
    switch(op) {
        case PLUS_:
        case MINUS_:
        case MUL_:
        case DIV_:
        case AND_:
        case OR_:
        case NOT_:
        case MINUS__:
        case MOD_:
        case EQUAL_:
        case UNEQUAL_:
        case GE_:
        case GT_:
        case LE_:
        case LT_:
            return true;
        default:
            return false;
    }
}

std::string x86SizeToString(x86_size size) {
    switch(size) {
        case byte:
            return "byte";
        case word:
            return "word";
        case dword:
            return "dword";
        case qword:
            return "qword";
        default:
            return "invalid";
    }
}

std::string reg_to_string(x86_reg reg) {
    switch(reg) {
        case rax:
            return "rax";
        case rbx:
            return "rbx";
        case rcx:
            return "rcx";
        case rdx:
            return "rdx";
        case rbp:
            return "rbp";
        case rsp:
            return "rsp";
        case rsi:
            return "rsi";
        case rdi:
            return "rdi";
        case r1:
            return "r1";
        case r2:
            return "r2";
        case r3:
            return "r3";
        case r4:
            return "r4";
        case r5:
            return "r5";
        case r6:
            return "r6";
        case r7:
            return "r7";
        case r8:
            return "r8";
        case r9:
            return "r9";
        case r10:
            return "r10";
        case r11:
            return "r11";
        case r12:
            return "r12";
        case r13:
            return "r13";
        case r14:
            return "r14";
        case r15:
            return "r15";
        default:
            return "not_in";
    }
}

std::string x86SizeToBssInit(x86_size size, unsigned int length) {
    std::string op;
    switch (size) {
        case byte:
            op = "resb";
            break;
        case word:
            op = "resw";
            break;
        case dword:
            op = "resd";
            break;
        case qword:
            op = "resq";
            break;
        default:
            op = "error";
    }
    op = op + " " + std::to_string(length);
    return op;
}

