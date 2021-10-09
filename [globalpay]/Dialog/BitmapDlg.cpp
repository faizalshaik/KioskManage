/**
* BitmapDlg.cpp
*
* @program		Global-Pay
* @author		АНКУ ХХК
* @copyright	Copyright (c) 2017 он, АНКУ ХХК.
* @version		0.1.0.0
* @date			2017.09.19
*/

#include "bitmapdlg.h"

using namespace globalpay;

CBitmapDialog::CBitmapDialog(void)
: CDialog()
{
	m_hBitmap = NULL;
	onPaint = OnPaint;
}

CBitmapDialog::~CBitmapDialog(void)
{
	SafeDeleteObject(m_hBitmap);
}

LPVOID CBitmapDialog::OnPaint(HWND hWnd, HDC pDC, PAINTSTRUCT pStruct)
{
	CBitmapDialog *ThisObject = reinterpret_cast<CBitmapDialog*>(GetUserDataLong(hWnd));

	if (ThisObject->GetBitmap())
	{
        HDC dcImage;
		dcImage = ::CreateCompatibleDC(pDC);
		if (!dcImage)
			return FALSE;

		BITMAP bmp;
		RECT rc = ThisObject->GetRect();
		::GetObject(ThisObject->GetBitmap(), sizeof(bmp), &bmp);
		int bWidth = bmp.bmWidth, bHeight = bmp.bmHeight;
		if (coord::width(rc) < bWidth)	bWidth = coord::width(rc);
		if (coord::height(rc) < bHeight) bHeight = coord::height(rc);
		::SelectObject(dcImage, ThisObject->GetBitmap());
		::BitBlt(pDC,0, 0, bWidth, bHeight, dcImage, 0, 0, SRCCOPY);
		::DeleteDC(dcImage);
	}
	return voidTRUE;
}

void CBitmapDialog::
	AssignBitmap(HBITMAP hNewImageHandle, bool bBmpToRgn, COLORREF cTransparentColor, COLORREF cTolerance)
{
	if (hNewImageHandle == NULL)
		return;

	SafeDeleteObject(m_hBitmap);

	m_hBitmap = hNewImageHandle;

	if(bBmpToRgn) 
		SetRegion(Rgn::Bitmap(GetBitmap(), cTransparentColor, cTolerance), false);
}

HBITMAP CBitmapDialog::GetBitmap()
{
	return m_hBitmap; 
}

void CBitmapDialog::CreateBitmap(int Width, int Height, UINT cPlanes, UINT cBitsPerPixel, CONST VOID* PDATA)
{
	if(!Width) 
		Width = coord::width(GetRect());

	if(!Height)	
		Height = coord::height(GetRect());

	m_hBitmap = ::CreateBitmap(Width, Height, cPlanes, cBitsPerPixel, PDATA);
}