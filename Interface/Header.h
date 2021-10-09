#pragma once

#include "window/hoverbitmapwnd.h"
#include "control/panel.h"
#include "control/static.h"

#include "../config.hpp"

class CSessionTimer : public CBitmapWindow
{
public:
	CSessionTimer();

	int Interval;
	COLORREF exp;

	void Start(int);
	void Stop();
	void Freeze();
	void GetRemaining(int *, int *);
	bool IsExpiring();

protected:
	static LPVOID CALLBACK OnTimer(HWND hWnd, UINT uTimerId);
	static LPVOID CALLBACK OnPaint(HWND hWnd, HDC hDC, PAINTSTRUCT pStruct);
};

class CHeader : public CPanel
{
public:
	CHeader();

	CHoverBitmapWindow icon;
	CStatic            status;
	CSessionTimer      timer;

	void init();
	void Show();
	void Hide();
	void SetStatus(LPCTSTR);

protected:
	static LPVOID CALLBACK OnCreate(HWND hWnd, LPARAM lParam);
	static LPVOID CALLBACK OnCtlColorStatic(HWND hWnd, HDC hStaticDC, HWND hStaticCtl);
	static LPVOID CALLBACK OnIconPaint(HWND hWnd, HDC hDC, PAINTSTRUCT pStruct);

	static void IconClicked(HWND);
};