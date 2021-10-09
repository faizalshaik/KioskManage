/**
* TicketMachine -> Touch Main Window - Initial version
*
* @program		Global-Pay
* @author		АНКУ ХХК
* @copyright	Copyright (c) 2017 он, АНКУ ХХК.
* @version		0.1.0.0
* @date			2017.09.19
*/

#pragma once

#include "window/wnd.h"
#include "control/panel.h"
#include "control/gif.h"
#include "system/thread.h"

using namespace globalpay;

class CLoader : public CWnd
{
public:
	CLoader();

	CGifControl gif;

protected:
	static LPVOID CALLBACK OnPaint(HWND OwnerWnd, HDC hDC, PAINTSTRUCT pStruct);
	static LPVOID CALLBACK OnSetText(HWND hWnd, WPARAM wParam, LPARAM lParam);
};

class CTouchWnd : public CWnd
{
public:
	CTouchWnd(void);

	CLoader loader;
	CThread service;

	BOOL OnDisconnect();
	
	static void IdentifyMe(LPVOID);
	UINT GetID();

protected:
	static LPVOID CALLBACK OnCreate(HWND hWnd, LPARAM lParam);
	static LPVOID CALLBACK OnClose(HWND hWnd);
	static LPVOID CALLBACK OnTimer(HWND hWnd, UINT uTimerId);
};

extern CTouchWnd Touch;