#include "Emulator.h"
#include "Error.h"
#include "stdafx.h"

/// <summary>
/// The default constructor for the Emulator class.
/// </summary>
/// <author>Hristo Denev</author>
/// <date>11/19/2023</date>
Emulator::Emulator()
{
	for (int i = 0; i < MEMSZ; i++) {
		m_memory[i] = 0;
	}
	
	m_accum = 0;
}

/// <summary>
/// Inserts a memory location and contents into the emulator's memory.
/// </summary>
/// <param name="a_location">The location of the memory</param>
/// <param name="a_contents">The contents of the memory</param>
/// <returns>True if the memory was inserted successfully</returns>
/// <author>Hristo Denev</author>
/// <date>11/19/2023</date>
bool Emulator::InsertMemory(int a_location, int a_contents)
{
	if (a_location > MEMSZ || a_location < 0) {
		Error::RecordError(Error::ErrorMsg(Error::ErrorCode::ERR_CONSTANT_OVERFLOW, a_location));

		return false;
	}

	m_memory[a_location] = a_contents;
	
	return true;
}

/// <summary>
/// Returns the contents of the memory location specified by a_location.
/// </summary>
/// <param name="a_location">The location of the memory</param>
/// <returns>Returns the contents of the memory location specified by a_location</returns>
/// <author>Hristo Denev</author>
/// <date>11/17/2023</date>
string Emulator::GetMemoryContent(int a_location)
{
	return to_string(m_memory[a_location] / 100000)
		+ to_string(m_memory[a_location] / 10000 % 10)
		+ to_string(m_memory[a_location] / 1000 % 10)
		+ to_string(m_memory[a_location] / 100 % 10)
		+ to_string(m_memory[a_location] / 10 % 10)
		+ to_string(m_memory[a_location] % 10);
}

/// <summary>
/// The function that runs the VC370 program recorded in memory.
/// </summary>
/// <returns>Return true if the program was run successfully</returns>
/// <author>Hristo Denev</author>
/// <date>NOT IMPLEMENTED YET</date>
bool Emulator::RunProgram()
{
	// TODO: Add your implementation code here.
	if (Error::WasThereErrors()) {
		Error::DisplayErrors();

		return false;
	}

	int loc = 100;
	m_accum = 0;

	while (loc < MEMSZ) {
		int opcode = m_memory[loc] / 10000;
		int operand = m_memory[loc] % 10000;

		switch (opcode)
		{
			case 1: // ADD
				m_accum += m_memory[operand];
				loc++;
				break;
			case 2: // SUB
				m_accum -= m_memory[operand];
				loc++;
				break;
			case 3: // MULT
				m_accum *= m_memory[operand];
				loc++;
				break;
			case 4: // DIV
				m_accum /= m_memory[operand];
				loc++;
				break;
			case 5: // LOAD
				m_accum = m_memory[operand];
				loc++;
				break;
			case 6: // STORE
				m_memory[operand] = m_accum;
				loc++;
				break;
			case 7: // READ
				cout << "? ";
				cin >> m_memory[operand];
				loc++;
				break;
			case 8: // WRITE
				cout << m_memory[operand] << endl;
				loc++;
				break;
			case 9: // BRANCH
				loc = operand;
				continue;
			case 10: // BRANCH MINUS
				if (m_accum < 0) {
					loc = operand;
					continue;
				}
				loc++;
				break;
			case 11: // BRANCH ZERO
				if (m_accum == 0) {
					loc = operand;
					continue;
				}
				loc++;
				break;
			case 12: // BRANCH PLUS
				if (m_accum > 0) {
					loc = operand;
					continue;
				}
				loc++;
				break;
			case 13: // HALT
				return true;
			
		}
	}

	return false;
}
