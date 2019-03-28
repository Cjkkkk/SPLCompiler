#include "spl_symtab.hpp"

Symbol::Symbol(const std::string &name, SPL_CLASS symbolClass, SPL_TYPE symbolType, Symbol *symbolTypePtr)
    : name(name), symbolClass(symbolClass), symbolType(symbolType)
{
    this->symbolTypePtr = nullptr;
    this->constValue = nullptr;
    this->memberList = nullptr;
    this->passMode = SPL_PASSMODE::VALUE;
    this->parentScope = nullptr;
    this->returnType = SPL_TYPE::UNKNOWN;
    this->returnTpyePtr = nullptr;
}

SymbolTable::SymbolTable()
{
    pushScope();
}

SymbolTable::~SymbolTable()
{
    Assert(variables.size() == 1, "spl.exe: error: mismatched scopes!");
    popScope();
}

void SymbolTable::pushScope()
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
}

void SymbolTable::popScope()
{
    Assert(variables.size() >= 1, "spl.exe: error: mismatched scopes!");
    freeSymbolMaps.push_back(variables.back());
    variables.pop_back();
}

bool SymbolTable::addVaraible(Symbol *symbol)
{
    // Check to see if a symbol of the same name has already been declared.
    for (int i = (int)variables.size() - 1; i >= 0; --i)
    {
        SymbolMapType &sm = *(variables[i]);
        if (sm.find(symbol->name) != sm.end())
        {
            if (i == (int)variables.size() - 1)
            {
                // If a symbol of the same name was declared in the
                // same scope, it's an error.
                std::string errorMsg = "spl.exe: error: ignoring redeclaration of symbol \"" + symbol->name + "\".";
                Assert(0, errorMsg.c_str());
                return false;
            }
            else
            {
                // Otherwise it's just shadowing something else, which
                // is legal but dangerous..
                (*variables.back())[symbol->name] = symbol;
                return true;
            }
        }
    }

    // No matches, so go ahead and add it...
    (*variables.back())[symbol->name] = symbol;
    return true;
}

Symbol *SymbolTable::lookupVariable(const char *name)
{
    for (int i = (int)variables.size() - 1; i >= 0; --i) {
        SymbolMapType& sm = *(variables[i]);
        SymbolMapType::iterator iter = sm.find(name);
        if (iter != sm.end())
            return iter->second;
    }
    std::string errorMsg = "spl.exe: error: identifier \"" + std::string(name) + "\"  is undefined.";
    Assert(0, errorMsg.c_str());
    return nullptr;
}

bool SymbolTable::addFunction(Symbol *symbol)
{
    return true;
}

Symbol *SymbolTable::lookupFunction(const char *name)
{
    return nullptr;
}

bool SymbolTable::addType(Symbol *symbol)
{
    return true;
}

Symbol *SymbolTable::lookupType(const char *name)
{
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

void SymbolTable::print()
{
    int depth = 0;
    fprintf(stderr, "Variables:\n----------------\n");
    for (int i = 0; i < (int)variables.size(); ++i)
    {
        SymbolMapType &sm = *(variables[i]);
        SymbolMapType::iterator iter;
        for (iter = sm.begin(); iter != sm.end(); ++iter)
        {
            fprintf(stderr, "%*c", depth, ' ');
            Symbol *sym = iter->second;
            fprintf(stderr, "%s [%s]", sym->name.c_str(), typeToString(sym->symbolType).c_str());
        }
        fprintf(stderr, "\n");
        depth += 4;
    }
}