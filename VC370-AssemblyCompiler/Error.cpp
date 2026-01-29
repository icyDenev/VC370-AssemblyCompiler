#include "Error.h"
#include "stdafx.h"

std::vector<Error::ErrorMsg> Error::m_ErrorMsgs;
bool Error::m_WasErrorMessages = false;

/// <summary>
/// Initializes the error reporting
/// by clearing the error messages and setting the
/// was error messages flag to false
/// </summary>
void Error::InitErrorReporting()
{
	m_ErrorMsgs.clear();
	m_WasErrorMessages = false;
}

/// <summary>
/// Records an error message
/// </summary>
/// <param name="a_emsg">The error message to be recorded</param>
void Error::RecordError(const ErrorMsg& a_emsg)
{
	m_ErrorMsgs.push_back(a_emsg);
	m_WasErrorMessages = true;
}

/// <summary>
/// Returns the error message at the specified index
/// </summary>
/// <returns>Returns the error message at the specified index</returns>
bool Error::WasThereErrors() noexcept
{
	return m_WasErrorMessages;
}

/// <summary>
///	Displays the error messages
/// ordered by the location of the error
/// </summary>
void Error::DisplayErrors()
{
	for (const auto& error : m_ErrorMsgs)
	{
		std::cout << std::format("{} {}\n", error.m_loc, GetErrorString(error.m_emsg));
	}
}
