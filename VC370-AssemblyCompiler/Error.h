//
// Class to manage error reporting. Note: all members are static so we can access them anywhere.
// What other choices do we have to accomplish the same thing?
//
#ifndef _ERROR_H
#define _ERROR_H

#include <string>
#include <string_view>
#include <vector>

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

        constexpr ErrorMsg(ErrorCode emsg, int loc) noexcept : m_emsg(emsg), m_loc(loc) { }
    };

    Error() { InitErrorReporting(); }

    // Initializes error reports
    static void InitErrorReporting();

    // Records an error message
    static void RecordError(const ErrorMsg& a_emsg);

    // Returns true if there were any error messages recorded
    [[nodiscard]] static bool WasThereErrors() noexcept;

    // Displays the collected error message.
    static void DisplayErrors();

    // Get error message string for an error code
    [[nodiscard]] static constexpr std::string_view GetErrorString(ErrorCode code) noexcept {
        switch (code) {
            case ErrorCode::ERR_INVALID_OPCODE: return "Invalid opcode";
            case ErrorCode::ERR_OPERAND_OVERFLOW: return "Operand overflow";
            case ErrorCode::ERR_DUPLICATE_LABEL: return "Duplicate label";
            case ErrorCode::ERR_END_STATEMENT_NOT_LAST: return "END statement not last";
            case ErrorCode::ERR_EXTRA_ELEMENTS: return "Extra elements on line";
            case ErrorCode::ERR_MISSING_OPERAND: return "Missing operand";
            case ErrorCode::ERR_MISSING_END_STATEMENT: return "Missing END statement";
            case ErrorCode::ERR_SYNTAX_ERROR: return "Syntax error";
            case ErrorCode::ERR_UNDEFINED_LABEL: return "Undefined label operand";
            case ErrorCode::ERR_INVALID_OPERAND: return "Invalid operand";
            case ErrorCode::ERR_MEMORY_OVERFLOW: return "Memory overflow";
            case ErrorCode::ERR_MACHINE_CODE_AFTER_HALT: return "Machine code after HALT";
            case ErrorCode::ERR_ASSEMBLY_CODE_BEFORE_HALT: return "Assembly code before HALT";
            case ErrorCode::ERR_INVALID_LABEL: return "Invalid label";
            default: return "Unknown error";
        }
    }

private:
    // List of error messages
    static std::vector<ErrorMsg> m_ErrorMsgs;
    // Variable to keep track of whether there were any error messages
    static bool m_WasErrorMessages;
};
#endif