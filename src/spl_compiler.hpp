/*
 * @file        spl_compiler.hpp
 * @brief       Global symbols and functions
 * @details     .
 * @author      Ke shanbin
 * @date        3/21/2019
 * @version     1.0.0
 * @par         Copyright(c): Zuiqiang Xiaozu(Best Group)
 */

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
    MINUS__,


    OP_ASSIGN,
    OP_IF_Z,
    OP_PARAM,
    OP_CALL,
    OP_REF,
    OP_RET,
    OP_GOTO,
    OP_NULL,
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
enum SPL_TYPE : unsigned char
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

enum AST_TYPE : unsigned int
{
    AST_MATH,
    AST_CONST,
    AST_SYM,
    AST_ARRAY,
    AST_DOT,
    AST_ASSIGN,
    AST_IF,
    AST_WHILE,
    AST_REPEAT,
    AST_FOR,
    AST_GOTO,
    AST_COMPOUND,
    AST_FUNC,
};

// parameter passing mode
enum SPL_PARA : bool
{
    VALUE = false,  // pass by value
    REFER = true    // pass by reference
};

void DoAssert(const char *errorMsg);

std::string SPL_OPToString(SPL_OP op);
std::string classToString(SPL_CLASS classId);
std::string typeToString(SPL_TYPE typeId);

#endif //_SPL_COMPILER_H_