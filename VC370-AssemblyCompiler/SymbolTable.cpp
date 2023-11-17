#include "SymbolTable.h"
#include "stdafx.h"

void SymbolTable::AddSymbol(string& a_symbol, int a_loc)
{
	// If the symbol is already in the symbol table, record it as multiply defined.
	auto result = m_symbolTable.find(a_symbol);
	
	if (result != m_symbolTable.end()) {
		result->second = multiplyDefinedSymbol;
		return;
	}

	// Record a the location in the symbol table.
	m_symbolTable[a_symbol] = a_loc;
}

void SymbolTable::DisplaySymbolTable()
{
	for (auto item : m_symbolTable) {
		cout << item.first << " " << item.second << endl;
	}
}

bool SymbolTable::LookupSymbol(string& a_symbol, int& a_loc)
{
	if (m_symbolTable[a_symbol] == a_loc && m_symbolTable.find(a_symbol) != m_symbolTable.end()) {
		return true;
	}

	return false;
}
