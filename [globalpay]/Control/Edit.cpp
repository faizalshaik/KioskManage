/**
* Edit.cpp
*
* @program		Global-Pay
* @author		АНКУ ХХК
* @copyright	Copyright (c) 2017 он, АНКУ ХХК.
* @version		0.1.0.0
* @date			2017.09.19
*/

#include "edit.h"

using namespace globalpay;

CEdit::CEdit(void)
: CSystemControl()
{
	SetClass(CEDITCLASS);
}

bool CEdit::Create(HINSTANCE hInst, HWND hParent, LPCTSTR szDefaultText, RECT rcRect, HFONT hFont, DWORD WINSTYLE, DWORD WINSTYLEEX, LPVOID lpParam)
{
	return CSystemControl::Create(hInst, hParent, szDefaultText, rcRect, hFont, WINSTYLE, WINSTYLEEX, lpParam);
}

void CEdit::SetReadOnly(BOOL bSet)
{
	SendMessage(EM_SETREADONLY, bSet, NULL);
}

void CEdit::SetLimitText(int limit)
{
	SendMessage(EM_SETLIMITTEXT, (WPARAM) limit, (LPARAM) 0);
}