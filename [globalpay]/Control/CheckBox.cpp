/**
* CheckBox.cpp
*
* @program		Global-Pay
* @author		АНКУ ХХК
* @copyright	Copyright (c) 2017 он, АНКУ ХХК.
* @version		0.1.0.0
* @date			2017.09.19
*/

#include "checkbox.h"

using namespace globalpay;

CCheckBox::CCheckBox()
	: CButton()
{
}

bool CCheckBox::Create(HINSTANCE hInst, HWND hParent, LPCTSTR szDefaultText, RECT rcRect, HFONT hFont, DWORD WINSTYLE, DWORD WINSTYLEEX, LPVOID lpParam)
{
	return CButton::Create(hInst, hParent, szDefaultText, rcRect, hFont, WINSTYLE, WINSTYLEEX, lpParam);
}