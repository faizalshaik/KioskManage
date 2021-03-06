/**
* CListBox class - ListBox.h
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
#define CLISTBOXCLASS WC_LISTBOX

	class CListBox : public CSystemControl
	{
	public:
		CListBox(void);

		bool Create(
			HINSTANCE,
			HWND,
			LPCTSTR,
			RECT rcRect = coord::to_rect(60, 60, 72, 65),
			HFONT hFont = NULL,
			DWORD WINSTYLE = WS_CHILD | ES_AUTOVSCROLL | WS_TABSTOP | LBS_NOTIFY | LBS_NOINTEGRALHEIGHT,
			DWORD WINSTYLEEX = WS_EX_LEFT | WS_EX_LTRREADING | WS_EX_RIGHTSCROLLBAR | WS_EX_NOPARENTNOTIFY | WS_EX_CLIENTEDGE,
			LPVOID lpParam = NULL);

		void AddString(LPCTSTR szStr);
		void DeleteString(int);
		void InsertString(LPCTSTR szStr, int);
		int  GetItemCount();
		void GetItemText(LPTSTR, UINT);
		int  GetFirstSelectedIndex();
		void SelectIndexMulti(int, BOOL bHighlight = TRUE);
		void DeselectAllMulti();
	};
}