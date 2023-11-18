#include "Emulator.h"
#include "stdafx.h"

Emulator::Emulator()
{
	// TODO: Add your implementation code here.
}

bool Emulator::insertMemory(int a_location, int a_contents)
{
	m_memory[a_location] = a_contents;
	
	return true;
}

string Emulator::getMemoryContent(int a_location)
{
	return to_string(m_memory[a_location] / 100000)
		+ to_string(m_memory[a_location] / 10000 % 10)
		+ to_string(m_memory[a_location] / 1000 % 10)
		+ to_string(m_memory[a_location] / 100 % 10)
		+ to_string(m_memory[a_location] / 10 % 10)
		+ to_string(m_memory[a_location] % 10);
}

bool Emulator::runProgram()
{
	// TODO: Add your implementation code here.

	return false;
}
