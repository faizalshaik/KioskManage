/**
* Radio.cpp
*
* @program		Global-Pay
* @author		АНКУ ХХК
* @copyright	Copyright (c) 2017 он, АНКУ ХХК.
* @version		0.1.0.0
* @date			2017.09.19
*/

#include "radio.h"

using namespace globalpay;

CRadio::CRadio()
	: CButton()
{
}

bool CRadio::Create(HINSTANCE hInst, HWND hParent, LPCTSTR szDefaultText, RECT rcRect, HFONT hFont, DWORD WINSTYLE, DWORD WINSTYLEEX, LPVOID lpParam)
{
	return CButton::Create(hInst, hParent, szDefaultText, rcRect, hFont, WINSTYLE, WINSTYLEEX, this);
}