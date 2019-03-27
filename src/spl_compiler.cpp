#include "spl_compiler.hpp"

void DoAssert(const char *errorMsg)
{
    fprintf(stderr, "%s\n", errorMsg);
    exit(0);
}