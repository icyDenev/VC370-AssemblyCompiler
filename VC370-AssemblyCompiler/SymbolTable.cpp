#include "SymbolTable.h"
#include "stdafx.h"

/// <summary>
/// Adds a symbol to the symbol table
/// </summary>
/// <param name="a_symbol">The symbol to be added</param>
/// <param name="a_loc">The location of the symbol</param>
void SymbolTable::AddSymbol(const std::string& a_symbol, int a_loc)
{
	// If the symbol is already in the symbol table, record it as multiply defined.
	if (auto result = m_symbolTable.find(a_symbol); result != m_symbolTable.end()) {
		result->second = multiplyDefinedSymbol;
		return;
	}

	// Record a the location in the symbol table.
	m_symbolTable[a_symbol] = a_loc;
}

/// <summary>
/// Displays the symbol table
/// </summary>
void SymbolTable::DisplaySymbolTable() const
{
	std::cout << "Symbol Table:\n";
	std::cout << "Symbol #    Symbol    Location\n";
	int i = 0;

	for (const auto& [symbol, location] : m_symbolTable) {
		std::cout << std::format(" {:<12}{:<10}{:<10}\n", i++, symbol, location);
	}

	std::cout << "____________________________________________\n\n";
	system("pause");
	std::cout << '\n';
}

/// <summary>
/// Looks up a symbol in the symbol table
/// </summary>
/// <param name="a_symbol">The symbol to be looked up</param>
/// <returns>True if the symbol is found and not multiply defined, false otherwise</returns>
bool SymbolTable::LookupSymbol(std::string_view a_symbol) const
{
	const std::string symbolStr{a_symbol};
	if (auto it = m_symbolTable.find(symbolStr); it != m_symbolTable.end()) {
		return it->second != multiplyDefinedSymbol;
	}
	return false;
}

/// <summary>
/// Returns the location of a symbol in the symbol table if it exists
/// </summary>
/// <param name="a_symbol">The symbol to be looked up</param>
/// <returns>The location of the symbol if it exists, 0 otherwise</returns>
int SymbolTable::GetSymbolLocation(std::string_view a_symbol) const
{
	const std::string symbolStr{a_symbol};
	if (auto it = m_symbolTable.find(symbolStr); it != m_symbolTable.end()) {
		return it->second;
	}
	return 0;
}
