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
    // Identify the type of the instruction
    if (a_buff.empty() || a_buff[0] == ';')
        return InstructionType::ST_COMMENT_OR_BLANK;

    string line = RemoveComment(a_buff);

    DivideInstruction(line);

    //TODO: Used boost::to_upper_copy to make the opCode case insensitive
    if (m_opCode == "END")
        return InstructionType::ST_END;
    if (isAssemblyCode())
        return InstructionType::ST_ASSEMBLY;
    if (isMachineCode())
        return InstructionType::ST_MACHINE;

    Error::RecordError(Error::ErrorMsg(Error::ErrorCode::ERR_INVALID_OPCODE, 0));
    
    return InstructionType::ST_ERROR;
}

int Instruction::NextInstructionLocation(const int& a_loc)
{
    return a_loc + 1;
}

/// <summary>
/// Returns the label of the instruction
/// </summary>
/// <returns>Returns the label of the instruction</returns>
/// <author>Hristo Denev</author>
/// <date>11/15/2023</date>
std::string& Instruction::GetLabel()
{
    return m_label;
}

/// <summary>
/// Returns the opCode of the instruction
/// </summary>
/// <returns> Returns the opCode of the instruction </returns>
/// <author>Hristo Denev</author>
/// <date>11/15/2023</date>
std::string& Instruction::GetOpCode()
{
    return m_opCode;
}

std::string& Instruction::GetOperand()
{
    return m_operand;
}


/// <summary>
/// Returns the numeric value of the operand
/// </summary>
/// <returns>Returns the numeric value of the operand</returns>
/// <author>Hristo Denev</author>
/// <date>11/17/2023</date>
int Instruction::GetNumericOperandValue()
{
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

    // If the opCode is not a machine language instruction, then return -1
    // Report error
    return m_numericOperandValue;
}

bool Instruction::IsLabelBlank()
{
    if (m_label.empty())
		return true;

    return false;
}

bool Instruction::IsInstructionValid()
{
    // TODO: Add checks for validity of the instruction

    return false;
}

void Instruction::DivideInstruction(const std::string& a_buff)
{
    istringstream inst(a_buff);

    m_label = m_opCode = m_operand = "";
    
    if (a_buff.empty())
        return;

    string extra;

    if (a_buff[0] != ' ' && a_buff[0] != '\t')
    {
        inst >> m_label;
    }

    inst >> m_opCode >> m_operand >> extra;

    return;
}

std::string Instruction::RemoveComment(const std::string& a_buff)
{
	size_t pos = a_buff.find(';');

	if (pos != string::npos)
		return a_buff.substr(0, pos);

    return a_buff;
}

bool Instruction::isAssemblyCode()
{
    if (find(begin(AssemblyLangInstructions), end(AssemblyLangInstructions), m_opCode) != end(AssemblyLangInstructions))
        return true;

    return false;
}

bool Instruction::isMachineCode()
{
    if (find(begin(MachineLangInstructions), end(MachineLangInstructions), m_opCode) != end(MachineLangInstructions))
		return true;

    return false;
}
