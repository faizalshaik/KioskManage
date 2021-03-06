/**
* CBitmapWindow class - BitmapWnd.h
*
* @program		Global-Pay
* @author		АНКУ ХХК
* @copyright	Copyright (c) 2017 он, АНКУ ХХК.
* @version		0.1.0.0
* @date			2017.09.19
*/

#pragma once

#include "../types/region.h"
#include "../types/bitmap.h"

#include "wnd.h"

namespace globalpay {
#define CBITMAPWINDOWCLASS TEXT("codesaur::bitmapwnd")

	class CBitmapWindow : public CWnd
	{
	public:
		CBitmapWindow(void);
		~CBitmapWindow(void);

		HBITMAP GetBitmap();
		void    DeleteBitmap();
		BOOL	Initialize(HINSTANCE, HWND, LPCTSTR, RECT, DWORD WINSTYLE = WS_CHILD | WS_CLIPSIBLINGS);
		void	AssignBitmap(HBITMAP, bool bUseBmpDimension = false, bool bBmpToRgn = false, COLORREF cTransparentColor = NULL, COLORREF cTolerance = 0x101010);
		void	CreateBitmap(int Width = 0, int Height = 0, UINT cPlanes = 1, UINT cBitsPerPixel = 32, CONST VOID* PDATA = NULL);
		bool    PaintBitmap(HDC hDC);
		SIZE    GetBitmapDimension();

	protected:
		static	LPVOID CALLBACK OnPaint(HWND OwnerWnd, HDC hDC, PAINTSTRUCT pStruct);

	private:
		HBITMAP	m_hBitmap;
		SIZE    bmDimension;
	};
}