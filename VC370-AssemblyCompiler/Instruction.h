#pragma once

#include <string>

class Instruction
{
public:
	Instruction() {};
	~Instruction() {};

	enum class InstructionType {
		ST_ASSEMBLY,
		ST_MACHINE,
		ST_COMMENT,
		ST_END
	};

	// Parse the instruction into label, opcode, operand and returns
	// the type of instruction
	InstructionType ParseInstruction(const std::string &a_buff);

	// Gets the memory location of the next instruction
	int NextInstructionLocation(const int &a_loc);
	
	// Returns the label of the instruction
	std::string &GetLabel();

	// Returns true if the instruction has a label
	bool IsLabelBlank();
	
	// Checks if the instruction is valid
	bool IsInstructionValid();

private:
	void DivideInstruction(const std::string& a_buff);

	std::string m_label;
	std::string m_opCode;
	std::string m_operand;

	std::string m_instruction;
	InstructionType m_type;
	bool m_isNumericOperand;
	bool m_numericOperandValue;
};

