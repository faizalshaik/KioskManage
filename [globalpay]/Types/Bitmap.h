/**
* Bitmap helper functions - Bitmap.h
*
* @program		Global-Pay
* @author		АНКУ ХХК
* @copyright	Copyright (c) 2017 он, АНКУ ХХК.
* @version		0.1.0.0
* @date			2017.09.19
*/

#pragma once

#include <windows.h>
#include <stdio.h>

#include <gdiplus.h>
#include <gdiplusinit.h>

#pragma comment(lib,"gdiplus.lib")

#include <OCIdl.h>
#include <OleCtl.h>

#include "globalpay.hpp"

using namespace Gdiplus;

namespace globalpay
{
#define COLORREF2RGB(Color) (Color & 0xff00) | ((Color >> 16) & 0xff) \
                                 | ((Color << 16) & 0xff0000)

	typedef struct
	{
		static COLORREF GetFirstPixelColor(HBITMAP);
		static HBITMAP  LoadPicture(LPCTSTR);
		static HBITMAP  LoadGDI(LPCTSTR);
		static HBITMAP  GetMonochrome(HBITMAP, COLORREF clrTransparent = NULL, COLORREF clrBK = NULL, COLORREF clrText = NULL);
		static SIZE     GetSize(HBITMAP);
		static HBITMAP  ReplaceColor(HBITMAP hBmp, COLORREF cOldColor, COLORREF cNewColor, HDC hDC = NULL);
	} Bmp;

	class CBitmapBits
	{
	public:
		CBitmapBits(BITMAP, int, int);
		virtual ~CBitmapBits();

		operator const VOID *() const;
		BYTE &operator [](const size_t);

	private:
		CBitmapBits();
		BYTE *m_bits;
	};
}