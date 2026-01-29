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
FileAccess::FileAccess(int argc, char* argv[])
{
    // Check that there is exactly one run time parameter.
    if (argc != 2) {
        std::cerr << "Usage: Assem <FileName>\n";
        std::exit(1);
    }

    // Open the file.
    m_sfile.open(argv[1], std::ios::in);

    // If the open failed, report the error and terminate.
    if (!m_sfile) {
        std::cerr << "Source file could not be opened, assembler terminated.\n";
        std::exit(1);
    }
}

/// <summary>
/// Destructor that closes the file
/// </summary>
FileAccess::~FileAccess()
{
    // ifstream automatically closes in destructor, but explicit close is fine
    if (m_sfile.is_open()) {
        m_sfile.close();
    }
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
bool FileAccess::GetNextLine(std::string& a_buff)
{
	// Read and return in one step; if there is no more data, the getline will fail and return false
    return static_cast<bool>(std::getline(m_sfile, a_buff));
}

/// <summary>
/// Rewinds the file to the beginning.
/// </summary>
void FileAccess::Rewind()
{
    // Clean the file and set the pointer to the beginning
    m_sfile.clear();
    m_sfile.seekg(0, std::ios::beg);
}
