/*
 * Assembler main program.
 */
#include "stdafx.h"     // This must be present if you use precompiled headers which you will use. 
#include <stdio.h>
#include <string>

#include "Assembler.h"

int main(int argc, char* argv[])
{
#define TEST
#ifdef TEST
    argc = 2;
    const char* args[2] { "AssemblerTest", "C:\\Users\\icyde\\source\\repos\\CMPS361\\VC370-AssemblyCompiler\\VC370-AssemblyCompiler\\x64\\Release\\test.asm"};

    argv = const_cast<char**>(args);

#endif
    Assembler assem(argc, argv);

    // Establish the location of the labels:
    assem.PassI();

    // Display the symbol table.
    assem.DisplaySymbolTable();

    // Output the symbol table and the translation.
    assem.PassII( );

    // Run the emulator on the Quack3200 program that was generated in Pass II.
    assem.RunProgramInEmulator();

    // Terminate indicating all is well.  If there is an unrecoverable error, the 
    // program will terminate at the point that it occurred with an exit(1) call.
    return 0;
}