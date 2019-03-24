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
	TYPE,	// user-defined typename
	FUNC,	// function
	PROC	// procedure
};

// type of a variable symbol
enum SPL_TYPE: unsigned int
{
	BOOL,	// boolean
	CHAR,	// char
	INT,	// int
	REAL,	// double
	STRING,	// string
	CUSTOM	// user-defined type, value = type_hashcode(<type_name>)
};

#endif //_SPL_COMPILER_H_