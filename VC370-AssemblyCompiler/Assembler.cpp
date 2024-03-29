#include "Assembler.h"
#include "stdafx.h"
#include "Error.h"

/// <summary>
/// Constructor for the Assembler class.
/// </summary>
/// <param name="argc"> The number of arguments passed to the program </param>
/// <param name="argv"> The arguments passed to the program </param>
/// <returns>Nothing</returns>
/// <author>Hristo Denev</author>
/// <date>11/05/2023</date>
Assembler::Assembler(int& argc, char* argv[]) : m_fileAcc(argc, argv) { }

/// <summary>
/// Destructor for the Assembler class.
/// </summary>
/// <returns>Nothing</returns>
/// <author>Hristo Denev</author>
/// <date>11/05/2023</date>
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
			loc = m_inst.NextInstructionLocation(loc) % 10000;
			continue;
		}

		// If the instruction is an ORG or DS command, we need to process this in a special way
		if (m_inst.GetOpCode() == "ORG") {
			loc = m_inst.NextInstructionLocation(stoi(m_inst.GetOperand()) - 1) % 10000;
		}
		else if (m_inst.GetOpCode() == "DS") {
			loc = m_inst.NextInstructionLocation(loc + stoi(m_inst.GetOperand()) - 1) % 10000;
		}
		// If the instruction is neither, we need to move to the next location in the memory
		else {
			loc = m_inst.NextInstructionLocation(loc) % 10000;
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
	int currOpCode = 0; // Tracks the current opcode
	int currOperand = 0; // Tracks the current operand
	int tempLoc = -1; // Tracks the temporary location in case of ORG or DS commands

	vector<string> currErrors; // Tracks the current errors
	
	bool machineCodeFinishedFl = false; // Tracks if there we have received a HALT command (therefore, the machine code is finished)

	cout << "Translation of Program:" << endl;
	cout << "Location  Contents       Original Statement" << endl;

	// Loop that reads every line and finds the location of each label
	while (1) {
		string line; // Read the next line from the source file
		int contents; // The contents of the line (opcode and operand)

		#pragma region NextLine

		// Check if there are more lines to read
		// If not, pass II is completed, but there is no END statement, so we also report it as an error
		if (!m_fileAcc.GetNextLine(line)) {
			Error::RecordError(Error::ErrorMsg(Error::ErrorCode::ERR_MISSING_END_STATEMENT, loc));
			cout << "Error: Missing END statement" << endl;
			cout << "____________________________________________\n\n";
			system("pause");
			cout << endl;
			return;
		}
		#pragma endregion

		// Parse the line into an instruction with all its elements
		Instruction::InstructionType st = m_inst.ParseInstruction(line);
		
		// Reset the variables
		currOpCode = 0;
		currOperand = 0;
		currErrors.clear();
		tempLoc = -1;

		#pragma region NonMachineOrAssemblyInstruction
		// If the instruction cannot be identified, we report it as a syntax error
		if (st == Instruction::InstructionType::ST_ERROR) {
			currOpCode = -1;
			Error::RecordError(Error::ErrorMsg(Error::ErrorCode::ERR_INVALID_OPCODE, loc));
			currErrors.push_back("Error: Invalid opcode");
		}

		// If the instruction is an END command, then Pass II is completed
		if (st == Instruction::InstructionType::ST_END) {

			cout << setw(20) << "" << "     " << line << endl;

			// If there is an operand after the END statement, we record an error
			if (!m_inst.IsOperandBlank()) {
				currOperand = -1;
				m_emul.InsertMemory(loc, currOpCode, currOperand);
				Error::RecordError(Error::ErrorMsg(Error::ErrorCode::ERR_EXTRA_ELEMENTS, loc));
				currErrors.push_back("Error: Extra elements on line");
			}

			// If there are more lines after the END statement, we record an error
			if (m_fileAcc.GetNextLine(line)) {
				Error::RecordError(Error::ErrorMsg(Error::ErrorCode::ERR_END_STATEMENT_NOT_LAST, loc));
				currErrors.push_back("Error: END statement not last");
			}

			// We output the errors if there are any
			for (auto error : currErrors) {
				cout << error << endl;
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

		#pragma region ChecksForBothMachineAndAssemblyCode
		// If the instruction has a label, then we need to check if it is a duplicate label
		if (!m_inst.IsLabelBlank()) {
			// If the label is a duplicate, we can record an error
			if (!m_symTab.LookupSymbol(m_inst.GetLabel())) {
				Error::RecordError(Error::ErrorMsg(Error::ErrorCode::ERR_DUPLICATE_LABEL, loc));
				currErrors.push_back("Error: Duplicate label");
			}

			if (m_inst.GetLabel().size() > 10) {
				Error::RecordError(Error::ErrorMsg(Error::ErrorCode::ERR_INVALID_LABEL, loc));
				currErrors.push_back("Error: Invalid label");
			}
		}

		// If the instruction has extra elements, we can record an error
		if (!m_inst.IsExtraBlank()) {
			Error::RecordError(Error::ErrorMsg(Error::ErrorCode::ERR_EXTRA_ELEMENTS, loc));
			currErrors.push_back("Error: Extra elements on line");
		}
		#pragma endregion

		#pragma region MachineInstruction
		if (st == Instruction::InstructionType::ST_MACHINE) {
			currOpCode = m_inst.GetNumericOpCodeValue();
			
			// If there is a machine instruction after HALT command, we record an error
			if (machineCodeFinishedFl) {
				Error::RecordError(Error::ErrorMsg(Error::ErrorCode::ERR_MACHINE_CODE_AFTER_HALT, loc));
				currErrors.push_back("Error: Machine Code After HALT");
			}

			// If the machine instruction is a HALT, we need to check if there is an operand
			if (m_inst.GetNumericOpCodeValue() == 13) {
				// If the operand is not missing, then there are extra elements. Therefore, we report an error
				if (!m_inst.IsOperandBlank()) {
					currOperand = -1;
					Error::RecordError(Error::ErrorMsg(Error::ErrorCode::ERR_EXTRA_ELEMENTS, loc));
					currErrors.push_back("Error: Extra elements on line");
					continue;
				}

				// We set the machineCodeFinishedFl to true since we have received a HALT command
				machineCodeFinishedFl = true;
			}
			// If the machine instruction is not a HALT command, then we need to check the operand
			else {
				// If the there are no operands, we record an error
				if (m_inst.IsOperandBlank()) {
					currOperand = -1;
					Error::RecordError(Error::ErrorMsg(Error::ErrorCode::ERR_MISSING_OPERAND, loc));
					currErrors.push_back("Error: Missing operand");
				}

				// If the label is not valid, we record an error
				else if (!isalpha(m_inst.GetOperand()[0])) {
					currOperand = -1;
					Error::RecordError(Error::ErrorMsg(Error::ErrorCode::ERR_SYNTAX_ERROR, loc));
					currErrors.push_back("Error: Syntax Error");
				}

				else if (m_inst.GetOperand().size() > 10) {
					currOperand = -1;
					Error::RecordError(Error::ErrorMsg(Error::ErrorCode::ERR_INVALID_OPERAND, loc));
					currErrors.push_back("Error: Invalid operand");
				}

				// If the label is not defined, we record an error
				else if (!m_symTab.GetSymbolLocation(m_inst.GetOperand())) {
					currOperand = -1;
					Error::RecordError(Error::ErrorMsg(Error::ErrorCode::ERR_UNDEFINED_LABEL, loc));
					currErrors.push_back("Error: Undefined label operand");
				}

				// If the operand uses a multiply defined label, we record an error
				else if (m_symTab.GetSymbolLocation(m_inst.GetOperand()) == m_symTab.multiplyDefinedSymbol) {
					currOperand = -1;
					Error::RecordError(Error::ErrorMsg(Error::ErrorCode::ERR_INVALID_OPERAND, loc));
					currErrors.push_back("Error: Invalid operand");
				}

				// If the operand is valid, we can put it in currOperand
				else {
					currOperand = m_symTab.GetSymbolLocation(m_inst.GetOperand());
				}
			}
		}
		#pragma endregion

		#pragma region AssemblyInstruction
		else if (st == Instruction::InstructionType::ST_ASSEMBLY) {
			// If the instruction is an assembly instruction before HALT command and is not ORG, we record an error
			if (!machineCodeFinishedFl && m_inst.GetOpCode() != "ORG") {
				Error::RecordError(Error::ErrorMsg(Error::ErrorCode::ERR_ASSEMBLY_CODE_BEFORE_HALT, loc));
				currErrors.push_back("Error: Assembly code before HALT");
			}

			// If the operand is missing, we record an error
			if (m_inst.IsOperandBlank()) {
				currOperand = -1;
				Error::RecordError(Error::ErrorMsg(Error::ErrorCode::ERR_MISSING_OPERAND, loc));
				currErrors.push_back("Error: Missing operand");
			}

			// If the operand is not a number, we record an error
			else if (!m_inst.IsOperandNumeric()) {
				currOperand = -1;
				Error::RecordError(Error::ErrorMsg(Error::ErrorCode::ERR_SYNTAX_ERROR, loc));
				currErrors.push_back("Error: Syntax Error");
			}

			else if (m_inst.GetOperand().size() >= 10) {
				currOperand = -1;
				Error::RecordError(Error::ErrorMsg(Error::ErrorCode::ERR_OPERAND_OVERFLOW, loc));
				currErrors.push_back("Error: Operand overflow");
			}

			// If the operand is not a number within the limit, we record an error
			else if (stoi(m_inst.GetOperand()) >= 1000000 || stoi(m_inst.GetOperand()) < 0) {
				currOperand = -1;
				Error::RecordError(Error::ErrorMsg(Error::ErrorCode::ERR_OPERAND_OVERFLOW, loc));
				currErrors.push_back("Error: Operand overflow");
			}

			// If the operand is valid, we can put it in currOperand
			else {
				currOpCode = stoi(m_inst.GetOperand()) / 10000;		// If the operand is a number bigger than 10000, then we put the first two digits in currOpCode
				currOperand = stoi(m_inst.GetOperand()) % 10000;	// If the operand is a number bigger than 10000, then we put the last four digits in currOperand

				// If the instruction is an ORG or DS command (or not a DC command),
				// then we have to update the location in a special way
				if (m_inst.GetOpCode() != "DC") {
					tempLoc = currOperand;

					// If the instruction is DS command, we need to add the operand to the current location
					if (m_inst.GetOpCode() == "DS") {
						tempLoc += loc;
					}

					// If the location is not within the limit, we record an error
					if (tempLoc >= 10000 || tempLoc < 0) {
						currOpCode = -1;
						currOperand = -1;
						tempLoc = loc + 1; // We move to the next location in the memory
						Error::RecordError(Error::ErrorMsg(Error::ErrorCode::ERR_MEMORY_OVERFLOW, loc));
						currErrors.push_back("Error: Memory overflow");
					}
				}
			}
		}
		#pragma endregion

		#pragma region Output
		// If the instruction is not a ORG or DS command, we need to output and move to the next location in the memory
		if (tempLoc == -1) {
			m_emul.InsertMemory(loc, currOpCode, currOperand);
			cout << setw(10) << loc << setw(10) << m_emul.GetMemoryContent(loc) << "     " << line << endl;

			// We move to the next location in the memory
			loc = m_inst.NextInstructionLocation(loc);

			// If the location is not within the limit, we record an error
			if (loc >= 10000) {
				loc %= 10000;
				Error::RecordError(Error::ErrorMsg(Error::ErrorCode::ERR_MEMORY_OVERFLOW, loc));
				currErrors.push_back("Error: Memory overflow");
			}
		}
		// If the instruction is a ORG or DS command, we need to output and move to the location in the memory that was stored in tempLoc
		else {
			cout << setw(10) << loc << setw(15) << "" << line << endl; // Output the location and the instruction				

			loc = tempLoc % 10000; // We move to the location in the memory that was stored in tempLoc
		}
		
		// Output the errors if there are any
		for (auto error : currErrors) {
			cout << error << endl;
		}
		
		#pragma endregion
	}
}