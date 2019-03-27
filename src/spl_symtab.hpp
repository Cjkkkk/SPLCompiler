#ifndef _SPL_SYMTAB_H_
#define _SPL_SYMTAB_H_

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include "spl_compiler.hpp"

using namespace std;

class Symbol
{
public:
    Symbol(const std::string& name, SPL_CLASS symbolClass, SPL_TYPE symbolType, Symbol* symbolTypePtr = nullptr);

    /* Symbol's name */
    std::string name;

    /* Symbol's class, e.g. variable/constant/function */
    SPL_CLASS symbolClass;

    /* Symbol's type, only available for variable/constant.
     * If symbol is atomic type (bool/int/char/real/string), 
     * then its type can be specified by symbolType, otherwise
     * the pointer symbolTypePtr is needed.
     */
    SPL_TYPE symbolType;
    Symbol* symbolTypePtr;

    /* For symbol with a constant value, pointing to ASTNode */ 
    void* constValue;

    /* For array/record/function/procedure, the member/field/argument list */
    std::vector<Symbol*>* memberList;

    /* For symbols that are parameters to functions or are 
       variables declared inside functions, this gives the
       scope they're in. 
     */
    std::map<std::string, Symbol*>* parentScope;
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

    bool addVaraible(Symbol* symbol);
    Symbol* lookupVariable(const char* name);

    bool addFunction(Symbol* symbol);
    Symbol* lookupFunction(const char* name);

    bool addType(Symbol* symbol);
    Symbol* lookupType(const char* name);

    bool addLabel(Symbol* symbol);
    Symbol* lookupLabel(const char* name);

    void print();

private:
    typedef std::map<std::string, Symbol*> SymbolMapType;
    std::vector<SymbolMapType*> freeSymbolMaps;
    std::vector<SymbolMapType*> variables;
    std::vector<SymbolMapType*> functions;
    std::vector<SymbolMapType*> types;
    std::vector<SymbolMapType*> labels;
};

#endif // !_SPL_SYMTAB_H_
