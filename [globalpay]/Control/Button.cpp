/**
* StaticWnd.cpp
*
* @program		Global-Pay
* @author		АНКУ ХХК
* @copyright	Copyright (c) 2017 он, АНКУ ХХК.
* @version		0.1.0.0
* @date			2017.09.19
*/

#include "button.h"

using namespace globalpay;

CButton::CButton(void)
: CSystemControl()
{
	SetClass(CBUTTONCLASS);
}

bool CButton::Create(HINSTANCE hInst, HWND hParent, LPCTSTR szDefaultText, RECT rcRect, HFONT hFont, DWORD WINSTYLE, DWORD WINSTYLEEX, LPVOID lpParam)
{
	return CSystemControl::Create(hInst, hParent, szDefaultText, rcRect, hFont, WINSTYLE, WINSTYLEEX, lpParam);
}

void CButton::Check(BOOL bChecked)
{
	SendMessage(BM_SETCHECK, (WPARAM)(bChecked ? BST_CHECKED : BST_UNCHECKED), (LPARAM)(bChecked ? 1 : 0));
}

bool CButton::IsChecked()
{
	return (BST_CHECKED == SendMessage(BM_GETCHECK, 0, 0));
}

LPVOID CButton::OnParentCommand(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	if (HIWORD(wParam) == BN_CLICKED)
	{
		::EnumChildWindows(hWnd, EnumSiblings, (LPARAM)(UINT)LOWORD(wParam));
	}
	return 0L;
}

BOOL CButton::EnumSiblings(HWND hBtn, LPARAM lParam)
{
	if ((UINT)lParam == GetWindowID(hBtn))
	{
		CButton* btn = reinterpret_cast<CButton*>(GetUserDataLong(hBtn));
		if (btn->onClick != NULL)
			(*btn->onClick)(hBtn);

		return FALSE;
	}
	return TRUE;
}