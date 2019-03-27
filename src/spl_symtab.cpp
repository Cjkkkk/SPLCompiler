#include "spl_symtab.hpp"

Symbol::Symbol(const std::string& name, SPL_CLASS symbolClass, SPL_TYPE symbolType, Symbol* symbolTypePtr)
	: name(name), symbolClass(symbolClass), symbolType(symbolType)
{
	this->symbolTypePtr = nullptr;
	this->constValue = nullptr;
	this->parentFunction = nullptr;
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
	SymbolMapType* sm;
	if (freeSymbolMaps.size() > 0) {
		sm = freeSymbolMaps.back();
		freeSymbolMaps.pop_back();
		sm->erase(sm->begin(), sm->end());
	}
	else
		sm = new SymbolMapType;

	variables.push_back(sm);
}

void SymbolTable::popScope() {
	Assert(variables.size() > 1, "spl.exe: error: mismatched scopes!");
	freeSymbolMaps.push_back(variables.back());
	variables.pop_back();
}

bool SymbolTable::addVaraible()
{
	return true;
}

Symbol* SymbolTable::lookupVariable(const char* name)
{
	return nullptr;
}

bool SymbolTable::addFunction()
{
	return true;
}

Symbol* SymbolTable::lookupFunction(const char* name)
{
	return nullptr;
}

bool SymbolTable::addType()
{
	return true;
}

Symbol* SymbolTable::lookupType(const char* name)
{
	return nullptr;
}

bool SymbolTable::addLabel()
{
	return true;
}

Symbol* SymbolTable::lookupLabel(const char* name)
{
	return nullptr;
}