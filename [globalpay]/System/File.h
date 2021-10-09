/**
* File tools - File.h
*
* @program		Global-Pay
* @author		АНКУ ХХК
* @copyright	Copyright (c) 2017 он, АНКУ ХХК.
* @version		0.1.0.0
* @date			2017.09.19
*/

#pragma once

#include <windows.h>
#include <tchar.h>
#include <stdio.h>

#include "../types/string.h"

BOOL GetModuleDirectory(LPTSTR, HMODULE hModule = NULL);
BOOL GetCurrentDirectory(LPTSTR, DWORD dwBUFFERSIZE = MAX_PATH);
BOOL ResExtractToFile(TCHAR*, TCHAR*, HINSTANCE, bool, TCHAR*);