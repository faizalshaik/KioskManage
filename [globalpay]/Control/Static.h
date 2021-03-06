/**
* CStatic class - Static.h
*
* @program		Global-Pay
* @author		АНКУ ХХК
* @copyright	Copyright (c) 2017 он, АНКУ ХХК.
* @version		0.1.0.0
* @date			2017.09.19
*/

#pragma once

#include "systemctl.h"

namespace globalpay {
#define CSTATICCLASS WC_STATIC

	class CStatic : public CSystemControl
	{
	public:
		CStatic(void);

		bool Create(
			HINSTANCE,
			HWND,
			LPCTSTR,
			RECT rcRect = coord::to_rect(50, 50, 29, 13),
			HFONT hFont = NULL,
			DWORD WINSTYLE = WS_CHILD | WS_VISIBLE | WS_GROUP | SS_LEFT | SS_NOTIFY,
			DWORD WINSTYLEEX = WS_EX_LEFT | WS_EX_LTRREADING | WS_EX_RIGHTSCROLLBAR | WS_EX_NOPARENTNOTIFY,
			LPVOID lpParam = NULL);
	};
}