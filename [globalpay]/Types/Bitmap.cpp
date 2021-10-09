/**
* Bitmap.cpp
*
* @program		Global-Pay
* @author		АНКУ ХХК
* @copyright	Copyright (c) 2017 он, АНКУ ХХК.
* @version		0.1.0.0
* @date			2017.09.19
*/

#include "bitmap.h"

using namespace globalpay;

COLORREF Bmp::GetFirstPixelColor(HBITMAP hBmp)
{
	COLORREF clr = RGB(0, 0, 0);
	if (hBmp)
	{
		HDC hMemDC = ::CreateCompatibleDC(NULL);
		if (hMemDC)
		{
			BITMAP bm;
			::GetObject(hBmp, sizeof(bm), &bm);

			BITMAPINFOHEADER RGB32BITSBITMAPINFO = { sizeof(BITMAPINFOHEADER), bm.bmWidth, bm.bmHeight, 1, 32, BI_RGB, 0, 0, 0, 0, 0 };

			VOID * pbits32;
			HBITMAP hbm32 = ::CreateDIBSection(hMemDC, (BITMAPINFO *)&RGB32BITSBITMAPINFO, DIB_RGB_COLORS, &pbits32, NULL, 0);
			if (hbm32)
			{
				HBITMAP holdBmp = (HBITMAP) ::SelectObject(hMemDC, hbm32);
				HDC hDC = ::CreateCompatibleDC(hMemDC);
				if (hDC)
				{
					BITMAP bm32;
					::GetObject(hbm32, sizeof(bm32), &bm32);
					while (bm32.bmWidthBytes % 4)
						bm32.bmWidthBytes++;

					HBITMAP holdBmp = (HBITMAP) ::SelectObject(hDC, hBmp);
					::BitBlt(hMemDC, 0, 0, bm.bmWidth, bm.bmHeight, hDC, 0, 0, SRCCOPY);
					BYTE *p32 = (BYTE *)bm32.bmBits + (bm32.bmHeight - 1) * bm32.bmWidthBytes;
					LONG *p = (LONG *)p32;

					clr = RGB(GetRValue(*p), GetGValue(*p), GetBValue(*p));

					::SelectObject(hDC, holdBmp);
					::DeleteDC(hDC);
				}
				::DeleteObject(::SelectObject(hMemDC, holdBmp));
			}
			::DeleteDC(hMemDC);
		}
	}
	return clr;
}

HBITMAP Bmp::LoadPicture(LPCTSTR szFileName)
{
	// Use IPicture stuff to use JPG / GIF files
	IPicture* p;
	IStream*  s;
	HGLOBAL   hG;
	void*     pp;
	FILE*     fp;
	errno_t   err;

	if ((err = ::_wfopen_s(&fp, szFileName, TEXT("rb"))) != 0)
	{
		return NULL;
	}

	::fseek(fp, 0, SEEK_END);
	int fs = ::ftell(fp);
	::fseek(fp, 0, SEEK_SET);
	hG = ::GlobalAlloc(GPTR, fs);
	if (!hG)
	{
		::fclose(fp);
		return NULL;
	}
	pp = (void*)hG;
	::fread(pp, 1, fs, fp);
	::fclose(fp);

	// Create an IStream so IPicture can 
	::CreateStreamOnHGlobal(hG, false, &s);
	if (!s)
	{
		::GlobalFree(hG);
		return NULL;
	}

	::OleLoadPicture(s, 0, false, IID_IPicture, (void**)&p);

	if (!p)
	{
		s->Release();
		::GlobalFree(hG);
		return NULL;
	}
	s->Release();
	::GlobalFree(hG);

	HBITMAP hB = 0;
	p->get_Handle((unsigned int*)&hB);

	// Copy the image. Necessary, because upon p's release,
	// the handle is destroyed.
	HBITMAP hBB = (HBITMAP)::CopyImage(hB, IMAGE_BITMAP, 0, 0, LR_COPYRETURNORG);

	p->Release();
	return hBB;
}

HBITMAP Bmp::LoadGDI(LPCTSTR pFilePath)
{
	GdiplusStartupInput gpStartupInput;
	ULONG_PTR gpToken;
	GdiplusStartup(&gpToken, &gpStartupInput, NULL);
	HBITMAP result = NULL;
	Gdiplus::Bitmap* bitmap = Gdiplus::Bitmap::FromFile(pFilePath, false);
	if (bitmap)
	{
		bitmap->GetHBITMAP(Color(255, 255, 255), &result);
		delete bitmap;
	}
	GdiplusShutdown(gpToken);
	return result;
}

HBITMAP Bmp::GetMonochrome(HBITMAP source, COLORREF clrTransparent, COLORREF clrBK, COLORREF clrText)
{
	HDC hdcColor = ::CreateCompatibleDC(NULL);
	::SelectObject(hdcColor, source);
	
	if (clrTransparent == NULL)
		clrTransparent = ::GetPixel(hdcColor, 0, 0);
	
	SIZE size = Bmp::GetSize(source);
	HBITMAP hbmMono = ::CreateBitmap(size.cx, size.cy, 1, 1, NULL);
	HDC hdcMono = ::CreateCompatibleDC(NULL);
	::SelectObject(hdcMono, hbmMono);

	::SetBkColor(hdcColor, clrTransparent);// clrBK == NULL ? clrTransparent : clrBK);
//	if (clrText != NULL)
//		::SetTextColor(hdcColor, clrText);
	::BitBlt(hdcMono, 0, 0, size.cx, size.cy, hdcColor, 0, 0, SRCCOPY);

	::DeleteDC(hdcMono);
	::DeleteDC(hdcColor);

	return hbmMono;
}

SIZE Bmp::GetSize(HBITMAP hBmp)
{
	BITMAP bmp;
	SIZE size = { 0 };

	if (hBmp)
	{
		::GetObject(hBmp, sizeof(bmp), &bmp);
		size.cx = bmp.bmWidth;
		size.cy = bmp.bmHeight;
	}

	return size;
}

CBitmapBits::CBitmapBits()
{
	m_bits = NULL;
}

CBitmapBits::CBitmapBits(BITMAP bm, int cx, int cy)
{
	DWORD cb = (bm.bmWidthBytes + bm.bmWidthBytes % sizeof(DWORD)) * cy;

	if (m_bits = new BYTE[cb])
		::memset(m_bits, 0, cb);
}

CBitmapBits::~CBitmapBits()
{
	if (m_bits)
		delete[] m_bits;
}

CBitmapBits::operator const VOID *() const
{
	return m_bits;
}

BYTE &CBitmapBits::operator [](const size_t ix)
{
	return m_bits[ix];
}

HBITMAP Bmp::ReplaceColor(HBITMAP hBmp, COLORREF cOldColor, COLORREF cNewColor, HDC hDC)
{
	HBITMAP RetBmp = NULL;
	if (hBmp)
	{
		HDC BufferDC = ::CreateCompatibleDC(NULL);
		if (BufferDC)
		{
			HBITMAP hTmpBitmap = (HBITMAP)NULL;
			if (hDC)
			{
				if (hBmp == (HBITMAP)::GetCurrentObject(hDC, OBJ_BITMAP))
				{
					hTmpBitmap = ::CreateBitmap(1, 1, 1, 1, NULL);
					::SelectObject(hDC, hTmpBitmap);
				}
			}

			HGDIOBJ PreviousBufferObject = ::SelectObject(BufferDC, hBmp);
			
			HDC DirectDC = ::CreateCompatibleDC(NULL);
			if (DirectDC)
			{
				BITMAP bm;
				::GetObject(hBmp, sizeof(bm), &bm);

				BITMAPINFO RGB32BitsBITMAPINFO;
				::ZeroMemory(&RGB32BitsBITMAPINFO, sizeof(BITMAPINFO));
				RGB32BitsBITMAPINFO.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
				RGB32BitsBITMAPINFO.bmiHeader.biWidth = bm.bmWidth;
				RGB32BitsBITMAPINFO.bmiHeader.biHeight = bm.bmHeight;
				RGB32BitsBITMAPINFO.bmiHeader.biPlanes = 1;
				RGB32BitsBITMAPINFO.bmiHeader.biBitCount = 32;

				UINT *ptPixels;
				HBITMAP DirectBitmap = ::CreateDIBSection(DirectDC,
					(BITMAPINFO *)&RGB32BitsBITMAPINFO,
					DIB_RGB_COLORS,
					(void **)&ptPixels,
					NULL, 0);
				if (DirectBitmap)
				{
					HGDIOBJ PreviousObject = ::SelectObject(DirectDC, DirectBitmap);
					::BitBlt(DirectDC, 0, 0,
						bm.bmWidth, bm.bmHeight,
						BufferDC, 0, 0, SRCCOPY);

					cOldColor = COLORREF2RGB(cOldColor);
					cNewColor = COLORREF2RGB(cNewColor);

					for (int i = ((bm.bmWidth*bm.bmHeight) - 1); i >= 0; i--)
					{
						if (ptPixels[i] == cOldColor) ptPixels[i] = cNewColor;
					}
					::SelectObject(DirectDC, PreviousObject);
					RetBmp = DirectBitmap;
				}
				::DeleteDC(DirectDC);
			}
			
			if (hTmpBitmap)
			{
				::SelectObject(hDC, hBmp);
				::DeleteObject(hTmpBitmap);
			}

			::SelectObject(BufferDC, PreviousBufferObject);
			::DeleteDC(BufferDC);
		}
	}
	return RetBmp;
}