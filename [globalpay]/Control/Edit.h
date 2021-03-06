/**
* CEdit class - Edit.h
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
#define CEDITCLASS WC_EDIT

	class CEdit : public CSystemControl
	{
	public:
		CEdit(void);

		bool Create(
			HINSTANCE,
			HWND,
			LPCTSTR,
			RECT rcRect = coord::to_rect(50, 50, 60, 23),
			HFONT hFont = NULL,
			DWORD WINSTYLE = WS_CHILDWINDOW | WS_VISIBLE | WS_BORDER | WS_TABSTOP | ES_LEFT | ES_AUTOHSCROLL,
			DWORD WINSTYLEEX = WS_EX_LEFT | WS_EX_LTRREADING | WS_EX_RIGHTSCROLLBAR | WS_EX_NOPARENTNOTIFY,
			LPVOID lpParam = NULL);

		void SetReadOnly(BOOL);
		void SetLimitText(int);
	};
}