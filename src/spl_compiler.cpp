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

        case CONST:
            return "CONST";

        case TYPE:
            return "TYPE";

        case FUNC:
            return "FUNC";

        case LABEL:
            return "LABEL";
        case PARAM:
            return "PARAM";
        case RET:
            return "RET";
        default:
            return "ERROR";

    }
}

std::string typeToString(SPL_TYPE typeId)
{
    switch (typeId)
    {
        case BOOL:
            return "BOOL";


        case CHAR:
            return "CHAR";

        
        case INT:
            return "INT";


        case REAL:
            return "REAL";


        case STRING:
            return "STRING";

        
        case ENUM:
            return "ENUM";

        
        case SUBRANGE:
            return "SUBRANGE";


        case ARRAY:
            return "ARRAY";

        
        case RECORD:
            return "RECORD";


        default:
            return "error";

    }
}

std::string opToString(SPL_OP op){
    switch (op)
    {
        case PLUS_:
            return "+";


        case MINUS_:
            return "-";


        case MUL_:
            return "*";


        case DIV_:
            return "/";


        case AND_:
            return "&&";


        case OR_:
            return "||";


        case NOT_:
            return "!";


        case MINUS__:
            return "-";


        case MOD_:
            return "%";

        case EQUAL_:
            return "==";

        case UNEQUAL_:
            return "!=";

        case GE_:
            return ">=";

        case GT_:
            return ">";

        case LE_:
            return "<=";

        case LT_:
            return "<";

        case OP_ASSIGN:
            return "=";

        case OP_GOTO:
            return "goto";

        case OP_IF_Z:
            return "ifFalse";

        case OP_IF:
            return "if";

        case OP_CALL:
            return "call";

        case OP_PARAM:
            return "param";

        case OP_POP:
            return "pop";

        case OP_NULL:
            return "";

        case OP_RET:
            return "ret";

        case OP_PHI:
            return "phi";

        case OP_FUNC_PARAM:
            return "func_param";

        case OP_FUNC_RET:
            return "func_ret";

        default:
            return "error";

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
        case MOD_:
        case DIV_:
            return "div";
        case AND_:
            return "and";
        case OR_:
            return "or";
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
        case eax:
            return "eax";
        case ebx:
            return "ebx";
        case ecx:
            return "ecx";
        case edx:
            return "edx";
        case rbp:
            return "rbp";
        case rsp:
            return "esp";
        case esi:
            return "esi";
        case edi:
            return "edi";
        case r8d:
            return "r8d";
        case r9d:
            return "r9d";
        case r10d:
            return "r10d";
        case r11d:
            return "r11d";
        case r12d:
            return "r12d";
        case r13d:
            return "r13d";
        case r14d:
            return "r14d";
        case r15d:
            return "r15d";
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

