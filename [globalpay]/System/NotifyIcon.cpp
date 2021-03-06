/**
* NotifyIcon.cpp
*
* @program		Global-Pay
* @author		АНКУ ХХК
* @copyright	Copyright (c) 2017 он, АНКУ ХХК.
* @version		0.1.0.0
* @date			2017.09.19
*/

#include "notifyicon.h"

using namespace globalpay;

CNotifyIcon::CNotifyIcon(void)
{
	bShown = FALSE;
	bMenu = FALSE;

	m_tnd.cbSize = sizeof(NOTIFYICONDATA);
	SetID(0);
	SetFlags(NIF_MESSAGE|NIF_ICON|NIF_TIP, true);
	SetText(_T("CNotifyIcon"));
	SetCallback(WM_TRAYICON);
}

CNotifyIcon::~CNotifyIcon(void)
{
	Delete();
}

void CNotifyIcon::SetID(UINT uID)
{
	m_tnd.uID = uID;
}

void CNotifyIcon::SetFlags(UINT uFlags, bool bSetAsNew)
{
	if (bSetAsNew)
		m_tnd.uFlags = uFlags;
	else
		m_tnd.uFlags = m_tnd.uFlags | uFlags;
}

void CNotifyIcon::SetText(LPCTSTR szText)
{
	::lstrcpyn(m_tnd.szTip, szText, sizeof(m_tnd.szTip));
}

void CNotifyIcon::SetCallback(UINT uCallbackMessage)
{
	m_tnd.uCallbackMessage = uCallbackMessage;
}

void CNotifyIcon::SetIcon(HICON hIcon)
{
	m_tnd.hIcon = hIcon;
}

void CNotifyIcon::SetInfo(LPCTSTR szInfoTitle, UINT uTimeOut)
{
	SetFlags(NIF_INFO);
	m_tnd.dwInfoFlags = NIIF_INFO;
    m_tnd.uTimeout = uTimeOut;

	::lstrcpyn(m_tnd.szInfoTitle, szInfoTitle, sizeof(m_tnd.szInfoTitle));
}

void CNotifyIcon::AssignWindow(HWND hWnd)
{
	m_tnd.hWnd = hWnd;
}

void CNotifyIcon::AddTray()
{
	Delete();

	DWORD dwMessage = NIM_ADD;
	bShown = ::Shell_NotifyIcon(dwMessage, &m_tnd);
}

void CNotifyIcon::AddInfo(LPCTSTR szInfo)
{
    ::lstrcpyn(m_tnd.szInfo, szInfo, sizeof(m_tnd.szInfo));
    ::Shell_NotifyIcon(NIM_MODIFY, &m_tnd);

    m_tnd.szInfo[0] = _T('\0');
}

void CNotifyIcon::Delete()
{
	if (bShown)
		::Shell_NotifyIcon(NIM_DELETE, &m_tnd);

	DeleteMenu();
}

UINT CNotifyIcon::GetID()
{
	return m_tnd.uID;
}

void CNotifyIcon::CreateMenu()
{
	if (hTrayMenu = ::CreatePopupMenu())
		bMenu = true;
}

void CNotifyIcon::DeleteMenu()
{
	if (bMenu)
		::DestroyMenu(hTrayMenu);
}