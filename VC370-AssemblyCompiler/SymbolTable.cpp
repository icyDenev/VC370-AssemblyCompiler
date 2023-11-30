#include "SymbolTable.h"
#include "stdafx.h"

/// <summary>
/// Adds a symbol to the symbol table
/// </summary>
/// <param name="a_symbol">The symbol to be added</param>
/// <param name="a_loc">The location of the symbol</param>
/// <returns>nothing</returns>
/// <author>Hristo Denev</author>
/// <date>11/10/2023</date>
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

/// <summary>
/// Displays the symbol table
/// </summary>
/// <returns>nothing</returns>
/// <author>Hristo Denev</author>
/// <date>11/17/2023</date>
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

/// <summary>
/// Looks up a symbol in the symbol table
/// </summary>
/// <param name="a_symbol">The symbol to be looked up</param>
/// <param name="a_loc">The location of the symbol</param>
/// <returns>True if the symbol is found, false otherwise</returns>
/// <author>Hristo Denev</author>
/// <date>11/17/2023</date>
bool SymbolTable::LookupSymbol(string& a_symbol)
{
	if (m_symbolTable[a_symbol] != multiplyDefinedSymbol && m_symbolTable.find(a_symbol) != m_symbolTable.end()) {
		return true;
	}

	return false;
}

/// <summary>
/// Returns the location of a symbol in the symbol table if it exists
/// </summary>
/// <param name="a_symbol">The symbol to be looked up</param>
/// <returns>The location of the symbol if it exists, -1 otherwise</returns>
int SymbolTable::GetSymbolLocation(string& a_symbol)
{
	if (m_symbolTable.find(a_symbol) != m_symbolTable.end() || m_symbolTable[a_symbol] != multiplyDefinedSymbol) {
		return m_symbolTable[a_symbol];
	}

	return -1;
}
