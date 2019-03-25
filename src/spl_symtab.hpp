#ifndef _SPL_SYMTAB_H_
#define _SPL_SYMTAB_H_

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include "spl_compiler.h"

using namespace std;

unsigned int hashcode(string& type_name);

// class 

class Symbol {
public:
	Symbol();
	~Symbol();

	std::string 		symbol_id;

	unsigned int		symbol_class = SPL_CLASS::VAR;

	// symbol_type is available only when symbol is:
	// 1. variable - VAR
	// 2. constant - CONST
	// 3. user-defined type - TYPE
	// initialize to be unknown (0)
	unsigned int		symbol_type = 0;

	// array_size is available only when symbol is array variable
	// detail: if it's not an array, value == 0, else the array size
	unsigned int 		array_size = 0;
	
	// type_list is available only when symbol is function or record
	// detail: (field/param, type)
	// 1. function - the argument list
	//	- if it's positive integer -- pass by value
	//	- if it's negative integer -- pass by reference
	// 
	// 2. record - the field list
	//	- all of them should be positive integer
	map<string, SPL_TYPE>	type_list;


	// ret_type is available only when symbol is function
	// detail: the return value type of a function
	SPL_TYPE			ret_type = SPL_TYPE::BOOL;

	// label_ptr is available only when symbol is a label
	// detail: it points to the corresponding AST node
	void*				label_ptr = nullptr;
};

class SymbolTable {
public:
	SymbolTable();
	~SymbolTable();

	map<string, Symbol>		symbols;
	void*					scope	= nullptr;
	SymbolTable*			parent	= nullptr;
};

#endif // !_SPL_SYMTAB_H_
