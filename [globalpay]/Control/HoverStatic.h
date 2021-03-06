/**
* CHoverStatic class - HoverStatic.h
*
* @program		Global-Pay
* @author		АНКУ ХХК
* @copyright	Copyright (c) 2017 он, АНКУ ХХК.
* @version		0.1.0.0
* @date			2017.09.19
*/

#pragma once

#include "../window/hoverlogic.h"

#include "static.h"

namespace globalpay {
	class CHoverStatic : public CStatic, public CHoverLogic
	{
	public:
		CHoverStatic(void);

	private:
		static LPVOID CALLBACK OnMouseMove(HWND hWnd, WPARAM sState, int X, int Y);
		static LPVOID CALLBACK OnMouseLeave(HWND hWnd);
		static LPVOID CALLBACK OnMouseDown(HWND hWnd, UINT mButton, WPARAM sState, int X, int Y);
		static LPVOID CALLBACK OnMouseUp(HWND hWnd, UINT mButton, WPARAM sState, int X, int Y);
	};
}