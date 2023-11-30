#include "Instruction.h"
#include "stdafx.h"
#include "Error.h"

/// <summary>
/// Parses the instruction and returns the type of instruction
/// </summary>
/// <param name="a_buff">The current line that is in the buffer</param>
/// <returns>Returns the instruction type</returns>
/// <author>Hristo Denev</author>
/// <date>11/15/2023</date>
Instruction::InstructionType Instruction::ParseInstruction(const string& a_buff)
{
    string line = RemoveComment(a_buff);

    // If after the comments are removed, the line is empty, then line is not an instruction of any kind
    if (line.find_first_not_of(" \t\n\r\f\v") == string::npos)
        return InstructionType::ST_COMMENT_OR_BLANK;

    DivideInstruction(line);

    //TODO: Used boost::to_upper_copy to make the opCode case insensitive
    if (m_opCode == "END")
        return InstructionType::ST_END;
    if (isAssemblyCode())
        return InstructionType::ST_ASSEMBLY;
    if (isMachineCode())
        return InstructionType::ST_MACHINE;

    return InstructionType::ST_ERROR;
}


/// <summary>
/// Gets the memory location of the next instruction
/// </summary>
/// <param name="a_loc">The current location of the instruction</param>
/// <returns>The memory location of the next instruction</returns>
/// <author>Hristo Denev</author>
/// <date>11/10/2023</date>
int Instruction::NextInstructionLocation(const int& a_loc)
{
    return a_loc + 1;
}

/// <summary>
/// Returns the label of the instruction
/// </summary>
/// <returns>Returns the label of the instruction</returns>
/// <author>Hristo Denev</author>
/// <date>11/10/2023</date>
std::string& Instruction::GetLabel()
{
    return m_label;
}

/// <summary>
/// Returns the opCode of the instruction
/// </summary>
/// <returns>Returns the opCode of the instruction</returns>
/// <author>Hristo Denev</author>
/// <date>11/10/2023</date>
std::string& Instruction::GetOpCode()
{
    return m_opCode;
}

/// <summary>
/// Returns the operand of the instruction
/// </summary>
/// <returns>Returns the operand of the instruction</returns>
/// <author>Hristo Denev</author>
/// <date>11/10/2023</date>
std::string& Instruction::GetOperand()
{
    return m_operand;
}


/// <summary>
/// Returns the numeric value of the operand
/// </summary>
/// <returns>Returns the numeric value of the operand</returns>
/// 
/// <author>Hristo Denev</author>
/// <date>11/17/2023</date>
int Instruction::GetNumericOpCodeValue()
{
    // Convert the operand to a numeric value
    if (m_opCode == "ADD") {
        m_numericOperandValue = 1;
    }
    else if (m_opCode == "SUB") {
        m_numericOperandValue = 2;
    }
    else if (m_opCode == "MULT") {
        m_numericOperandValue = 3;
    }
    else if (m_opCode == "DIV") {
        m_numericOperandValue = 4;
    }
    else if (m_opCode == "LOAD") {
        m_numericOperandValue = 5;
    }
    else if (m_opCode == "STORE") {
        m_numericOperandValue = 6;
    }
    else if (m_opCode == "READ") {
        m_numericOperandValue = 7;
    }
    else if (m_opCode == "WRITE") {
        m_numericOperandValue = 8;
    }
    else if (m_opCode == "B") {
        m_numericOperandValue = 9;
    }
    else if (m_opCode == "BM") {
        m_numericOperandValue = 10;
    }
    else if (m_opCode == "BZ") {
        m_numericOperandValue = 11;
    }
    else if (m_opCode == "BP") {
        m_numericOperandValue = 12;
    }
    else if (m_opCode == "HALT") {
        m_numericOperandValue = 13;
    }
    else {
        m_numericOperandValue = -1;
    }

    return m_numericOperandValue;
}

/// <summary>
/// Checks if the instruction's label is blank
/// </summary>
/// <returns>Returns true if the instruction doesn't have a label</returns>
/// <author>Hristo Denev</author>
/// <date>11/10/2023</date>
bool Instruction::IsLabelBlank()
{
    if (m_label.empty())
        return true;

    return false;
}

/// <summary>
/// Returns true if the instruction's extra field is blank
/// </summary>
/// <returns> Returns true if the instruction's extra field is blank</returns>
/// <author>Hristo Denev</author>
/// <date>11/28/2023</date>
bool Instruction::IsExtraBlank()
{
    if (m_extra.empty())
        return true;

    return false;
}

/// <summary>
/// Returns true if the instruction's operand is blank
/// </summary>
/// <returns> Returns true if the instruction's operand is blank</returns>
/// <author>Hristo Denev</author>
/// <date>11/28/2023</date>
bool Instruction::IsOperandBlank()
{
    if (m_operand.empty())
        return true;

    return false;
}

/// <summary>
/// Returns true if the instruction's operand is numeric
/// </summary>
/// <returns> Returns true if the instruction's operand is numeric</returns>
/// <author>Hristo Denev</author>
/// <date>11/29/2023</date>
bool Instruction::IsOperandNumeric()
{
    for (int i = 0; i < m_operand.length(); i++)
    {
        if (!isdigit(m_operand[i]))
            return false;
    }

    return true;
}

/// <summary>
/// Divides the instruction into label, opCode and operand
/// </summary>
/// <param name="a_buff">The current line that is in the buffer</param>
/// <returns> nothing </returns>
/// <author>Hristo Denev</author>
/// <date>11/10/2023</date>
void Instruction::DivideInstruction(const std::string& a_buff)
{
    istringstream inst(a_buff);

    m_extra = m_label = m_opCode = m_operand = "";

    if (a_buff.empty())
        return;

    string extra;

    if (a_buff[0] != ' ' && a_buff[0] != '\t')
    {
        inst >> m_label;
    }

    inst >> m_opCode >> m_operand >> m_extra;
    toUpper(m_opCode);

    return;
}

/// <summary>
/// Removes the comment from the instruction
/// </summary>
/// <param name="a_buff">The current line that is in the buffer</param>
/// <returns>Nothing</returns>
/// <author>Hristo Denev</author>
/// <date>11/17/2023</date>
std::string Instruction::RemoveComment(const std::string& a_buff)
{
    size_t pos = a_buff.find(';');

    if (pos != string::npos)
        return a_buff.substr(0, pos);

    return a_buff;
}

void Instruction::toUpper(string& a_str)
{
    for (int i = 0; i < a_str.length(); i++)
        a_str[i] = toupper(a_str[i]);
}

/// <summary>
/// Checks if the instruction is an assembly instruction
/// </summary>
/// <returns>If the instruction is an assembly instruction, then return true; false otherwise</returns>
/// <author>Hristo Denev</author>
/// <date>11/10/2023</date>
bool Instruction::isAssemblyCode()
{
    if (find(begin(AssemblyLangInstructions), end(AssemblyLangInstructions), m_opCode) != end(AssemblyLangInstructions))
        return true;

    return false;
}

/// <summary>
/// Checks if the instruction is a machine language instruction
/// </summary>
/// <returns>If the instruction is a machine language instruction, then return true; false otherwise</returns>
/// <author>Hristo Denev</author>
/// <date>11/10/2023</date>
bool Instruction::isMachineCode()
{
    if (find(begin(MachineLangInstructions), end(MachineLangInstructions), m_opCode) != end(MachineLangInstructions))
        return true;

    return false;
}
