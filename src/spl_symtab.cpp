/*
 * @file        spl_symtab.hpp
 * @brief       Symbol table implementation
 * @details     .
 * @author      Jiang Xiaochong
 * @date        3/27/2019
 * @version     1.0.0
 * @par         Copyright(c): Zuiqiang Xiaozu(Best Group)
 */

#include "spl_symtab.hpp"

Symbol::Symbol(const std::string &name, SPL_CLASS symbolClass, SPL_TYPE symbolType)
    : name(name), symbolClass(symbolClass), symbolType(symbolType)
{
    this->relevantASTNode = nullptr;
    this->elementType = SPL_TYPE::UNKNOWN;
    this->elementTypePtr = nullptr;
    this->subSymbolMap = nullptr;
    this->subSymbolList = nullptr;
    this->scalarSize = 0;
    this->paraType = VALUE;
    this->returnTypePtr = nullptr;
}

SymbolTable::SymbolTable()
{
    currentScopeIndex = -1;
}

SymbolTable::~SymbolTable()
{
    // Assert(variables.size() == 1, "spl.exe: error: mismatched scopes!");
    // popScope();
}

void SymbolTable::pushScope(const std::string &scopeName)
{
    unsigned int newScopeIndex = variables.size();
    scopeNames.push_back(scopeName);
    variables.push_back(new SymbolMapType);
    functions.push_back(new SymbolMapType);
    types.push_back(new SymbolMapType);

    // TODO: assertion
    Assert(prevScopeMap.find(newScopeIndex) == prevScopeMap.end(),
           "spl.exe: error: allocating memory error!");
    prevScopeMap[newScopeIndex] = currentScopeIndex;
    currentScopeIndex = newScopeIndex;
}

void SymbolTable::popScope()
{
    Assert(variables.size() >= 1, "spl.exe: error: mismatched scopes!");
    // freeSymbolMaps.push_back(variables.back());
    currentScopeIndex = prevScopeMap[currentScopeIndex];
}

bool SymbolTable::detectCollision(const std::string &name)
{
    // TODO: mask
    if (
        name == scopeNames[currentScopeIndex] || variables[currentScopeIndex]->find(name) != variables[currentScopeIndex]->end() || functions[currentScopeIndex]->find(name) != functions[currentScopeIndex]->end() || types[currentScopeIndex]->find(name) != types[currentScopeIndex]->end() || labels.find(name) != labels.end())
    {
        return true;
    }
    return false;
}

bool SymbolTable::addVariable(Symbol *symbol)
{
    std::string name = symbol->name;
    // Check to see if a symbol of the same name has already been declared.
    if (
        functions[currentScopeIndex]->find(name) != functions[currentScopeIndex]->end() || variables.back()->find(name) != variables.back()->end())
    {
        std::string errorMsg = "spl.exe: error: ignoring redeclaration of symbol \"" + name + "\".";
        Assert(0, errorMsg.c_str());
        return false;
    }
    // No matches, so go ahead and add it...
    (*variables[currentScopeIndex])[name] = symbol;
    return true;
}

Symbol *SymbolTable::lookupVariable(const char *name)
{
    unsigned int scopeIndex = currentScopeIndex;
    while (scopeIndex >= 0)
    {
        SymbolMapType::iterator iter = variables[scopeIndex]->find(name);
        if (iter != variables[scopeIndex]->end())
            return iter->second;
        if(scopeIndex == 0)
            return nullptr;
        else {
            scopeIndex = prevScopeMap.find(scopeIndex)->second;
        }
    }
    return nullptr;
}

bool SymbolTable::addFunction(Symbol *symbol)
{
    std::string name = symbol->name;
    if (
        variables[currentScopeIndex]->find(name) != variables[currentScopeIndex]->end() || functions[currentScopeIndex]->find(name) != functions[currentScopeIndex]->end() || types[currentScopeIndex]->find(name) != types[currentScopeIndex]->end() || labels.find(name) != labels.end())
    {
        std::string errorMsg = "spl.exe: error: ignoring redeclaration of symbol \"" + name + "\".";
        Assert(0, errorMsg.c_str());
        return false;
    }
    (*functions[currentScopeIndex])[name] = symbol;
    return true;
}

Symbol *SymbolTable::lookupFunction(const char *name)
{
    unsigned int scopeIndex = currentScopeIndex;
    while (scopeIndex >= 0)
    {
        SymbolMapType::iterator iter = functions[scopeIndex]->find(name);
        if (iter != functions[scopeIndex]->end())
            return iter->second;
        if(scopeIndex == 0)
            return nullptr;
        else {
            scopeIndex = prevScopeMap.find(scopeIndex)->second;
        }
    }
    return nullptr;
}

bool SymbolTable::addType(Symbol *symbol)
{
    std::string name = symbol->name;
    if (
        variables[currentScopeIndex]->find(name) != variables[currentScopeIndex]->end() || functions[currentScopeIndex]->find(name) != functions[currentScopeIndex]->end() || types[currentScopeIndex]->find(name) != types[currentScopeIndex]->end() || labels.find(name) != labels.end())
    {
        std::string errorMsg = "spl.exe: error: ignoring redeclaration of symbol \"" + name + "\".";
        Assert(0, errorMsg.c_str());
        return false;
    }
    (*types[currentScopeIndex])[name] = symbol;
    return true;
}

Symbol *SymbolTable::lookupType(const char *name)
{
    unsigned int scopeIndex = currentScopeIndex;
    while (scopeIndex >= 0)
    {
        SymbolMapType::iterator iter = types[scopeIndex]->find(name);
        if (iter != types[scopeIndex]->end())
            return iter->second;
        if(scopeIndex == 0)
            return nullptr;
        else {
            scopeIndex = prevScopeMap.find(scopeIndex)->second;
        }
    }
    return nullptr;
}

bool SymbolTable::addLabel(Symbol *symbol)
{
    return true;
}

Symbol *SymbolTable::lookupLabel(const char *name)
{
    return nullptr;
}

void SymbolTable::printType(Symbol *sym)
{
    if (sym->symbolType >= BOOL && sym->symbolType <= STRING)
        fprintf(stderr, "%s", typeToString(sym->symbolType).c_str());
    else if (sym->symbolType == ARRAY)
    {
        fprintf(stderr, "ARRAY [%u] of ", sym->scalarSize);
        if (sym->elementType >= BOOL && sym->elementType <= STRING)
            fprintf(stderr, "%s", typeToString(sym->elementType).c_str());
        else
            printType(sym->elementTypePtr);
    }
    else if (sym->symbolType == RECORD)
    {
        fprintf(stderr, "RECORD ( ");
        for (size_t i = 0; i < sym->subSymbolList->size(); i++)
        {
            Symbol *symbol = (*sym->subSymbolList)[i];
            fprintf(stderr, "%s : ", symbol->name.c_str());
            printType(symbol);
            if (i == sym->subSymbolList->size() - 1)
                fprintf(stderr, ")");
            else
                fprintf(stderr, ", ");
        }
    }
}

void SymbolTable::print()
{
    int depth = 0;

    fprintf(stderr, "Types:\n----------------\n");
    for (int i = 0; i < (int)types.size(); ++i)
    {
        fprintf(stderr, "Scope %u: %s\n", i, scopeNames[i].c_str());
        SymbolMapType::iterator iter = types[i]->begin();
        while (iter != types[i]->end())
        {
            fprintf(stderr, "%*c", depth, ' ');
            fprintf(stderr, "%s -> ", iter->first.c_str());
            printType(iter->second);
            fprintf(stderr, "\n");
            ++iter;
        }
        fprintf(stderr, "----------------\n\n");
    }
    fprintf(stderr, "****************\n\n");

    depth = 0;
    fprintf(stderr, "Variables:\n----------------\n");
    for (int i = 0; i < (int)variables.size(); ++i)
    {
        fprintf(stderr, "Scope %u: %s\n", i, scopeNames[i].c_str());
        SymbolMapType &sm = *(variables[i]);
        SymbolMapType::iterator iter;
        for (iter = sm.begin(); iter != sm.end(); ++iter)
        {
            if (iter->second->symbolClass == VAR)
            {
                fprintf(stderr, "\t%s -> ", iter->second->name.c_str());
                printType(iter->second);
                fprintf(stderr, "\n");
            }
            else if (iter->second->symbolClass == CONST)
            {
                fprintf(stderr, "[CONST]\t%s -> ", iter->second->name.c_str());
                printType(iter->second);
                fprintf(stderr, "\n");
            }
        }
        fprintf(stderr, "----------------\n\n");
    }
    fprintf(stderr, "****************\n\n");

    fprintf(stderr, "Functions:\n----------------\n");
    for (int i = 0; i < (int)variables.size(); ++i)
    {
        fprintf(stderr, "Scope %u: %s\n", i, scopeNames[i].c_str());
        SymbolMapType::iterator iter = functions[i]->begin();
        while (iter != functions[i]->end())
        {
            fprintf(stderr, "%*c", depth, ' ');
            fprintf(stderr, "%s \tParam: (", iter->first.c_str());
            for (size_t i = 0; i < iter->second->subSymbolList->size(); i++)
            {
                Symbol *symbol = (*iter->second->subSymbolList)[i];
                fprintf(stderr, "%s : ", symbol->name.c_str());
                if (symbol->paraType)
                    fprintf(stderr, "var ");
                printType(symbol);
                if (i != iter->second->subSymbolList->size() - 1)
                    fprintf(stderr, ", ");
            }
            fprintf(stderr, ") ");
            if (iter->second->symbolType >= BOOL && iter->second->symbolType <= STRING)
            {
                fprintf(stderr, "Return: %s ", typeToString(iter->second->symbolType).c_str());
            }
            else if (iter->second->returnTypePtr != nullptr)
            {
                fprintf(stderr, "Return: ");
                printType(iter->second->returnTypePtr);
            }
            fprintf(stderr, "\n");
            ++iter;
        }
        fprintf(stderr, "----------------\n\n");
    }
    fprintf(stderr, "****************\n\n");
}