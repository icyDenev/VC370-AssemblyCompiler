//
//		Symbol table class.
//
#pragma once
#include "stdafx.h"
#include <unordered_map>
#include <string_view>

// This class is our symbol table.
class SymbolTable {

public:
    SymbolTable() = default;
    ~SymbolTable() = default;

    static constexpr int multiplyDefinedSymbol = -999;

    // Add a new symbol to the symbol table.
    void AddSymbol(const std::string& a_symbol, int a_loc);

    // Display the symbol table.
    void DisplaySymbolTable() const;

    // Lookup a symbol in the symbol table.
    [[nodiscard]] bool LookupSymbol(std::string_view a_symbol) const;

    // Get location of symbol in the symbol table.
    [[nodiscard]] int GetSymbolLocation(std::string_view a_symbol) const;

private:

    // This is the actual symbol table.  The symbol is the key to the map.
    // Using unordered_map for O(1) average lookup
    std::unordered_map<std::string, int> m_symbolTable;

};