#include "Assembler.h"
#include "stdafx.h"
#include "Error.h"

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

	cout << "Translation of Program:" << endl;
	cout << "Location  Contents       Original Statement" << endl;

	// Loop that reads every line and finds the location of each label
	while (1) {
		string line; // Read the next line from the source file
		int contents; // The contents of the line (opcode and operand)

		// Check if there are more lines to read
		// If not, pass II is completed
		if (!m_fileAcc.GetNextLine(line)) {
			Error::RecordError(Error::ErrorMsg(Error::ErrorCode::ERR_MISSING_END_STATEMENT, loc));

			cout << "____________________________________________\n\n";
			system("pause");
			cout << endl;
			return;
		}

		Instruction::InstructionType st = m_inst.ParseInstruction(line);

		// If the instruction is an error, then we can skip it
		if (st == Instruction::InstructionType::ST_ERROR) {
			m_emul.InsertMemory(loc, -1);
			continue;
		}

		// If the instruction is an END command, then Pass II is completed
		if (st == Instruction::InstructionType::ST_END) {
			cout << setw(20) << "" << "     " << line << endl;

			cout << "____________________________________________\n\n";
			system("pause");
			cout << endl;
			return;
		}

		// If the instruction is a comment or blank, then we can skip it
		if (st == Instruction::InstructionType::ST_COMMENT_OR_BLANK) {
			cout << setw(20) << "" << "     " << line << endl;
			continue;
		}

		// If the instruction is a machine instruction, then we can output the instruction accordingly
		if (st == Instruction::InstructionType::ST_MACHINE) {
			// If the instruction is a machine instruction, then we need to check if the operand is a valid label
			if (m_symTab.GetSymbolLocation(m_inst.GetOperand()) == -1) {
				// TODO: Report Error
				m_emul.InsertMemory(loc, -1);
				continue;
			}

			// If the label is valid, then we can put it in the memory output the instruction accordingly
			m_emul.InsertMemory(loc, m_inst.GetNumericOperandValue() * 10000 + m_symTab.GetSymbolLocation(m_inst.GetOperand()));
			cout << setw(10) << loc << setw(10) << m_emul.GetMemoryContent(loc) << "     " << line << endl;
		}
		// If it is not anything of the above types of instruction, then it is an assembly instruction
		else
		{
			// If the instruction is an ORG or DS command
			// then we have to update the location and output the instruction accordingly
			if (m_inst.GetOpCode() == "ORG") {
				loc = m_inst.NextInstructionLocation(stoi(m_inst.GetOperand()) - 1);

				cout << setw(10) << loc << setw(15) << "" << line << endl; // Output the location and the instruction
				
				continue;
			}
			else if (m_inst.GetOpCode() == "DS") {
				loc = m_inst.NextInstructionLocation(loc + stoi(m_inst.GetOperand()) - 1);
				
				cout << setw(10) << loc << setw(15) << "" << line << endl; // Output the location and the instruction
				
				continue;
			}
			// If the Assembly Instruction is not an ORG, DS or END command, then it is the DC command
			// We don't need to odify the location in a special way, but we need to output the instruction accordingly
			else {
				m_emul.InsertMemory(loc, 00 + stoi(m_inst.GetOperand()));
				
				cout << setw(10) << loc << setw(10) << m_emul.GetMemoryContent(loc) << "     " << line << endl;
			}
		}
		
		// If the instruction is not an ORG or DS command, then we can update the location by going to loc + 1
		loc = m_inst.NextInstructionLocation(loc);
	}
}