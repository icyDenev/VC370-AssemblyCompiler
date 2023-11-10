#include "Assembly.h"
#include "stdafx.h"

Assembler::Assembler(int& argc, char* argv[]) : m_fileA                     cc(argc, argv) { }

Assembler::~Assembler() { }

/// <summary>
/// The Pass I function of the assembler that reads the source file
/// stores the labels in the symbol table as well as the location of each label
/// </summary>
/// <returns>nothing</returns>
/// <author>Hristo Denev</author>
/// <date>11/05/2023</date>
void Assembler::PassI() {
	int loc = 0; // Tracks the location of the instructions

	// Loop that reads every line and finds the location of each label
	while (1) {
		string line; // Read the next line from the source file

		// Check if there are more lines to read
		// If not, pass I is completed
		if (!m_fileAcc.GetNextLine(line)) return;

		Instruction::InstructionType st = m_inst.ParseInstruction(line);

		// If the instruction is an END command, then Pass I is completed
		if (st == Instruction::InstructionType::ST_END)
			return;
		
		// If the instruction is a comment or blank, then we can skip it
		if (st == Instruction::InstructionType::ST_COMMENT_OR_BLANK
			|| st == Instruction::InstructionType::ST_MACHINE)
			continue;

		// If the instruction is a label,
		// then we can add it to the symbol table
		if (!m_inst.IsLabelBlank()) {
			m_symTab.AddSymbol(m_inst.GetLabel(), loc);
		}

		if (m_inst.GetOpCode() == "ORG") {
			
		}

		// Move to the next memory location
		m_inst.NextInstructionLocation(loc);
	}
}