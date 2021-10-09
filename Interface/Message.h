#pragma once

#include "window/hoverbitmapwnd.h"
#include "control/picturepanel.h"
#include "control/button.h"
#include "control/gif.h"

#include "../config.hpp"

using namespace globalpay;

class ClientMessage : public CPicturePanel
{
public:
	ClientMessage();
	
	BOOL Create(HINSTANCE, HWND, RECT, DWORD WinStyle = WS_CHILD, DWORD ExWinStyle = WS_EX_CLIENTEDGE | WS_EX_TOPMOST);
	void SetImage(HBITMAP);
	void SetImage(LPCTSTR);

protected:
	static LPVOID CALLBACK DefaultPainter(HWND, HDC, PAINTSTRUCT);
};

class ClientAnimate : public CPanel
{
public:
	ClientAnimate();

	BOOL Create(HINSTANCE, HWND, RECT, DWORD WinStyle = WS_CHILD, DWORD ExWinStyle = WS_EX_CLIENTEDGE | WS_EX_TOPMOST);
	void SetAnimation(LPCTSTR);
	void SetRect(RECT);

	CGifControl animator;
};
/*
class ClientNotice : public ClientMessage
{
public:
	ClientNotice();

	CHoverBitmapWindow     OK, Retry, Cancel;
	static void            OkClicked(HWND);
	static void            RetryClicked(HWND);
	static void            CancelClicked(HWND);

protected:
	static LPVOID CALLBACK OnCreate(HWND hWnd, LPARAM lParam);
	static LPVOID CALLBACK NoticePainter(HWND, HDC, PAINTSTRUCT);
};
*/