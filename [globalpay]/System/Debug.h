/**
* TRACE macro for non-mfc debuggings - Debug.h
*
* @program		Global-Pay
* @author		АНКУ ХХК
* @copyright	Copyright (c) 2017 он, АНКУ ХХК.
* @version		0.1.0.0
* @date			2017.09.19
*/

#pragma once

#include <windows.h>

#ifndef TRACE
#define TRACE TraceDebug
void TraceDebug(LPCTSTR);
#endif