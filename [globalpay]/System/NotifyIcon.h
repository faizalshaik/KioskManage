/**
* Contains information that the system needs to display notifications in the notification area - NotifyIcon.h
*
* @program		Global-Pay
* @author		АНКУ ХХК
* @copyright	Copyright (c) 2017 он, АНКУ ХХК.
* @version		0.1.0.0
* @date			2017.09.19
*/

#pragma once

#include <windows.h>
#include <tchar.h>

#include "../window/msg.h"

namespace globalpay {
	class CNotifyIcon
	{
	public:
		CNotifyIcon(void);
		~CNotifyIcon(void);

	private:
		NOTIFYICONDATA m_tnd;
		BOOL           bShown;
		HMENU          hTrayMenu;
		BOOL           bMenu;

	public:
		void SetID(UINT);
		UINT GetID();
		void SetFlags(UINT, bool bSetAsNew = false);
		void SetText(LPCTSTR);
		void SetCallback(UINT);
		void SetIcon(HICON);
		void SetInfo(LPCTSTR, UINT uTimeOut = 10);

		void AssignWindow(HWND);
		void AddTray();
		void AddInfo(LPCTSTR);

		void Delete();

		void CreateMenu();
		void DeleteMenu();
	};
}