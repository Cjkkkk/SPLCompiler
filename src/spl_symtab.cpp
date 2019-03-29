#include "spl_symtab.hpp"

Symbol::Symbol(const std::string &name, SPL_CLASS symbolClass, SPL_TYPE symbolType, Symbol *symbolTypePtr)
    : name(name), symbolClass(symbolClass), symbolType(symbolType)
{
    //this->symbolTypePtr = nullptr;
    this->constValue = nullptr;
    this->memberType = SPL_TYPE::UNKNOWN;
    this->memberTypePtr = nullptr;
    this->memberList = nullptr;
    this->arraySize = 0;
    this->passMode = SPL_PASSMODE::VALUE;
    this->parentScope = nullptr;
    this->returnType = SPL_TYPE::UNKNOWN;
    this->returnTpyePtr = nullptr;
}

SymbolTable::SymbolTable()
{
    pushScope();

    //
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

// void printSymbol(Symbol *sym)
// {
//     if (sym->symbolType >= BOOL && sym->symbolType <= STRING)
//         fprintf(stderr, "%s ", typeToString(sym->symbolType).c_str());
//     else if (sym->symbolType == ARRAY)
//     {
//     }
// }

void SymbolTable::print()
{
    int depth = 0;

    fprintf(stderr, "Types:\n---------------\n");
    SymbolMapType::iterator iter = types.begin();
    while (iter != types.end())
    {
        fprintf(stderr, "%*c", depth, ' ');
        Symbol *sym = iter->second;
        if ((sym->symbolType >= BOOL && sym->symbolType <= STRING) || sym->symbolType == RECORD)
            fprintf(stderr, "%s -> %s\n", iter->first.c_str(), typeToString(sym->symbolType).c_str());
        else if (sym->symbolType == ARRAY)
        {
            fprintf(stderr, "%s -> %s [", iter->first.c_str(), typeToString(sym->symbolType).c_str());
            fprintf(stderr, "%u] of ", sym->arraySize);
            Symbol *p = sym;
            while (p != nullptr)
            {
                if (p->memberType >= BOOL && p->memberType <= STRING)
                    fprintf(stderr, "%s ", typeToString(p->memberType).c_str());
                else if (p->memberType == ARRAY)
                {
                    fprintf(stderr, "%s [%u] of ", typeToString(p->memberType).c_str(), p->memberTypePtr->arraySize);
                }
                p = p->memberTypePtr;
            }
            fprintf(stderr, "\n");
        }
        ++iter;
    }
    fprintf(stderr, "----------------\n\n");

    depth = 0;
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
    fprintf(stderr, "----------------\n\n");
}