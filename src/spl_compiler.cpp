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

std::string SPL_OPToString(SPL_OP op){
    switch (op)
    {
        case PLUS_:
            return "PLUS";
            break;

        case MINUS_:
            return "MINUS";
            break;

        case MUL_:
            return "MUL";
            break;

        case DIV_:
            return "DIV";
            break;

        case AND_:
            return "AND";
            break;

        case OR_:
            return "OR";
            break;

        case NOT_:
            return "NOT";
            break;

        case MINUS__:
            return "MINUS_";
            break;

        case MOD_:
            return "MOD";
            break;
        case EQUAL_:
            return "EQUAL";
            break;
        case UNEQUAL_:
            return "UNEQUAL";
            break;
        case GE_:
            return "GE";
            break;
        case GT_:
            return "GT";
            break;
        case LE_:
            return "LE";
            break;
        case LT_:
            return "LT";
            break;
        case OP_ASSIGN:
            return "ASSIGN";
            break;
        case OP_GOTO:
            return "GOTO";
            break;
        case OP_IF_Z:
            return "IF_Z";
            break;
        case OP_IF:
            return "IF";
            break;
        case OP_CALL:
            return "CALL";
            break;
        case OP_PARAM:
            return "PARAM";
            break;
        case OP_POP:
            return "POP";
            break;
        case OP_NULL:
            return "";
            break;
        case OP_RET:
            return "RETURN";
            break;
        case OP_PHI:
            return "PHI";
            break;
        default:
            return "ERROR";
            break;
    }
}