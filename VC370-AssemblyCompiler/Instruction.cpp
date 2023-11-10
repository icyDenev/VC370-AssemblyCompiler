#include "Instruction.h"
#include "stdafx.h"
#include "Error.h"

Instruction::InstructionType Instruction::ParseInstruction(const std::string& a_buff)
{
    DivideInstruction(a_buff);

    // Identify the type of the instruction
    if (m_opCode.empty() || m_opCode[0] == ';')
        return InstructionType::ST_COMMENT_OR_BLANK;
    //TODO: Used boost::to_upper_copy to make the opCode case insensitive
    if (m_opCode == "END")
        return InstructionType::ST_END;
    if (isAssemblyCode())
        return InstructionType::ST_ASSEMBLY;
    if (isMachineCode())
        return InstructionType::ST_MACHINE;

    Error::RecordError(Error::ErrorCode::ERR_INVALID_OPCODE);
    
    return InstructionType::ST_COMMENT_OR_BLANK;
}

int Instruction::NextInstructionLocation(const int& a_loc)
{
    return 0;
}

std::string& Instruction::GetLabel()
{
    return m_label;
}

std::string& Instruction::GetOpCode()
{
    return m_opCode;
}

std::string& Instruction::GetOperand()
{
    return m_operand;
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

bool Instruction::isAssemblyCode()
{
    if (AssemblyLangInstructions->find(m_opCode) != string::npos)
        return true;

    return false;
}

bool Instruction::isMachineCode()
{
    if (MachineLangInstructions->find(m_opCode) != string::npos)
		return true;

    return false;
}
