/**
* Sys tools - Tools.h
*
* @program		Global-Pay
* @author		АНКУ ХХК
* @copyright	Copyright (c) 2017 он, АНКУ ХХК.
* @version		0.1.0.0
* @date			2017.09.19
*/

#pragma once

#include <windows.h>
#include <string>

BOOL RunProcess(LPCTSTR, LPTSTR, DWORD&, DWORD dwWaitMilliseconds = INFINITE, DWORD dwCreationFlags = NORMAL_PRIORITY_CLASS | CREATE_NO_WINDOW);
void GetLastErrorMessage(LPTSTR);
std::string ComputerName();