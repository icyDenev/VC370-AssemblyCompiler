//
//		Assembler class.  This is a container for all the components
//		that make up the assembler.
//
#pragma once 

#include "SymbolTable.h"
#include "Instruction.h"
#include "FileAccess.h"
#include "Emulator.h"
#include "stdafx.h"


class Assembler {

public:
    Assembler(int argc, char* argv[]);
    ~Assembler() = default;

    // Prevent copying
    Assembler(const Assembler&) = delete;
    Assembler& operator=(const Assembler&) = delete;

    // Pass I - establish the locations of the symbols
    void PassI();

    // Pass II - generate a translation
    void PassII();

    // Display the symbols in the symbol table.
    void DisplaySymbolTable() const { m_symTab.DisplaySymbolTable(); }

    // Run emulator on the translation.
    void RunProgramInEmulator() { m_emul.RunProgram(); }

private:

    FileAccess m_fileAcc;	    // File Access object
    SymbolTable m_symTab;	    // Symbol table object
    Instruction m_inst;	        // Instruction object
    Emulator m_emul;            // Emulator object
};
