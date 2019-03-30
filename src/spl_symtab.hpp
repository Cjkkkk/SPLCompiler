#ifndef _SPL_SYMTAB_H_
#define _SPL_SYMTAB_H_

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include "spl_compiler.hpp"

using namespace std;

class Symbol;
typedef std::map<std::string, Symbol*> SymbolMapType;

class Symbol
{
public:
    Symbol(const std::string& name, SPL_CLASS symbolClass, SPL_TYPE symbolType);

    /* Symbol's name */
    std::string name;

    /* Symbol's class, e.g. variable/constant/function */
    SPL_CLASS symbolClass;

    /* Symbol's type, only available for variable/constant/type */
    SPL_TYPE symbolType;

    /* For symbol with a constant value, pointing to ASTNode */ 
    void* constValuePtr;

    /* For array type, the member type.
     * If symbol is atomic type (bool/int/char/real/string), 
     * then its type can be specified by elementType, otherwise
     * the pointer elementTypePtr is needed for more information.
     */
    SPL_TYPE elementType;
    Symbol* elementTypePtr;

    /* Array size */
    unsigned int arraySize;

    /* For record type/variable, the member list */
    SymbolMapType* memberList;

    /* For function/procedure symbol, the argument list */
    std::vector<Symbol*>* argsList;

    /* Parameter passing mode */
    SPL_PARA paraType;

    // TODO: subrange type and enum type
    // std::map<Symbol*, unsigned int>* enumMap;
    // std::map<unsigned int, unsigned int>* subrangeMap;

    /* For symbols that are parameters to functions or are 
     * variables declared inside functions, this gives the
     * scope they're in. 
     */
    SymbolMapType* parentScope;

    /* Return value's type, only available for function.
     * If symbol is atomic type (bool/int/char/real/string), 
     * then its type can be specified by returnType, otherwise
     * the pointer returnTypePtr is needed.
     */
    SPL_TYPE returnType;
    Symbol* returnTpyePtr;
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
    void pushScope(const std::string& scopeName);

    /* For each scope started by a call to SymbolTable::PushScope(), there
     * must be a matching call to SymbolTable::PopScope() at the end of
     * that scope. 
     */
    void popScope();

    bool addVariable(Symbol* symbol);
    Symbol* lookupVariable(const char* name);

    bool addFunction(Symbol* symbol);
    Symbol* lookupFunction(const char* name);

    bool addType(Symbol* symbol);
    Symbol* lookupType(const char* name);

    bool addLabel(Symbol* symbol);
    Symbol* lookupLabel(const char* name);

    void print();
    void printType(Symbol* sym);

private:
    std::vector<std::string> scopeNames;
    std::vector<SymbolMapType*> freeSymbolMaps;
    std::vector<SymbolMapType*> variables;
    SymbolMapType functions;
    SymbolMapType types;
    SymbolMapType labels;
};

#endif // !_SPL_SYMTAB_H_
