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
		
		// If operand is not numeric or is missing, then these is an error and we can skip it
		if (!m_inst.IsOperandNumeric() || m_inst.IsOperandBlank())
		{
			loc = m_inst.NextInstructionLocation(loc);
			continue;
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
/// <date>11/29/2023</date>
void Assembler::PassII() {
	m_fileAcc.Rewind();
	Error::InitErrorReporting();

	int loc = 0; // Tracks the location of the instructions

	cout << "Translation of Program:" << endl;
	cout << "Location  Contents       Original Statement" << endl;

	// Loop that reads every line and finds the location of each label
	while (1) {
		string line; // Read the next line from the source file
		int contents; // The contents of the line (opcode and operand)

		#pragma region NextLine
		// Check if there are more lines to read
		// If not, pass II is completed
		if (!m_fileAcc.GetNextLine(line)) {
			m_emul.InsertMemory(loc, -1);
			Error::RecordError(Error::ErrorMsg(Error::ErrorCode::ERR_MISSING_END_STATEMENT, loc));
			cout << "Error: Missing END statement" << endl;

			cout << "____________________________________________\n\n";
			system("pause");
			cout << endl;
			return;
		}
		#pragma endregion

		Instruction::InstructionType st = m_inst.ParseInstruction(line);
		
		#pragma region NonMachineOrAssemblyInstruction
		// If the instruction is an error, then we can skip it
		if (st == Instruction::InstructionType::ST_ERROR) {
			m_emul.InsertMemory(loc, -1);
			cout << setw(10) << loc << setw(10) << m_emul.GetMemoryContent(loc) << "     " << line << endl;
			cout << "Error: Syntax error" << endl;
			Error::RecordError(Error::ErrorMsg(Error::ErrorCode::ERR_SYNTAX_ERROR, loc));
			continue;
		}

		// If the instruction is an END command, then Pass II is completed
		if (st == Instruction::InstructionType::ST_END) {
			cout << setw(20) << "" << "     " << line << endl;

			if (!m_inst.IsOperandBlank()) {
				m_emul.InsertMemory(loc, -1);
				Error::RecordError(Error::ErrorMsg(Error::ErrorCode::ERR_EXTRA_ELEMENTS, loc));
				cout << "Error: Extra elements on line" << endl;
			}

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
		#pragma endregion

		// If the instruction has a label, then we need to check if it is a duplicate label
		if (!m_inst.IsLabelBlank()) {
			// If the label is a duplicate, then we can record an error and output the instruction accordingly
			if (!m_symTab.LookupSymbol(m_inst.GetLabel())) {
				m_emul.InsertMemory(loc, -1);
				cout << setw(10) << loc << setw(10) << m_emul.GetMemoryContent(loc) << "     " << line << endl;
				Error::RecordError(Error::ErrorMsg(Error::ErrorCode::ERR_DUPLICATE_LABEL, loc));
				cout << "Error: Duplicate label" << endl;
				continue;
			}
		}

		// If the instruction has extra elements, then we can record an error and output the instruction accordingly
		if (!m_inst.IsExtraBlank()) {
			m_emul.InsertMemory(loc, -1);
			cout << setw(10) << loc << setw(10) << m_emul.GetMemoryContent(loc) << "     " << line << endl;
			Error::RecordError(Error::ErrorMsg(Error::ErrorCode::ERR_EXTRA_ELEMENTS, loc));
			cout << "Error: Extra elements on line" << endl;
			continue;
		}

		// If the instruction is a machine instruction, then we need to process it accordingly
		#pragma region MachineInstruction
		// If the instruction is a machine instruction, then we can output the instruction accordingly
		if (st == Instruction::InstructionType::ST_MACHINE) {
			// If the opcode is invalid, then we can record an error and output the instruction accordingly
			if (m_inst.GetNumericOpCodeValue() == -1) {
				m_emul.InsertMemory(loc, -1);
				cout << setw(10) << loc << setw(10) << m_emul.GetMemoryContent(loc) << "     " << line << endl;
				cout << "Error: Invalid opcode" << endl;
				Error::RecordError(Error::ErrorMsg(Error::ErrorCode::ERR_INVALID_OPCODE, loc));
				continue;
			}

			// If the machine instruction is a HALT, then we don't need to check the operand since there is none
			if (m_inst.GetNumericOpCodeValue() == 13) {
				// If the opcode is a HALT command and the operand is not missing, then there are extra elements
				// and, therefore, we can record an error and output the instruction accordingly
				if (!m_inst.IsOperandBlank()) {
					m_emul.InsertMemory(loc, -1);
					cout << setw(10) << loc << setw(10) << m_emul.GetMemoryContent(loc) << "     " << line << endl;
					Error::RecordError(Error::ErrorMsg(Error::ErrorCode::ERR_EXTRA_ELEMENTS, loc));
					cout << "Error: Extra elements on line" << endl;
					continue;
				}
			}
			// If the machine instruction is not a HALT command, then we need to check the operand
			else {
				// If the there are no operands, then we can record an error and output the instruction accordingly
				if (m_inst.IsOperandBlank()) {
					m_emul.InsertMemory(loc, -1);
					cout << setw(10) << loc << setw(10) << m_emul.GetMemoryContent(loc) << "     " << line << endl;
					Error::RecordError(Error::ErrorMsg(Error::ErrorCode::ERR_MISSING_OPERAND, loc));
					cout << "Error: Missing operand" << endl;
					continue;
				}

				// If the label is not valid, then we can record an error and output the instruction accordingly
				if (!isalpha(m_inst.GetOperand()[0])) {
					m_emul.InsertMemory(loc, -1);
					cout << setw(10) << loc << setw(10) << m_emul.GetMemoryContent(loc) << "     " << line << endl;
					Error::RecordError(Error::ErrorMsg(Error::ErrorCode::ERR_INVALID_OPERAND, loc));
					cout << "Error: Invalid operand" << endl;
					continue;
				}

				// If the label is not defined, then we can record an error and output the instruction accordingly
				if (!m_symTab.GetSymbolLocation(m_inst.GetOperand())) {
					m_emul.InsertMemory(loc, -1);
					cout << setw(10) << loc << setw(10) << m_emul.GetMemoryContent(loc) << "     " << line << endl;
					Error::RecordError(Error::ErrorMsg(Error::ErrorCode::ERR_UNDEFINED_LABEL, loc));
					cout << "Error: Undefined label" << endl;
					continue;
				}
			}

			// If the label and opcode are valid, then we can put it in the memory output the instruction accordingly
			m_emul.InsertMemory(loc, m_inst.GetNumericOpCodeValue() * 10000 + m_symTab.GetSymbolLocation(m_inst.GetOperand()));
			cout << setw(10) << loc << setw(10) << m_emul.GetMemoryContent(loc) << "     " << line << endl;
		}
		#pragma endregion

		// If it is not anything of the above types of instruction, then it is an assembly instruction
		#pragma region AssemblyInstruction
		else {
			// If the instruction is a non-end assembly instruction and the operand is missing, then we can record an error and output the instruction accordingly
			if (m_inst.IsOperandBlank()) {
				m_emul.InsertMemory(loc, -1);
				cout << setw(10) << loc << setw(10) << m_emul.GetMemoryContent(loc) << "     " << line << endl;
				Error::RecordError(Error::ErrorMsg(Error::ErrorCode::ERR_MISSING_OPERAND, loc));
				cout << "Error: Missing operand" << endl;
				continue;
			}

			// If the instruction is an ORG or DS command
			// then we have to update the location and output the instruction accordingly
			if (m_inst.GetOpCode() == "ORG") {
				int tempLoc;

				// If the operand is not a number, then we can record an error and output the instruction accordingly
				if (!m_inst.IsOperandNumeric()) {
					cout << setw(10) << "????" << setw(10) << m_emul.GetMemoryContent(loc) << "     " << line << endl;
					Error::RecordError(Error::ErrorMsg(Error::ErrorCode::ERR_INVALID_OPERAND, loc));
					cout << "Error: Invalid operand" << endl;
					continue;
				}

				tempLoc = m_inst.NextInstructionLocation(stoi(m_inst.GetOperand()) - 1);

				// If the location is not within the limit, then we can record an error and output the instruction accordingly
				if (tempLoc >= 10000 || tempLoc < 0) {
					cout << setw(10) << "????" << setw(10) << m_emul.GetMemoryContent(-1) << "     " << line << endl;
					Error::RecordError(Error::ErrorMsg(Error::ErrorCode::ERR_MEMORY_OVERFLOW, loc));
					cout << "Error: Memory overflow" << endl;
					continue;
				}

				cout << setw(10) << loc << setw(15) << "" << line << endl; // Output the location and the instruction				

				loc = tempLoc;

				continue;
			}
			if (m_inst.GetOpCode() == "DS") {
				int tempLoc;

				// If the operand is not a number, then we can record an error and output the instruction accordingly
				if (!m_inst.IsOperandNumeric()) {
					m_emul.InsertMemory(loc, -1);
					cout << setw(10) << loc << setw(10) << m_emul.GetMemoryContent(loc) << "     " << line << endl;
					Error::RecordError(Error::ErrorMsg(Error::ErrorCode::ERR_INVALID_OPERAND, loc));
					cout << "Error: Invalid operand" << endl;
					continue;
				}

				tempLoc = m_inst.NextInstructionLocation(loc + stoi(m_inst.GetOperand()) - 1);

				// If the location is not within the limit, then we can record an error and output the instruction accordingly
				if (tempLoc >= 10000 || tempLoc < 0) {
					cout << setw(10) << "????" << setw(10) << m_emul.GetMemoryContent(-1) << "     " << line << endl;
					Error::RecordError(Error::ErrorMsg(Error::ErrorCode::ERR_MEMORY_OVERFLOW, loc));
					cout << "Error: Memory overflow" << endl;
					continue;
				}


				cout << setw(10) << loc << setw(15) << "" << line << endl; // Output the location and the instruction
				loc = tempLoc;

				continue;
			}

			// If the Assembly Instruction is not an ORG, DS or END command, then it is the DC command
			// We don't need to odify the location in a special way, but we need to output the instruction accordingly

			// If the operand is not a number, then we can record an error and output the instruction accordingly
			if (!m_inst.IsOperandNumeric()) {
				m_emul.InsertMemory(loc, -1);
				cout << setw(10) << loc << setw(10) << m_emul.GetMemoryContent(loc) << "     " << line << endl;
				Error::RecordError(Error::ErrorMsg(Error::ErrorCode::ERR_INVALID_OPERAND, loc));
				cout << "Error: Invalid operand" << endl;
				continue;
			}

			// If the operand is not a number within the limit, then we can record an error and output the instruction accordingly
			if (stoi(m_inst.GetOperand()) >= 10000 || stoi(m_inst.GetOperand()) < 0) {
				m_emul.InsertMemory(loc, -1);
				cout << setw(10) << loc << setw(10) << m_emul.GetMemoryContent(loc) << "     " << line << endl;
				Error::RecordError(Error::ErrorMsg(Error::ErrorCode::ERR_CONSTANT_OVERFLOW, loc));
				cout << "Error: Operand overflow" << endl;
				continue;
			}

			m_emul.InsertMemory(loc, 00 + stoi(m_inst.GetOperand()));
			cout << setw(10) << loc << setw(10) << m_emul.GetMemoryContent(loc) << "     " << line << endl;
		}
		#pragma endregion
		
		// We move to the next location in the memory
		loc = m_inst.NextInstructionLocation(loc);
	}
}