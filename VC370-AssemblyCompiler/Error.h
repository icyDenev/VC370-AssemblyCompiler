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
    // Error codes
    enum class ErrorCode {
        ERR_INVALID_OPCODE,
        ERR_UNDEFINED_LABEL,
        ERR_DUPLICATE_LABEL,
        ERR_INVALID_LABEL,
        ERR_SYNTAX_ERROR,
        ERR_EXTRA_ELEMENTS,
        ERR_MISSING_OPERAND,
        ERR_MISSING_END_STATEMENT,
        ERR_END_STATEMENT_NOT_LAST,
        ERR_OPERAND_OVERFLOW,
        ERR_INVALID_OPERAND,
        ERR_MEMORY_OVERFLOW,
        ERR_MACHINE_CODE_AFTER_HALT,
        ERR_ASSEMBLY_CODE_BEFORE_HALT
    };

    // Structure to hold information
    struct ErrorMsg {
        ErrorCode m_emsg;
        int m_loc;

        ErrorMsg(ErrorCode emsg, int loc) : m_emsg(emsg), m_loc(loc) { }
    };

    Error() { InitErrorReporting(); }

    // Initializes error reports
    static void InitErrorReporting();

    // Records an error message
    static void RecordError(const ErrorMsg a_emsg);

    // Returns true if there were any error messages recorded
    static bool WasThereErrors();

    // Displays the collected error message.
    static void DisplayErrors();

private:
    // List of error messages
    static vector<ErrorMsg> m_ErrorMsgs;
    // Variable to keep track of whether there were any error messages
    static bool m_WasErrorMessages;
};
#endif