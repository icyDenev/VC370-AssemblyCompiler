//
//		Emulator class - supports the emulation of VC370 programs
//
#ifndef _EMULATOR_H      // A previous way of preventing multiple inclusions.
#define _EMULATOR_H

class Emulator {

public:

	const static int MEMSZ = 10'000;	// The size of the memory of the VC370.
	// Default constructor.  Will set the accumulator to zero.
	Emulator();

	// Records instructions and data into VC370 memory.
	bool insertMemory(int a_location, int a_contents);

	// Runs the VC370 program recorded in memory.
	bool runProgram();
private:

	// The VC370 has 10,000 words of memory.  Each word contains 6 decimal
	int m_memory[MEMSZ];    
	// The accumulator for the VC370
	int m_accum;
};

#endif