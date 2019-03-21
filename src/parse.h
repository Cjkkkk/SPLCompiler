//
// Created by DELL on 2019/3/21.
//

#ifndef SPLCOMPILER_PARSE_H
#define SPLCOMPILER_PARSE_H

enum opType{
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

enum dataType{
    BOOL_,	    // boolean
	CHAR_,	    // char
	INT_,	    // int
	REAL_,	    // double
	STRING_,	// string
	CUSTOM_,	// user-defined type, value = type_hashcode(<type_name>)
};

#endif //SPLCOMPILER_PARSE_H