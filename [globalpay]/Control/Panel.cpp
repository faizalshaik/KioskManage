/**
* Panel.cpp
*
* @program		Global-Pay
* @author		АНКУ ХХК
* @copyright	Copyright (c) 2017 он, АНКУ ХХК.
* @version		0.1.0.0
* @date			2017.09.19
*/

#include "panel.h"

using namespace globalpay;

CPanel::CPanel(void)
	: CWnd()
{
	SetClass(CPANELCLASS);
}

BOOL CPanel::Initialize(HINSTANCE hInst, HWND hParent, RECT rcRect, LPCTSTR szText, DWORD WINSTYLE)
{
	if(!Register(CS_DBLCLKS|CS_PARENTDC|CS_SAVEBITS, hInst))
		return FALSE;

	return Create(szText, WINSTYLE, rcRect, hParent, NULL, hInst, this);
}

BOOL CPanel::InitializeHollow(HINSTANCE hInst, HWND hParent, RECT rcRect, LPCTSTR szText, DWORD WINSTYLE)
{
	if(!Register(CS_DBLCLKS|CS_PARENTDC|CS_SAVEBITS, hInst))
		return FALSE;

	return Create(szText, WINSTYLE, rcRect, hParent, NULL, hInst, this);

}

BOOL CPanel::InitializeEx(DWORD dwExStyle, HINSTANCE hInst, HWND hParent, RECT rcArea, LPCTSTR szText, DWORD dwWinStyle)
{
	if(!Register(CS_DBLCLKS|CS_PARENTDC|CS_SAVEBITS, hInst))
		return FALSE;

	return CreateEx(dwExStyle, szText, dwWinStyle, rcArea, hParent, NULL, hInst, this);
}