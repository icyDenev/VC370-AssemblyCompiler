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
	cout << "Symbol Table:" << endl;
	cout << "Symbol #    Symbol    Location" << endl;
	int i = 0;

	for (auto item : m_symbolTable) {
		cout << " " << setw(12) << left << i++ << setw(10) << left << item.first << setw(10) << left << item.second << endl;
	}

	cout << "____________________________________________\n\n";
	system("pause");
	cout << endl;
}

bool SymbolTable::LookupSymbol(string& a_symbol, int& a_loc)
{
	if (m_symbolTable[a_symbol] == a_loc && m_symbolTable.find(a_symbol) != m_symbolTable.end()) {
		return true;
	}

	return false;
}

int SymbolTable::GetSymbolLocation(string& a_symbol)
{
	if (m_symbolTable.find(a_symbol) != m_symbolTable.end() || m_symbolTable[a_symbol] != multiplyDefinedSymbol) {
		return m_symbolTable[a_symbol];
	}

	return -1;
}
