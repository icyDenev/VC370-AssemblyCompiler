#include "Assembler.h"
#include "stdafx.h"

Assembler::Assembler(int& argc, char* argv[]) : m_fileAcc(argc, argv) { }

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
		if (st == Instruction::InstructionType::ST_COMMENT_OR_BLANK)
			continue;

		// If the instruction is a label,
		// then we can add it to the symbol table
		if (!m_inst.IsLabelBlank()) {
			m_symTab.AddSymbol(m_inst.GetLabel(), loc);
		}

		if (m_inst.GetOpCode() == "ORG") {
			loc = m_inst.NextInstructionLocation(stoi(m_inst.GetOperand()) - 1);
		}
		else if (m_inst.GetOpCode() == "DS") {
			loc = m_inst.NextInstructionLocation(loc + stoi(m_inst.GetOperand()) - 1);
		}
		else {
			loc = m_inst.NextInstructionLocation(loc);
		}
	}
}

/// <summary>
/// The Pass II function of the assembler that reads the source file
/// and translates the assembly instructions into machine code
/// </summary>
/// <returns>nothing</returns>
/// <author>Hristo Denev</author>
/// <date>11/15/2023</date>
void Assembler::PassII() {
	m_fileAcc.Rewind();

	int loc = 0; // Tracks the location of the instructions

	// Loop that reads every line and finds the location of each label
	while (1) {
		string line; // Read the next line from the source file
		int contents; // The contents of the line (opcode and operand)

		// Check if there are more lines to read
		// If not, pass II is completed
		if (!m_fileAcc.GetNextLine(line)) {
			return;
		}

		Instruction::InstructionType st = m_inst.ParseInstruction(line);

		// If the instruction is an error, then we can skip it
		if (st == Instruction::InstructionType::ST_ERROR) {
			// Report Error
			m_emul.insertMemory(loc, -1);
			continue;
		}

		// If the instruction is an END command, then Pass II is completed
		if (st == Instruction::InstructionType::ST_END) {
			// TODO: Report error
			return;
		}

		// If the instruction is a comment or blank, then we can skip it
		if (st == Instruction::InstructionType::ST_COMMENT_OR_BLANK)
			continue;

		if (st == Instruction::InstructionType::ST_MACHINE) {
			// TODO: Implement the machine code translation
			if (!m_symTab.LookupSymbol(m_inst.GetOperand(), loc)) {
				// Report Error
				m_emul.insertMemory(loc, -1);
				continue;
			}

			m_emul.insertMemory(loc, m_inst.GetNumericOperandValue() * 10000 + stoi(m_inst.GetOperand()));
		}
		else // If it is not a machine instruction, then it is an assembly instruction
		{
			// If the instruction is an ORG or DS command, then we have to update the location accordingly
			if (m_inst.GetOpCode() == "ORG") {
				loc = m_inst.NextInstructionLocation(stoi(m_inst.GetOperand()) - 1);
				
				continue;
			}
			else if (m_inst.GetOpCode() == "DS") {
				loc = m_inst.NextInstructionLocation(loc + stoi(m_inst.GetOperand()) - 1);

				continue;
			}
			// If the Assembly Instruction is not an ORG, DS or END command, then it is the DC command
			else {
				m_emul.insertMemory(loc, 00 + stoi(m_inst.GetOperand()));
			}
		}
		
		// If the instruction is not an ORG or DS command, then we can update the location by going to loc + 1
		loc = m_inst.NextInstructionLocation(loc);
	}
}