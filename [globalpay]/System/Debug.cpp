// original name : Debug.cpp
// more info @   : Debug.h

#include "debug.h"

void TraceDebug(LPCTSTR szStrToOutput)
{
#ifdef _DEBUG
	::OutputDebugString(szStrToOutput);
	::OutputDebugString(TEXT("\n"));
#endif
}