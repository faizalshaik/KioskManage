/**
* WndDef.cpp
*
* @program		Global-Pay
* @author		АНКУ ХХК
* @copyright	Copyright (c) 2017 он, АНКУ ХХК.
* @version		0.1.0.0
* @date			2017.09.19
*/

#include "wnddef.h"

using namespace globalpay;

UINT CWndDef::CHILD_ID = CSAUR_CHILD_ID_START;

CWndDef::CWndDef(void)
: CWndBasic(), CMsg()
{
	onClick = NULL;
}

CWndDef::~CWndDef(void)
{
}

void CWndDef::SetClickEvent(void(*OnClickEvent)(HWND OwnerWnd))
{
	onClick = OnClickEvent;
}