/*
 * @file        spl_symtab.hpp
 * @brief       Symbol table definition
 * @details     .
 * @author      Jiang Xiaochong
 * @date        3/27/2019
 * @version     1.0.0
 * @par         Copyright(c): Zuiqiang Xiaozu(Best Group)
 */

#ifndef _SPL_SYMTAB_H_
#define _SPL_SYMTAB_H_

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include "spl_compiler.hpp"

using namespace std;

class Symbol;
typedef std::vector<Symbol*> SymbolListType;
typedef std::map<std::string, Symbol*> SymbolMapType;

class Symbol
{
public:
    Symbol(const std::string& name, SPL_CLASS symbolClass, SPL_TYPE symbolType);

    /* Symbol's name */
    std::string name;

    /* Symbol's class, e.g. variable/constant/function */
    SPL_CLASS symbolClass;

    /* Symbol's type. The type of variable/constant/type symbols
     * or return type of function symbols.
     */
    SPL_TYPE symbolType;

    /* If symbol is a constant class, pointing to ASTNode with constant value.
     * If symbol is a label, pointing to corresponding ASTNode with label.
     */ 
    void* relevantASTNode;

    /* For array type, the member type.
     * If symbol is atomic type (bool/int/char/real/string), 
     * then its type can be specified by elementType, otherwise
     * the pointer elementTypePtr is needed for more information.
     */
    SPL_TYPE elementType;
    Symbol* elementTypePtr;

    /* Array/Enumeration/Subrange size */
    unsigned int scalarSize;

    /* In record/function/procedure symbol, we use
     * (name, Symbol*) mapping to gain faster accessing.
     */
    SymbolMapType* subSymbolMap;

    /* For record/function/procedure symbol, the member/argument list */
    SymbolListType* subSymbolList;

    /* Parameter passing mode */
    SPL_PARA paraType;

    // TODO: subrange type and enum type
    // std::map<Symbol*, unsigned int>* enumMap;
    // std::map<unsigned int, unsigned int>* subrangeMap;

    /* Return value's type, only available for function.
     * If symbol is atomic type (bool/int/char/real/string), 
     * then its type can be specified by symbolType, otherwise
     * the pointer returnTypePtr is needed.
     */
    Symbol* returnTypePtr;
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

    bool detectCollision(const std::string& name);

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
    /* containers */
    std::vector<std::string> scopeNames;
    // std::vector<SymbolMapType*> freeSymbolMaps;
    std::vector<SymbolMapType*> variables;
    std::vector<SymbolMapType*> functions;
    std::vector<SymbolMapType*> types;
    SymbolMapType labels;

    /* tracking working scope */
    std::map<unsigned int, unsigned int> prevScopeMap;
    unsigned int currentScopeIndex;

};

#endif // !_SPL_SYMTAB_H_
