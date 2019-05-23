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
            break;

        case MINUS_:
            return THREE;
            break;

        case MUL_:
            return THREE;
            break;

        case DIV_:
            return THREE;
            break;

        case AND_:
            return THREE;
            break;

        case OR_:
            return THREE;
            break;

        case NOT_:
            return TWO;
            break;

        case MINUS__:
            return TWO;
            break;

        case MOD_:
            return THREE;
            break;
        case EQUAL_:
            return THREE;
            break;
        case UNEQUAL_:
            return THREE;
            break;
        case GE_:
            return THREE;
            break;
        case GT_:
            return THREE;
            break;
        case LE_:
            return THREE;
            break;
        case LT_:
            return THREE;
            break;
        case OP_ASSIGN:
            return TWO;
            break;
        case OP_GOTO:
            return ONE;
            break;
        case OP_IF_Z:
            return TWO;
            break;
        case OP_IF:
            return TWO;
            break;
        case OP_CALL:
            return ONE;
            break;
        case OP_PARAM:
            return ONE;
            break;
        case OP_POP:
            return ONE;
            break;
        case OP_NULL:
            return ZERO;
            break;
        case OP_RET:
            return ZERO;
            break;
        case OP_PHI:
            return ZERO;
            break;
        default:
            return ZERO;
            break;
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