/**
* CHoverBitmapWindow class - HoverBitmapWnd.h
*
* @program		Global-Pay
* @author		АНКУ ХХК
* @copyright	Copyright (c) 2017 он, АНКУ ХХК.
* @version		0.1.0.0
* @date			2017.09.19
*/

#pragma once

#include "bitmapwnd.h"
#include "hoverlogic.h"

namespace globalpay {
#define CHOVERBITMAPWNDCLASS TEXT("codesaur::hoverbitmapwnd")

	class CHoverBitmapWindow : public CBitmapWindow, public CHoverLogic
	{
	public:
		CHoverBitmapWindow(void);

		void Instant(HINSTANCE, HWND, RECT, HBITMAP hBmp = NULL, void(*OnClickEvent)(HWND) = NULL, BOOL bShow = TRUE, LPCTSTR szText = TEXT("HoverBitmapWindow"), DWORD WINSTYLE = WS_CHILD);
		bool PaintBitmap(HDC hDC);

	protected:
		static LPVOID CALLBACK OnShow(HWND hWnd, UINT nStatus);
		static LPVOID CALLBACK OnPaint(HWND hWnd, HDC hDC, PAINTSTRUCT pStruct);
		static LPVOID CALLBACK OnMouseMove(HWND hWnd, WPARAM sState, int X, int Y);
		static LPVOID CALLBACK OnMouseLeave(HWND hWnd);
		static LPVOID CALLBACK OnMouseDown(HWND hWnd, UINT mButton, WPARAM sState, int X, int Y);
		static LPVOID CALLBACK OnMouseUp(HWND hWnd, UINT mButton, WPARAM sState, int X, int Y);
		static LPVOID CALLBACK OnMouseDblClick(HWND hWnd, UINT mButton, WPARAM sState, int X, int Y);
	};
}