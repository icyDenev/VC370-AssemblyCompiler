#pragma once

#include "stdafx.h"
#include <array>
#include <string_view>

class Instruction
{
public:
	Instruction() = default;
	~Instruction() = default;

	enum class InstructionType {
		ST_ASSEMBLY,
		ST_MACHINE,
		ST_COMMENT_OR_BLANK,
		ST_END,
		ST_ERROR
	};

	// Parse the instruction into label, opcode, operand and returns
	// the type of instruction
	[[nodiscard]] InstructionType ParseInstruction(std::string_view a_buff);

	// Gets the memory location of the next instruction
	[[nodiscard]] static constexpr int NextInstructionLocation(int a_loc) noexcept {
		return a_loc + 1;
	}
	
	// Returns the label of the instruction
	[[nodiscard]] const std::string& GetLabel() const noexcept { return m_label; }

	// Returns the op code of the instruction
	[[nodiscard]] const std::string& GetOpCode() const noexcept { return m_opCode; }

	// Returns the operand of the instruction
	[[nodiscard]] const std::string& GetOperand() const noexcept { return m_operand; }

	// Returns the numeric value of the operation code
	[[nodiscard]] int GetNumericOpCodeValue() const noexcept;

	// Returns true if the instruction has a label
	[[nodiscard]] bool IsLabelBlank() const noexcept { return m_label.empty(); }

	// Returns true if the instruction has extra elements
	[[nodiscard]] bool IsExtraBlank() const noexcept { return m_extra.empty(); }

	// Returns true if the instruction has an operand
	[[nodiscard]] bool IsOperandBlank() const noexcept { return m_operand.empty(); }

	// Returns true if the operand is numeric
	[[nodiscard]] bool IsOperandNumeric() const noexcept;

private:
	void DivideInstruction(std::string_view a_buff);
	[[nodiscard]] static std::string RemoveComment(std::string_view a_buff);
	static void toUpper(std::string& a_str) noexcept;

	[[nodiscard]] bool isAssemblyCode() const noexcept;
	[[nodiscard]] bool isMachineCode() const noexcept;

	std::string m_label;
	std::string m_opCode;
	std::string m_operand;
	std::string m_extra;

	std::string m_instruction;
	InstructionType m_type{};

	// Machine language instructions using std::array and string_view
	static constexpr std::array<std::string_view, 13> MachineLangInstructions = {
		"ADD", "SUB", "MULT", "DIV", "LOAD", "STORE", "READ", "WRITE", "B", "BM", "BZ", "BP", "HALT"
	};
	
	// Assembly language instructions using std::array and string_view
	static constexpr std::array<std::string_view, 4> AssemblyLangInstructions = {
		"DC", "DS", "ORG", "END"
	};
};

