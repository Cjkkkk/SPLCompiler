#include "spl_symtab.hpp"

Symbol::Symbol(const std::string &name, SPL_CLASS symbolClass, SPL_TYPE symbolType)
    : name(name), symbolClass(symbolClass), symbolType(symbolType)
{
    this->constValuePtr = nullptr;
    this->elementType = SPL_TYPE::UNKNOWN;
    this->elementTypePtr = nullptr;
    this->subSymbolMap = nullptr;
    this->subSymbolList = nullptr;
    this->arraySize = 0;
    this->paraType = VALUE;
    this->parentScope = nullptr;
    this->returnTypePtr = nullptr;
}

SymbolTable::SymbolTable()
{
    pushScope("Global");

    //
}

SymbolTable::~SymbolTable()
{
    // Assert(variables.size() == 1, "spl.exe: error: mismatched scopes!");
    // popScope();
}

void SymbolTable::pushScope(const std::string &scopeName)
{
    SymbolMapType *sm;
    if (freeSymbolMaps.size() > 0)
    {
        sm = freeSymbolMaps.back();
        freeSymbolMaps.pop_back();
        sm->erase(sm->begin(), sm->end());
    }
    else
        sm = new SymbolMapType;

    variables.push_back(sm);
    scopeNames.push_back(scopeName);
}

void SymbolTable::popScope()
{
    Assert(variables.size() >= 1, "spl.exe: error: mismatched scopes!");
    freeSymbolMaps.push_back(variables.back());
    variables.pop_back();
}

bool SymbolTable::addVariable(Symbol *symbol)
{
    // Check to see if a symbol of the same name has already been declared.
    if (
        functions.find(symbol->name) != functions.end() ||
        variables.back()->find(symbol->name) != variables.back() ->end())
    {
        std::string errorMsg = "spl.exe: error: ignoring redeclaration of symbol \"" + symbol->name + "\".";
        Assert(0, errorMsg.c_str());
        return false;
    }
    // No matches, so go ahead and add it...
    (*variables.back())[symbol->name] = symbol;
    return true;
}

Symbol *SymbolTable::lookupVariable(const char *name)
{
    for (int i = (int)variables.size() - 1; i >= 0; --i)
    {
        SymbolMapType &sm = *(variables[i]);
        SymbolMapType::iterator iter = sm.find(name);
        if (iter != sm.end())
            return iter->second;
    }
    return nullptr;
}

bool SymbolTable::addFunction(Symbol *symbol)
{

    if (types.find(symbol->name) != types.end() ||
        functions.find(symbol->name) != functions.end() ||
        labels.find(symbol->name) != labels.end() ||
        variables[0]->find(symbol->name) != variables[0]->end())
    {
        std::string errorMsg = "spl.exe: error: ignoring redeclaration of symbol \"" + symbol->name + "\".";
        Assert(0, errorMsg.c_str());
        return false;
    }
    functions[symbol->name] = symbol;
    return true;
}

Symbol *SymbolTable::lookupFunction(const char *name)
{
    return nullptr;
}

bool SymbolTable::addType(Symbol *symbol)
{
    if (types.find(symbol->name) != types.end())
    {
        std::string errorMsg = "spl.exe: error: ignoring redeclaration of symbol \"" + symbol->name + "\".";
        Assert(0, errorMsg.c_str());
        return false;
    }
    types[symbol->name] = symbol;
    return true;
}

Symbol *SymbolTable::lookupType(const char *name)
{
    SymbolMapType::iterator iter = types.find(name);
    if (iter != types.end())
        return iter->second;
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
        fprintf(stderr, "ARRAY [%u] of ", sym->arraySize);
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
    SymbolMapType::iterator iter = types.begin();
    while (iter != types.end())
    {
        fprintf(stderr, "%*c", depth, ' ');
        fprintf(stderr, "%s -> ", iter->first.c_str());
        printType(iter->second);
        fprintf(stderr, "\n");
        ++iter;
    }
    fprintf(stderr, "----------------\n\n");

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
    }
    fprintf(stderr, "----------------\n\n");

    fprintf(stderr, "Functions:\n----------------\n");
    iter = functions.begin();
    while (iter != functions.end())
    {
        fprintf(stderr, "%*c", depth, ' ');
        fprintf(stderr, "%s \tParam: (", iter->first.c_str());
        for(size_t i = 0; i < iter->second->subSymbolList->size(); i++)
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