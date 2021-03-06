/**
* CComboBox class - ComboBox.h
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
#define CCOMBOBOXCLASS WC_COMBOBOX

	class CComboBox : public CSystemControl
	{
	public:
		CComboBox(void);

		bool Create(
			HINSTANCE,
			HWND,
			LPCTSTR,
			RECT rcRect = coord::to_rect(60, 60, 100, 50),
			HFONT hFont = NULL,
			DWORD WINSTYLE = WS_CHILD | ES_AUTOVSCROLL | WS_TABSTOP | LBS_NOTIFY | LBS_NOINTEGRALHEIGHT,
			DWORD WINSTYLEEX = WS_EX_LEFT | WS_EX_LTRREADING | WS_EX_RIGHTSCROLLBAR | WS_EX_NOPARENTNOTIFY | WS_EX_CLIENTEDGE,
			LPVOID lpParam = NULL);
	};
}