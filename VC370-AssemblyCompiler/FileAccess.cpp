#include "FileAccess.h"
#include "stdafx.h"

/// <summary>
/// Constructor for the file access class.
/// It checks if the argument count is exactly 2.
/// If not, it prints an error message and terminates the program.
/// If it is, it opens the file.
/// Then checks if the file is correctly opened.
/// If not, it prints an error message and terminates the program.
/// </summary>
/// <param name="argc">Number of program arguments</param>
/// <param name="argv">Program arguments</param>
/// <author>Hristo Denev</author>
/// <date>11/05/2023</date>
FileAccess::FileAccess(int& argc, char* argv[])
{
    // Check that there is exactly one run time parameter.
    if (argc != 2) {
        cerr << "Usage: Assem <FileName>" << endl;
        exit(1);
    }

    // Open the file.  One might question if this is the best place to open the file.
    // One might also question whether we need a file access class.
    m_sfile.open(argv[1], ios::in);

    // If the open failed, report the error and terminate.
    if (!m_sfile) {
        cerr << "Source file could not be opened, assembler terminated."
            << endl;
        exit(1);
    }
}

/// <summary>
/// Destructor that closes the file
/// </summary>
/// <author>Hristo Denev</author>
/// <date>11/05/2023</date>
FileAccess::~FileAccess()
{
	m_sfile.close();
}

/// <summary>
/// Bool function that checks if the end of the file is reached.
/// If not, it reads the next line and returns true.
/// If yes, it returns false.
/// </summary>
/// <param name="a_buff">Buffer that is going to store the next line if there is one</param>
/// <returns>
/// Returns true if there is another line.
/// Returns false if the end of the file is reached.
/// </returns>
/// <author>Hristo Denev</author>
/// <date>11/05/2023</date>
bool FileAccess::GetNextLine(std::string& a_buff)
{
    // If there is no more data, return false
	if (m_sfile.eof())
	{
		return false;
	}
	
    // Read the next line since the end of the file is not reached
	getline(m_sfile, a_buff);
	return true;
}

/// <summary>
/// Rewinds the file to the beginning.
/// </summary>
/// <author>Hristo Denev</author>
/// <date>11/05/2023</date>
void FileAccess::Rewind()
{
    // Clean the file and set the pointer to the beginning
	m_sfile.clear();
	m_sfile.seekg(0, std::ios::beg);
}
