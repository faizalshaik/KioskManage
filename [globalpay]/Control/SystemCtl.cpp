/**
* SystemCtl.cpp
*
* @program		Global-Pay
* @author		АНКУ ХХК
* @copyright	Copyright (c) 2017 он, АНКУ ХХК.
* @version		0.1.0.0
* @date			2017.09.19
*/

#include "systemctl.h"

using namespace globalpay;

UINT_PTR CSystemControl::CLASS_PTR = 0;

CSystemControl::CSystemControl(void)
: CWndDef()
{
}

CSystemControl::~CSystemControl(void)
{
	::RemoveWindowSubclass(GetHandle(), reinterpret_cast<SUBCLASSPROC>(CSystemControl::ControlProc), CLASS_PTR);
}

LRESULT CSystemControl::ControlProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData)
{
	CSystemControl* thisControl = reinterpret_cast<CSystemControl*>(dwRefData);
	if (thisControl)
	{
		BOOL   isReallyProcessed;
		LPVOID vpResult = thisControl->MessageHandler(hWnd, uMsg, wParam, lParam, &isReallyProcessed);
		if (isReallyProcessed)
			return reinterpret_cast<LRESULT>(vpResult);
	}
	return ::DefSubclassProc(hWnd, uMsg, wParam, lParam);
}

bool CSystemControl::Create(HINSTANCE hInst, HWND hParent, LPCTSTR szDefaultText, RECT rcRect, HFONT hFont, DWORD WINSTYLE, DWORD WINSTYLEEX, LPVOID lpParam)
{
	m_hWnd = ::CreateWindowEx(WINSTYLEEX, GetClass(), szDefaultText, WINSTYLE,
							  rcRect.left, rcRect.top, coord::width(rcRect), coord::height(rcRect),
						      hParent, reinterpret_cast<HMENU>(CWndDef::CHILD_ID), hInst, lpParam);
	
	if (!GetHandle())
		return false;
	
	CWndDef::CHILD_ID++;
	::SetWindowLongPtr(GetHandle(), GWLP_USERDATA, reinterpret_cast<LONG_PTR> (lpParam != NULL ? lpParam : this));
	::SetWindowSubclass(GetHandle(), reinterpret_cast<SUBCLASSPROC>(ControlProc), CLASS_PTR, reinterpret_cast<DWORD_PTR>(lpParam != NULL ? lpParam : this));

	if (!hFont)
		hFont = ::CreateFont(12, 0, 0, 0, 400, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEVICE_PRECIS, CLIP_MASK, ANTIALIASED_QUALITY, DEFAULT_PITCH, TEXT("MS Shell Dlg"));

	SendMessage(WM_SETFONT, WPARAM(hFont), TRUE);

	return true;
}

void CSystemControl::SetText(LPCTSTR szText)
{
	SendMessage(WM_SETTEXT, NULL, (LPARAM)szText);
}