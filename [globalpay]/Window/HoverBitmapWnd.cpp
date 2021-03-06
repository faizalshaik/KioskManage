/**
* HoverBitmapWnd.cpp
*
* @program		Global-Pay
* @author		АНКУ ХХК
* @copyright	Copyright (c) 2017 он, АНКУ ХХК.
* @version		0.1.0.0
* @date			2017.09.19
*/

#include "hoverbitmapwnd.h"

using namespace globalpay;

CHoverBitmapWindow::CHoverBitmapWindow(void)
	: CBitmapWindow(), CHoverLogic()
{
	SetClass(CHOVERBITMAPWNDCLASS);

	onShow = OnShow;
	onPaint = OnPaint;
	onMouseMove = OnMouseMove;
	onMouseLeave = OnMouseLeave;
	onMouseDown = OnMouseDown;
	onMouseUp = OnMouseUp;
}

void CHoverBitmapWindow::Instant(HINSTANCE hInst, HWND hParent, RECT rc, HBITMAP hBmp, void(*OnClickEvent)(HWND), BOOL bShow, LPCTSTR szText, DWORD WINSTYLE)
{
	if (Initialize(hInst, hParent, szText, rc, WINSTYLE))
	{
		if (hBmp)
			AssignBitmap(hBmp);

		SetCursorHand();

		if (bShow)
			ShowNormal();

		SetHoverClick(OnClickEvent);
	}
}

LPVOID CHoverBitmapWindow::OnShow(HWND hWnd, UINT nStatus)
{
	if (IsMouseOverWnd(hWnd))
	{
		CHoverBitmapWindow *ThisObject = reinterpret_cast<CHoverBitmapWindow*>(GetUserDataLong(hWnd));
		if (StartTrackMouseLeaveWnd(hWnd))
			ThisObject->SetMouseTracking(TRUE);
	}
	return 0L;
}

LPVOID CHoverBitmapWindow::OnPaint(HWND hWnd, HDC hDC, PAINTSTRUCT pStruct)
{
	CHoverBitmapWindow *ThisObject = reinterpret_cast<CHoverBitmapWindow*>(GetUserDataLong(hWnd));

	if (ThisObject)
	{
		ThisObject->PaintBitmap(hDC);
	}

	return 0L;
}

LPVOID CHoverBitmapWindow::OnMouseMove(HWND hWnd, WPARAM sState, int X, int Y)
{
	CHoverBitmapWindow *ThisObject = reinterpret_cast<CHoverBitmapWindow*>(GetUserDataLong(hWnd));

	if (ThisObject)
		ThisObject->OnLogicMouseMove(hWnd);

	return 0L;
}

LPVOID CHoverBitmapWindow::OnMouseLeave(HWND hWnd)
{
	CHoverBitmapWindow *ThisObject = reinterpret_cast<CHoverBitmapWindow*>(GetUserDataLong(hWnd));

	if (ThisObject)
		ThisObject->OnLogicMouseLeave(hWnd);

	return 0L;
}

LPVOID CHoverBitmapWindow::OnMouseDown(HWND hWnd, UINT mButton, WPARAM sState, int X, int Y)
{
	if (mButton == mButtonLeft)
	{
		CHoverBitmapWindow *ThisObject = reinterpret_cast<CHoverBitmapWindow*>(GetUserDataLong(hWnd));

		if (ThisObject)
			ThisObject->OnLogicMouseDown(hWnd);
	}

	return 0L;
}

LPVOID CHoverBitmapWindow::OnMouseUp(HWND hWnd, UINT mButton, WPARAM sState, int X, int Y)
{
	if (mButton == mButtonLeft)
	{
		CHoverBitmapWindow *ThisObject = reinterpret_cast<CHoverBitmapWindow*>(GetUserDataLong(hWnd));

		if (ThisObject)
			ThisObject->OnLogicMouseUp(hWnd, mButton, ThisObject->IsMouseTracking());
	}

	return 0L;
}

LPVOID CHoverBitmapWindow::OnMouseDblClick(HWND hWnd, UINT mButton, WPARAM sState, int X, int Y)
{
	if (mButton == mButtonLeft)
	{
		CHoverBitmapWindow *ThisObject = reinterpret_cast<CHoverBitmapWindow*>(GetUserDataLong(hWnd));

		if (ThisObject)
			ThisObject->OnLogicMouseDblClick(hWnd, mButton, ThisObject->IsMouseTracking());
	}
	return 0L;
}

bool CHoverBitmapWindow::PaintBitmap(HDC hDC)
{
	if (GetBitmap())
	{
		HDC dcImage;
		dcImage = ::CreateCompatibleDC(hDC);
		if (!dcImage)
			return false;

		RECT rc = GetRect();

		::SelectObject(dcImage, GetBitmap());
		::BitBlt(hDC, 0, 0, coord::width(rc), coord::height(rc), dcImage,
			coord::width(rc) * (IsFixed() ? FramePos[_hsDown] : FramePos[hsCurrent]), 0, SRCCOPY);
		::DeleteDC(dcImage);
		return true;
	}
	return false;
}