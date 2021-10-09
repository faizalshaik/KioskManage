/**
* CRadio class - Radio.h - Initial
*
* @program		Global-Pay
* @author		АНКУ ХХК
* @copyright	Copyright (c) 2017 он, АНКУ ХХК.
* @version		0.1.0.0
* @date			2017.09.19
*/

#pragma once

#include "button.h"
#include "static.h"

namespace globalpay {
	class CRadio : public CButton
	{
	public:
		CRadio();

		bool Create(
			HINSTANCE,
			HWND,
			LPCTSTR,
			RECT rcRect = coord::to_rect(10, 10, 200, 30),
			HFONT hFont = NULL,
			DWORD WINSTYLE = WS_VISIBLE | WS_CHILD | BS_AUTORADIOBUTTON,
			DWORD WINSTYLEEX = WS_EX_LEFT | WS_EX_LTRREADING | WS_EX_RIGHTSCROLLBAR | WS_EX_NOPARENTNOTIFY,
			LPVOID lpParam = NULL);
	};
}