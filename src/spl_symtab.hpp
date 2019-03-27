#ifndef _SPL_SYMTAB_H_
#define _SPL_SYMTAB_H_

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include "spl_compiler.h"

using namespace std;

class Symbol
{
public:
	Symbol(const std::string& name, SPL_CLASS symbolClass, SPL_TYPE symbolType, Symbol* symbolTypePtr = nullptr);

	/* Symbol's name */
	std::string name;

	/* Symbol's class, e.g. variable/constant/function */
	unsigned char symbolClass;

	/* Symbol's type, only available for variable/constant. */
	/* If symbol is atomic type (bool/int/char/real/string), 
	 * then its type can be specified by symbolType, otherwise
	 * the pointer symbolTypePtr is needed.
	 */
	unsigned int symbolType;
	Symbol* symbolTypePtr;

	/* For symbol with a constant value, pointing to ASTNode */
	void* constValue = nullptr;

	/* For array/record/function/procedure, the member/field/argument list */
	std::vector<Symbol*> memberList;

	/* For symbols that are parameters to functions or are 
	   variables declared inside functions, this gives the
	   function they're in. 
	 */
	Symbol* parentFunction = nullptr;
};

class SymbolTable
{
public:

	/* ctor & dtor */
	SymbolTable();
	~SymbolTable();

	/* The parser calls this method when it enters a new scope in the
	 * program; this allows us to track variables that shadows others in
     * outer scopes with same name as well as to efficiently discard all
     * of the variables declared in a particular scope when we exit that
     * scope. 
	 */
	void pushScope();

	/* For each scope started by a call to SymbolTable::PushScope(), there
     * must be a matching call to SymbolTable::PopScope() at the end of
     * that scope. 
	 */
	void popScope();

	bool addVaraible();
	Symbol* lookupVariable(const char* name);

	bool addFunction();
	Symbol* lookupFunction(const char* name);

	bool addType();
	Symbol* lookupType(const char* name);

	bool addLabel();
	Symbol* lookupLabel(const char* name);

private:
	typedef std::map<std::string, Symbol*> SymbolMapType;
	std::vector<SymbolMapType*> freeSymbolMaps;
	std::vector<SymbolMapType*> variables;
	std::vector<SymbolMapType*> functions;
	std::vector<SymbolMapType*> types;
	std::vector<SymbolMapType*> labels;
};

#endif // !_SPL_SYMTAB_H_
