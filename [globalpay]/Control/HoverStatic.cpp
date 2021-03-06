/**
* HoverStatic.cpp
*
* @program		Global-Pay
* @author		АНКУ ХХК
* @copyright	Copyright (c) 2017 он, АНКУ ХХК.
* @version		0.1.0.0
* @date			2017.09.19
*/

#include "hoverstatic.h"

using namespace globalpay;

CHoverStatic::CHoverStatic(void)
: CStatic(), CHoverLogic()
{
	onMouseMove = OnMouseMove;
	onMouseLeave = OnMouseLeave;
	onMouseDown = OnMouseDown;
	onMouseUp = OnMouseUp;
}

LPVOID CHoverStatic::OnMouseMove(HWND hWnd, WPARAM sState, int X, int Y)
{
	CHoverStatic *ThisObject = reinterpret_cast<CHoverStatic*>(GetUserDataLong(hWnd));

	if (ThisObject)
		ThisObject->OnLogicMouseMove(hWnd);

	return 0L;
}

LPVOID CHoverStatic::OnMouseLeave(HWND hWnd)
{
	CHoverStatic *ThisObject = reinterpret_cast<CHoverStatic*>(GetUserDataLong(hWnd));

	if (ThisObject)
		ThisObject->OnLogicMouseLeave(hWnd);

	return 0L;
}

LPVOID CHoverStatic::OnMouseDown(HWND hWnd, UINT mButton, WPARAM sState, int X, int Y)
{
	if (mButton == mButtonLeft)
	{
		CHoverStatic *ThisObject = reinterpret_cast<CHoverStatic*>(GetUserDataLong(hWnd));

		if (ThisObject)
			ThisObject->OnLogicMouseDown(hWnd);
	}

	return 0L;
}

LPVOID CHoverStatic::OnMouseUp(HWND hWnd, UINT mButton, WPARAM sState, int X, int Y)
{
	if (mButton == mButtonLeft)
	{
		CHoverStatic *ThisObject = reinterpret_cast<CHoverStatic*>(GetUserDataLong(hWnd));

		if (ThisObject)
			ThisObject->OnLogicMouseUp(hWnd, mButton, ThisObject->IsMouseTracking());
	}

	return 0L;
}