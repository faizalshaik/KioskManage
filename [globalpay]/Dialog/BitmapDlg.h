/**
* CBitmapDialog class - BitmapDlg.h
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

#include "dlg.h"

namespace globalpay {
	class CBitmapDialog : public CDialog
	{
	public:
		CBitmapDialog(void);
		~CBitmapDialog(void);

	private:
		HBITMAP	m_hBitmap;

	protected:
		static LPVOID CALLBACK OnPaint(HWND hWnd, HDC pDC, PAINTSTRUCT pStruct);

	public:
		void	AssignBitmap(HBITMAP, bool bBmpToRgn = false, COLORREF cTransparentColor = NULL, COLORREF cTolerance = 0x101010);
		HBITMAP GetBitmap();
		void	CreateBitmap(int Width = 0, int Height = 0, UINT cPlanes = 1, UINT cBitsPerPixel = 32, CONST VOID* PDATA = NULL);
	};
}