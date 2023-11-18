#include "Emulator.h"
#include "stdafx.h"

/// <summary>
/// The default constructor for the Emulator class.
/// </summary>
Emulator::Emulator()
{
	// TODO: Add your implementation code here.
}

/// <summary>
/// Inserts a memory location and contents into the emulator's memory.
/// </summary>
/// <param name="a_location">The location of the memory</param>
/// <param name="a_contents">The contents of the memory</param>
/// <returns>True if the memory was inserted successfully</returns>
/// <author>Hristo Denev</author>
/// <date>11/17/2023</date>
bool Emulator::InsertMemory(int a_location, int a_contents)
{
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

	return false;
}
