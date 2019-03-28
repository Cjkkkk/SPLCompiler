//
// Created by DELL on 2019/3/21.
//
// This header is definition for global symbols and global functions.

#ifndef _SPL_COMPILER_HPP_
#define _SPL_COMPILER_HPP_

#include <iostream>
#include <cstdio>
#include <cstdlib>
//#include <assert.h>

#define Assert(expr, errorMsg) ((void)((expr) ? 0 : ((void)DoAssert(errorMsg), 0)))

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
    UNKNOWN,    // unknown
    BOOL,       // boolean
    CHAR,       // char
    INT,        // int
    REAL,       // double
    STRING,     // string
    ENUM,       // enumeration
    SUBRANGE,	// subrange
    ARRAY,      // array
    RECORD      // record
};

// parameter passing mode
enum SPL_PASSMODE : bool
{
    VALUE = false,  // pass by value
    REFER = true    // pass by reference
};

void DoAssert(const char *errorMsg);

std::string classToString(SPL_CLASS classId);
std::string typeToString(SPL_TYPE typeId);

#endif //_SPL_COMPILER_H_