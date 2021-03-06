/**
* PicturePanel.cpp
*
* @program		Global-Pay
* @author		АНКУ ХХК
* @copyright	Copyright (c) 2017 он, АНКУ ХХК.
* @version		0.1.0.0
* @date			2017.09.19
*/

#include "picturepanel.h"

using namespace globalpay;

CPicturePanel::CPicturePanel(void)
	: CBitmapWindow()
{
	SetClass(CPICTUREPANELCLASS);
}

bool CPicturePanel::Initialize(HINSTANCE hInst, HWND hParent, RECT rcRect, HBITMAP hBmp, LPCTSTR szText, DWORD WINSTYLE)
{
	if (!CBitmapWindow::Initialize(hInst, hParent, szText, rcRect))
		return false;

	if (hBmp)
		AssignBitmap(hBmp);

	return true;
}
