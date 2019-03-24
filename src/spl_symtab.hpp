#ifndef _SPL_SYMTAB_H_
#define _SPL_SYMTAB_H_

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include "spl_compiler.h"

using namespace std;

unsigned int hashcode(string& type_name);

class Symbol {
public:
	Symbol();
	~Symbol();

	unsigned int		symbol_class = 0;

	// specify the type of a variable symbol
	unsigned int		symbol_type = 0;
	
	// specify the type of arguments and return value
	// if it's a function or procedure symbol
	SPL_TYPE			ret_type = SPL_TYPE::BOOL;
	vector<SPL_TYPE>	args_type;

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
