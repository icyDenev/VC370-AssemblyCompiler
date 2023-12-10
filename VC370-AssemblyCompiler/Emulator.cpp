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
bool Emulator::InsertMemory(int a_location, int opCode, int operand)
{
	if (a_location > MEMSZ || a_location < 0) {
		Error::RecordError(Error::ErrorMsg(Error::ErrorCode::ERR_CONSTANT_OVERFLOW, a_location));

		return false;
	}

	// If the opCode is -1, the operand is invalid and, therefore, the memory location is empty
	if (opCode == -1) {
		m_memoryContents[a_location] = "??";
		opCode = 0;
	}
	else {
		m_memoryContents[a_location] = to_string(opCode / 10)
									 + to_string(opCode % 10);
	}

	// If the operand is -1, the operand is invalid and, therefore, the memory location is empty
	if (operand == -1) {
		m_memoryContents[a_location] += "????";
		operand = 0;
	}
	else {
		m_memoryContents[a_location] += to_string(operand / 1000)
									 + to_string((operand % 1000) / 100)
									 + to_string((operand % 100) / 10)
									 + to_string(operand % 10);
	}

	m_memory[a_location] = opCode * 10000 + operand;
	
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
	if (a_location > MEMSZ || a_location < 0) {
		Error::RecordError(Error::ErrorMsg(Error::ErrorCode::ERR_CONSTANT_OVERFLOW, a_location));

		return "?????";
	}

	return m_memoryContents[a_location];
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

		exit(-1);
	}

	int loc = 100;
	string line;
	m_accum = 0;

	while (loc < MEMSZ) {
		int opcode = m_memory[loc] / 10000;
		int operand = m_memory[loc] % 10000;

		switch (opcode)
		{
			case 1: // ADD
				m_accum += m_memory[operand];
				m_accum %= 1000000;
				loc++;
				break;
			case 2: // SUB
				m_accum -= m_memory[operand];
				m_accum %= 1000000;
				loc++;
				break;
			case 3: // MULT
				m_accum *= m_memory[operand];
				m_accum %= 1000000;
				loc++;
				break;
			case 4: // DIV
				m_accum /= m_memory[operand];
				m_accum %= 1000000;
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
				cin >> line;
				// If the input is not an integer, output an error and terminate
				if (!isInteger(line)) {
					cout << "Error: Invalid input" << endl;

					return false;
				}
				m_memory[operand] = stoi(line[0] == '-' ? line.substr(0, 7) : line.substr(0, 6));
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

/// <summary>
/// Checks if a string is an integer
/// </summary>
/// <param name="s"> The string to be checked</param>
/// <returns> Returns true if the string is an integer</returns>
/// <author>Hristo Denev</author>
/// <date>12/09/2023</date>
bool Emulator::isInteger(const string& s)
{
	for (int i = 0; i < s.length(); i++)
	{
		if (!isdigit(s[i]))
			return false;
	}

	return true;
}
