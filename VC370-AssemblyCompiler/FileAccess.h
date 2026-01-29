//
//		File access to source file.
//
#ifndef _FILEACCESS_H  // This is the way that multiple inclusions are defended against often used in UNIX
#define _FILEACCESS_H // We use pramas in Visual Studio.  See other include files

#include <fstream>
#include <string>
#include <string_view>
#include <cstdlib>

class FileAccess {

public:

    // Opens the file.
    FileAccess(int argc, char* argv[]);

    // Closes the file.
    ~FileAccess();

    // Prevent copying
    FileAccess(const FileAccess&) = delete;
    FileAccess& operator=(const FileAccess&) = delete;

    // Get the next line from the source file.
    [[nodiscard]] bool GetNextLine(std::string& a_buff);

    // Put the file pointer back to the beginning of the file.
    void Rewind();

private:
    // Source file object.
    std::ifstream m_sfile;
};
#endif
