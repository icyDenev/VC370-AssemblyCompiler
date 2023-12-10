#include "Error.h"
#include "stdafx.h"

vector<Error::ErrorMsg> Error::m_ErrorMsgs = vector<ErrorMsg>();
bool Error::m_WasErrorMessages = false;

/// <summary>
/// Initializes the error reporting
/// by clearing the error messages and setting the
/// was error messages flag to false
/// </summary>
/// <returns>nothing</returns>
/// <author>Hristo Denev</author>
/// <date>11/27/2023</date>
void Error::InitErrorReporting()
{
	m_ErrorMsgs = vector<ErrorMsg>();
	m_WasErrorMessages = false;
}

/// <summary>
/// Records an error message
/// </summary>
/// <param name="a_emsg">The error message to be recorded</param>
/// <returns>nothing</returns>
/// <author>Hristo Denev</author>
/// <date>11/27/2023</date>
void Error::RecordError(const ErrorMsg a_emsg)
{
	m_ErrorMsgs.push_back(a_emsg);
	m_WasErrorMessages = true;
}

/// <summary>
/// Returns the error message at the specified index
/// </summary>
/// <returns>Returns the error message at the specified index</returns>
/// <author>Hristo Denev</author>
/// <date>11/27/2023</date>
bool Error::WasThereErrors()
{
	return m_WasErrorMessages;
}

/// <summary>
///	Displays the error messages
/// ordered by the location of the error
/// </summary>
/// <returns>nothing</returns>
/// <author>Hristo Denev</author>
/// <date>11/27/2023</date>
void Error::DisplayErrors()
{
	for (ErrorMsg error : m_ErrorMsgs)
	{
		cout << error.m_loc << " ";
		
		switch (error.m_emsg)
		{
			case ErrorCode::ERR_INVALID_OPCODE:
				cout << "Invalid opcode" << endl;
				break;
			case ErrorCode::ERR_CONSTANT_OVERFLOW:
				cout << "Constant overflow" << endl;
				break;
			case ErrorCode::ERR_DUPLICATE_LABEL:
				cout << "Duplicate label" << endl;
				break;
			case ErrorCode::ERR_END_STATEMENT_NOT_LAST:
				cout << "END statement not last" << endl;
				break;
			case ErrorCode::ERR_EXTRA_ELEMENTS:
				cout << "Extra elements on line" << endl;
				break;
			case ErrorCode::ERR_MISSING_OPERAND:
				cout << "Missing operand" << endl;
				break;
			case ErrorCode::ERR_MISSING_END_STATEMENT:
				cout << "Missing END statement" << endl;
				break;
			case ErrorCode::ERR_SYNTAX_ERROR:
				cout << "Syntax error" << endl;
				break;
			case ErrorCode::ERR_UNDEFINED_LABEL:
				cout << "Undefined label operand" << endl;
				break;
			case ErrorCode::ERR_INVALID_OPERAND:
				cout << "Invalid operand" << endl;
				break;
			case ErrorCode::ERR_MEMORY_OVERFLOW:
				cout << "Memory overflow" << endl;
				break;
			case ErrorCode::ERR_MACHINE_CODE_AFTER_HALT:
				cout << "Machine code after HALT" << endl;
				break;
			case ErrorCode::ERR_ASSEMBLY_CODE_BEFORE_HALT:
				cout << "Assembly code before HALT" << endl;
				break;
			case ErrorCode::ERR_INVALID_LABEL:
				cout << "Invalid label" << endl;
				break;
			default:
				break;
		}
	}
}
