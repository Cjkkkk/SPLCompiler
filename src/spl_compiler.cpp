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
            return "ERROR";
            break;
    }
}

std::string SPL_OPToString(SPL_OP op){
    switch (op)
    {
        case PLUS_:
            return "OP_PLUS";
            break;

        case MINUS_:
            return "OP_MINUS";
            break;

        case MUL_:
            return "OP_MUL";
            break;

        case DIV_:
            return "OP_DIV";
            break;

        case AND_:
            return "OP_AND";
            break;

        case OR_:
            return "OP_OR";
            break;

        case NOT_:
            return "OP_NOT";
            break;

        case MINUS__:
            return "OP_MINUS_";
            break;

        case MOD_:
            return "OP_MOD";
            break;
        case EQUAL_:
            return "OP_EQUAL";
            break;
        case UNEQUAL_:
            return "OP_UNEQUAL";
            break;
        case GE_:
            return "OP_GE";
            break;
        case GT_:
            return "OP_GT";
            break;
        case LE_:
            return "OP_LE";
            break;
        case LT_:
            return "OP_LT";
            break;
        case OP_ASSIGN:
            return "OP_ASSIGN";
            break;
        case OP_GOTO:
            return "OP_GOTO";
            break;
        case OP_IF:
            return "OP_IF";
            break;
        case OP_CALL:
            return "OP_CALL";
            break;
        case OP_PARAM:
            return "OP_PARAM";
            break;
        case OP_NULL:
            return "";
            break;
        default:
            return "ERROR";
            break;
    }
}