//
// Created by DELL on 2019/3/21.
//
// This header is definition for global symbols.

#ifndef _SPL_COMPILER_H_
#define _SPL_COMPILER_H_

enum SPL_OP: unsigned int
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
enum SPL_CLASS: unsigned char
{
	VAR,	// variable
    CONST,  // constant
	TYPE,	// user-defined type
	FUNC,	// function
	PROC,	// procedure
    LABEL   // label
};

// type of a variable/constant/type symbol
// if the value is negative, it means reference of it
enum SPL_TYPE: int
{
	BOOL = 1,	// boolean
	CHAR,	    // char
	INT,	    // int
	REAL,	    // double
	STRING,	    // string
    RECORD      // value = type_hashcode(<record_name>) 
};

#endif //_SPL_COMPILER_H_