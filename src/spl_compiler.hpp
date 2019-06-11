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

#define Assert(expr, errorMsg) ((void)((expr) ? 0 : ((void)DoAssert(errorMsg), 0)))

enum SPL_OP_NUM {
    ZERO,
    ONE,
    TWO,
    THREE
};
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
    OP_IF,
    OP_PARAM,
    OP_CALL,
    OP_POP,
    OP_RET,
    OP_GOTO,
    OP_NULL,
    OP_PHI,

    OP_FUNC_PARAM,
};

// class of a symbol
enum SPL_CLASS
{
    VAR,   // variable
    CONST, // constant
    TYPE,  // user-defined type
    FUNC,  // function/procedure
    LABEL,  // label
    TEMP,   // temp variable
    KNOWN, // Known in compile time / literal
};

// type of a variable symbol
enum SPL_TYPE
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

typedef enum x86_reg {
    eax, ebx, ecx, edx, rbp, rsp,
    esi, edi,
    r8d, r9d, r10d, r11d, r12d, r13d, r14d, r15d,
    not_in
} reg;


typedef enum x86_size {
    byte=1, word=2, dword=4, qword=8, invalid=100
} x86_size;

void DoAssert(const char *errorMsg);
std::string opToString(SPL_OP op);
std::string classToString(SPL_CLASS classId);
std::string typeToString(SPL_TYPE typeId);
std::string opTox86Ins(SPL_OP op);
std::string x86SizeToString(x86_size size);
std::string x86SizeToBssInit(x86_size size, unsigned int length);
std::string reg_to_string(x86_reg reg);
SPL_OP_NUM getOperandNum(SPL_OP op);
bool isCalculateOp(SPL_OP op);
#endif //_SPL_COMPILER_H_