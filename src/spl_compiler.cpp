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