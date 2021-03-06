// original name : SysHelper.cpp
// more info @   : SysHelper.h

#include "tools.h"

BOOL RunProcess(LPCTSTR lpAppPath, LPTSTR lpComLine, DWORD &exitCode, DWORD dwWaitMilliseconds, DWORD dwCreationFlags)
{
	PROCESS_INFORMATION processInfo = {0};
	STARTUPINFO startupInfo         = {0};
	startupInfo.cb                  = sizeof(startupInfo);

	BOOL result = ::CreateProcess(lpAppPath, lpComLine, NULL, NULL, FALSE, dwCreationFlags, NULL, NULL, &startupInfo, &processInfo);
	
	if (!result)
		return FALSE;
	else
	{
		::WaitForSingleObject(processInfo.hProcess, dwWaitMilliseconds);
		
		result = ::GetExitCodeProcess(processInfo.hProcess, &exitCode);
		
		::CloseHandle(processInfo.hProcess);
		::CloseHandle(processInfo.hThread);
		
		if (!result)
		{
		}
	}
	return TRUE;
}

void GetLastErrorMessage(LPTSTR lpErrorMessage)
{
	LPVOID lpMsgBuf;
	DWORD dw = ::GetLastError();
	::FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER|FORMAT_MESSAGE_FROM_SYSTEM|FORMAT_MESSAGE_IGNORE_INSERTS, NULL, dw, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR) &lpMsgBuf, 0, NULL);
	::lstrcpy(lpErrorMessage, (LPTSTR) lpMsgBuf);

	::LocalFree(lpMsgBuf);
}

std::string ComputerName()
{
	TCHAR buffer[(MAX_COMPUTERNAME_LENGTH + 1) * sizeof(TCHAR)];
	DWORD len = (MAX_COMPUTERNAME_LENGTH + 1) * sizeof(TCHAR);
	
	if (::GetComputerName(buffer, &len))
	{
		std::string str;
#ifndef UNICODE
		str = buffer;
#else
		std::wstring wStr = buffer;
		str = std::string(wStr.begin(), wStr.end());
#endif
		return str;
	}
	return "UNKNOWN";
}