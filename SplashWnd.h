#pragma once

#include "window/bitmapwnd.h"
#include "control/gif.h"

using namespace globalpay;

class CSplashWnd : public CBitmapWindow
{
public:
	CSplashWnd();

	void Init(HINSTANCE);
	void Display(UINT);

protected:
	static LPVOID CALLBACK OnTimer(HWND hWnd, UINT uTimerId);
	static LPVOID CALLBACK OnDestroy(HWND hWnd);
	static LPVOID CALLBACK OnPaint(HWND OwnerWnd, HDC hDC, PAINTSTRUCT pStruct);
};