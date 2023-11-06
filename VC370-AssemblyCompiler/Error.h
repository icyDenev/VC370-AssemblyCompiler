//
// Class to manage error reporting. Note: all members are static so we can access them anywhere.
// What other choices do we have to accomplish the same thing?
//
#ifndef _ERROR_H
#define _ERROR_H

#include <string>
#include <vector>
using namespace std;

class Error {

public:

    // Initializes error reports.
    static void InitErrorReporting();

    // Records an error message.
    static void RecordError(const string a_emsg);

    // Returns true if there were any error messages recorded.
    static bool WasThereErrors();

    // Displays the collected error message.
    static void DisplayErrors();

private:
    // List of error messages.
    static vector<string> m_ErrorMsgs;
    // Variable to keep track of whether there were any error messages.
    static bool m_WasErrorMessages;
};
#endif