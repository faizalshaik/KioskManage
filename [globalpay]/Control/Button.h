/**
* CButton class - Button.h - Drived from CSystemControl
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
#define CBUTTONCLASS WC_BUTTON

	class CButton : public CSystemControl
	{
	public:
		CButton(void);

		bool Create(
			HINSTANCE,
			HWND,
			LPCTSTR,
			RECT rcRect = coord::to_rect(50, 50, 29, 13),
			HFONT hFont = NULL,
			DWORD WINSTYLE = WS_CHILD | WS_VISIBLE | WS_GROUP | BS_CENTER | BS_VCENTER,
			DWORD WINSTYLEEX = WS_EX_LEFT | WS_EX_LTRREADING | WS_EX_RIGHTSCROLLBAR | WS_EX_NOPARENTNOTIFY,
			LPVOID lpParam = NULL);

		void Check(BOOL bChecked = TRUE);
		bool IsChecked();

		static LPVOID CALLBACK OnParentCommand(HWND hWnd, WPARAM wParam, LPARAM lParam);
		static BOOL   CALLBACK EnumSiblings(HWND hBtn, LPARAM lParam);
	};
}