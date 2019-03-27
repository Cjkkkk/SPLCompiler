//
// Created by DELL on 2019/3/21.
//
// This header is definition for global symbols.

#ifndef _SPL_COMPILER_H_
#define _SPL_COMPILER_H_

#include <cstdio>
#include <cstdlib>

#define Assert(expr, errorMsg) ((void)((expr) ? 0 : ((void)DoAssert(errorMsg), 0)))

void DoAssert(const char *errorMsg)
{
    fprintf(stderr, "%s\n", errorMsg);
    exit(0);
}

enum SPL_OP : unsigned int
{
    PLUS_ = 1,
    MINUS_,
    MUL_,
    DIV_,
    MOD_,
    OR_,
    AND_,
    EQUAL_,
    UNEQUAL_,
    GE_,
    GT_,
    LE_,
    LT_,
    NOT_,
    MINUS__
};

// class of a symbol
enum SPL_CLASS : unsigned char
{
    VAR,   // variable
    CONST, // constant
    TYPE,  // user-defined type
    FUNC,  // function/procedure
    LABEL  // label
};

// type of a variable symbol
enum SPL_TYPE : unsigned int
{
    BOOL = 1, // boolean
    CHAR,     // char
    INT,      // int
    REAL,     // double
    STRING,   // string
    ARRAY,    // array
    RECORD    // record
};

#endif //_SPL_COMPILER_H_