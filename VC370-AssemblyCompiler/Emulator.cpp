#include "Emulator.h"
#include "Error.h"
#include "stdafx.h"

Emulator::Emulator()
{
	// TODO: Add your implementation code here.
}

bool Emulator::insertMemory(int a_location, int a_contents)
{
	// TODO: Add your implementation code here.
	
	return false;
}

bool Emulator::runProgram()
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
