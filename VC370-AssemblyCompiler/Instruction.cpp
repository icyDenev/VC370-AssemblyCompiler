#include "Instruction.h"
#include "stdafx.h"

Instruction::InstructionType Instruction::ParseInstruction(const std::string& a_buff)
{
    DivideInstruction(a_buff);

    if (m_opCode.empty() || m_opCode[0] == ';')
        return InstructionType::ST_COMMENT;
    if (boost::to_upper_copy(m_opCode) == "END")
        return InstructionType::ST_END;

    return Instruction::InstructionType::ST_COMMENT;
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
