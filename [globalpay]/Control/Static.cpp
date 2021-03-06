/**
* Static.cpp
*
* @program		Global-Pay
* @author		АНКУ ХХК
* @copyright	Copyright (c) 2017 он, АНКУ ХХК.
* @version		0.1.0.0
* @date			2017.09.19
*/

#include "static.h"

using namespace globalpay;

CStatic::CStatic(void)
: CSystemControl()
{
	SetClass(CSTATICCLASS);
}


bool CStatic::Create(HINSTANCE hInst, HWND hParent, LPCTSTR szDefaultText, RECT rcRect, HFONT hFont, DWORD WINSTYLE, DWORD WINSTYLEEX, LPVOID lpParam)
{
	return CSystemControl::Create(hInst, hParent, szDefaultText, rcRect, hFont, WINSTYLE, WINSTYLEEX, lpParam);
}
/*
LPVOID CStatic::OnPaint(HWND OwnerWnd, HDC hDC, PAINTSTRUCT pStruct)
{

	CStatic *ThisObject = reinterpret_cast<CStatic*>(GetUserDataLong(OwnerWnd));
	if (ThisObject)
	{
		::MessageBox(OwnerWnd, TEXT("thisobject static"), TEXT("static"), MB_OK);
	}
	HPEN hpenOld = static_cast<HPEN>(SelectObject(hDC, GetStockObject(DC_PEN)));
	HBRUSH hbrushOld = static_cast<HBRUSH>(SelectObject(hDC, GetStockObject(NULL_BRUSH)));

	RECT rcWindow;
	RECT rc1;
	::GetClientRect(OwnerWnd, &rcWindow);
	rc1 = rcWindow;
	
	::SetBkMode(hDC, TRANSPARENT);


	WCHAR szBuffer[255];
	UINT nCurrentChars = ::GetWindowTextLength(OwnerWnd);
	::GetWindowTextW(OwnerWnd, szBuffer, nCurrentChars * sizeof(WCHAR));
	::DrawText(hDC, szBuffer, -1, &rc1, DT_CENTER | DT_SINGLELINE | DT_VCENTER);

	// Clean up after ourselves.
	::SelectObject(hDC, hpenOld);
	::SelectObject(hDC, hbrushOld);
	return 0L;
}
*/