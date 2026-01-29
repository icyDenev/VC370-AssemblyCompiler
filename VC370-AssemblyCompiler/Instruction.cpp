#include "Instruction.h"
#include "stdafx.h"
#include "Error.h"
#include <algorithm>
#include <ranges>
#include <cctype>

/// <summary>
/// Parses the instruction and returns the type of instruction
/// </summary>
/// <param name="a_buff">The current line that is in the buffer</param>
/// <returns>Returns the instruction type</returns>
Instruction::InstructionType Instruction::ParseInstruction(std::string_view a_buff)
{
    // Remove the comments from the instruction
    std::string line = RemoveComment(a_buff);

    // If after the comments are removed, the line is empty, then line is not an instruction of any kind
    if (line.find_first_not_of(" \t\n\r\f\v") == std::string::npos)
        return InstructionType::ST_COMMENT_OR_BLANK;
    
    // Divide the instruction into label, opCode and operand
    DivideInstruction(line);

    if (m_opCode == "END")
        return InstructionType::ST_END;
    if (isAssemblyCode())
        return InstructionType::ST_ASSEMBLY;
    if (isMachineCode())
        return InstructionType::ST_MACHINE;

    return InstructionType::ST_ERROR;
}

/// <summary>
/// Returns the numeric value of the operand
/// </summary>
/// <returns>Returns the numeric value of the operand</returns>
int Instruction::GetNumericOpCodeValue() const noexcept
{
    // Use a map for cleaner lookup
    static const std::unordered_map<std::string_view, int> opcodeMap = {
        {"ADD", 1}, {"SUB", 2}, {"MULT", 3}, {"DIV", 4},
        {"LOAD", 5}, {"STORE", 6}, {"READ", 7}, {"WRITE", 8},
        {"B", 9}, {"BM", 10}, {"BZ", 11}, {"BP", 12}, {"HALT", 13}
    };

    if (auto it = opcodeMap.find(m_opCode); it != opcodeMap.end()) {
        return it->second;
    }
    return -1;
}

/// <summary>
/// Returns true if the instruction's operand is numeric
/// </summary>
/// <returns>Returns true if the instruction's operand is numeric</returns>
bool Instruction::IsOperandNumeric() const noexcept
{
    if (m_operand.empty()) return false;
    return std::ranges::all_of(m_operand, [](unsigned char c) { return std::isdigit(c); });
}

/// <summary>
/// Divides the instruction into label, opCode and operand
/// </summary>
/// <param name="a_buff">The current line that is in the buffer</param>
void Instruction::DivideInstruction(std::string_view a_buff)
{
    std::istringstream inst{std::string(a_buff)};

    m_extra.clear();
    m_label.clear();
    m_opCode.clear();
    m_operand.clear();

    // If the buffer is empty, there is nothing to divide
    if (a_buff.empty())
        return;

    // If the first character is not a space or a tab, then the line has a label
    if (a_buff[0] != ' ' && a_buff[0] != '\t')
    {
        inst >> m_label;
    }

    inst >> m_opCode >> m_operand >> m_extra;

    // We make the opCode uppercase to make it case insensitive
    toUpper(m_opCode);
}

/// <summary>
/// Removes the comment from the instruction
/// </summary>
/// <param name="a_buff">The current line that is in the buffer</param>
/// <returns>String without comment</returns>
std::string Instruction::RemoveComment(std::string_view a_buff)
{
    if (const auto pos = a_buff.find(';'); pos != std::string_view::npos)
        return std::string(a_buff.substr(0, pos));

    return std::string(a_buff);
}

/// <summary>
/// Makes all the characters in the string uppercase
/// </summary>
/// <param name="a_str">The string to be made uppercase</param>
void Instruction::toUpper(std::string& a_str) noexcept
{
    std::ranges::transform(a_str, a_str.begin(), 
        [](unsigned char c) { return static_cast<char>(std::toupper(c)); });
}

/// <summary>
/// Checks if the instruction is an assembly instruction
/// </summary>
/// <returns>If the instruction is an assembly instruction, then return true; false otherwise</returns>
bool Instruction::isAssemblyCode() const noexcept
{
    return std::ranges::find(AssemblyLangInstructions, m_opCode) != AssemblyLangInstructions.end();
}

/// <summary>
/// Checks if the instruction is a machine language instruction
/// </summary>
/// <returns>If the instruction is a machine language instruction, then return true; false otherwise</returns>
bool Instruction::isMachineCode() const noexcept
{
    return std::ranges::find(MachineLangInstructions, m_opCode) != MachineLangInstructions.end();
}
