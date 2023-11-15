#include "Error.h"

vector<Error::ErrorMsg> Error::m_ErrorMsgs = vector<ErrorMsg>();
bool Error::m_WasErrorMessages = false;

void Error::InitErrorReporting()
{
	m_ErrorMsgs = vector<ErrorMsg>();
	m_WasErrorMessages = false;
}

void Error::RecordError(const ErrorMsg a_emsg)
{
	m_ErrorMsgs.push_back(a_emsg);
	m_WasErrorMessages = true;
}

bool Error::WasThereErrors()
{
	return m_WasErrorMessages;
}

void Error::DisplayErrors()
{
}
