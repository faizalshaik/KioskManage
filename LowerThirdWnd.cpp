// original name : LowerThirdWnd.cpp
// more info @   : LowerThirdWnd.h

#include "stdafx.h"

#include "resource.h"

#include "lowerthirdwnd.h"

CLowerThirdWnd LowerThird;

void CLowerThirdWnd::MarqueeSetText(std::wstring szText)
{
	CallFunction((TCHAR*)GetExternalXML(FLASH_EXTERNAL_SETTEXT, GetArgumentXML(szText)).c_str());
}