#pragma once

#include "stdafx.h"

class Instruction
{
public:
	Instruction() {}
	~Instruction() {}

	enum class InstructionType {
		ST_ASSEMBLY,
		ST_MACHINE,
		ST_COMMENT_OR_BLANK,
		ST_END,
		ST_ERROR
	};

	// Parse the instruction into label, opcode, operand and returns
	// the type of instruction
	InstructionType ParseInstruction(const std::string &a_buff);

	// Gets the memory location of the next instruction
	int NextInstructionLocation(const int &a_loc);
	
	// Returns the label of the instruction
	std::string &GetLabel();

	// Returns the op code of the instruction
	std::string &GetOpCode();

	// Returns the operand of the instruction
	std::string &GetOperand();

	// Returns the numeric value of the operand
	int GetNumericOperandValue();

	// Returns true if the instruction has a label
	bool IsLabelBlank();
	
	// Checks if the instruction is valid
	bool IsInstructionValid();

private:
	void DivideInstruction(const std::string& a_buff);

	bool isAssemblyCode();
	bool isMachineCode();

	std::string m_label;
	std::string m_opCode;
	std::string m_operand;

	std::string m_instruction;
	InstructionType m_type;

	bool m_numericOperandValue;

	string MachineLangInstructions[13] { "ADD", "SUB", "MULT", "DIV", "LOAD", "STORE", "READ", "WRITE", "B", "BM", "BZ", "BP", "HALT" };
	string AssemblyLangInstructions[4] { "DC", "DS", "ORG", "END" };
};

