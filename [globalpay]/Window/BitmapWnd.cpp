/**
* BitmapWnd.cpp
*
* @program		Global-Pay
* @author		АНКУ ХХК
* @copyright	Copyright (c) 2017 он, АНКУ ХХК.
* @version		0.1.0.0
* @date			2017.09.19
*/

#include "bitmapwnd.h"

using namespace globalpay;

CBitmapWindow::CBitmapWindow(void)
: CWnd()
{
	SetClass(CBITMAPWINDOWCLASS);

	m_hBitmap      = NULL;
	bmDimension.cx = 0;
	bmDimension.cy = 0;
	onPaint        = OnPaint;
}

CBitmapWindow::~CBitmapWindow(void)
{
	SafeDeleteObject(m_hBitmap);
}

LPVOID CBitmapWindow::OnPaint(HWND OwnerWnd, HDC hDC, PAINTSTRUCT pStruct)
{
	CBitmapWindow *ThisObject = reinterpret_cast<CBitmapWindow*>(GetUserDataLong(OwnerWnd));

	if (ThisObject)
	{
		ThisObject->PaintBitmap(hDC);
	}

	return 0L;
}

BOOL CBitmapWindow::Initialize(HINSTANCE hInst, HWND hParent, LPCTSTR szText, RECT rcRect, DWORD WINSTYLE)
{
	if (!Register(CS_DBLCLKS|CS_OWNDC|CS_SAVEBITS, hInst))
		return FALSE;

	return Create(szText, WINSTYLE, rcRect, hParent, NULL, hInst, this);
}

void CBitmapWindow::AssignBitmap(HBITMAP hNewImageHandle, bool bUseBmpDimension, bool bBmpToRgn, COLORREF cTransparentColor, COLORREF cTolerance)
{
	if (hNewImageHandle == NULL)
		return;

	DeleteBitmap();

	m_hBitmap = hNewImageHandle;

	bmDimension = Bmp::GetSize(GetBitmap());

	if (bUseBmpDimension)
	{
		SetSize(bmDimension.cx, bmDimension.cy);
	}

	if (bBmpToRgn)
		SetRegion(Rgn::Bitmap(GetBitmap(), cTransparentColor, cTolerance), false);
}

HBITMAP CBitmapWindow::GetBitmap()
{
	return m_hBitmap; 
}

void CBitmapWindow::DeleteBitmap()
{
	SafeDeleteObject(m_hBitmap);
}

void CBitmapWindow::CreateBitmap(int Width, int Height, UINT cPlanes, UINT cBitsPerPixel, CONST VOID* PDATA)
{
	if (!Width)
		Width = coord::width(GetRect());

	if (!Height)
		Height = coord::height(GetRect());

	bmDimension.cx = Width;
	bmDimension.cy = Height;

	m_hBitmap = ::CreateBitmap(Width, Height, cPlanes, cBitsPerPixel, PDATA);
}

bool CBitmapWindow::PaintBitmap(HDC hDC)
{
	if (GetBitmap())
	{
		HDC dcImage;
		dcImage = ::CreateCompatibleDC(hDC);
		if (!dcImage)
			return false;

		RECT rc = GetRect();
		::SelectObject(dcImage, GetBitmap());
		::BitBlt(
			hDC, 0, 0,
			(coord::width(rc) < bmDimension.cx) ? coord::width(rc) : bmDimension.cx,
			(coord::height(rc) < bmDimension.cy) ? coord::height(rc) : bmDimension.cy,
			dcImage, 0, 0, SRCCOPY);
		::DeleteDC(dcImage);

		return true;
	}
	return false;
}

SIZE CBitmapWindow::GetBitmapDimension()
{
	return bmDimension;
}