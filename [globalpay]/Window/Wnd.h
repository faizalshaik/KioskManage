/**
* Window Object - Wnd.h
			    - added SetLeft(), SetTop(), SetRight(), SetBottom(), SetWidth(), SetHeight()
	            - added IsWindowExists()
	            - char m_szClassName[35] replaced with LPTSTR m_sClassName // good solution
	            - added SetRect()
	            - fixed errors SetRight() & SetBottom() // SWP_NOSIZE -> SWP_NOMOVE
	            - changed SetRegion(HRGN) -> SetRegion(HRGN, bool bRepaint=true)
	            - support Mouse Double Click event // OnMouseDblClick()
	            - new events OnDestroy, OnSize
	            - added GetParentRect(), fixed some errors on Size functions
	            - added Move method
	            - added SetParentBkBrush()
*
* @program		Global-Pay
* @author		АНКУ ХХК
* @copyright	Copyright (c) 2017 он, АНКУ ХХК.
* @version		0.1.0.0
* @date			2017.09.19
*/

#pragma once

#include "wnddef.h"

namespace globalpay {
#define CWNDCLASS TEXT("codesaur::wnd")

	class CWnd : public CWndDef
	{
	public:
		CWnd(void);
		~CWnd(void);

	protected:
		static LRESULT CALLBACK InitialWndProc(HWND, UINT, WPARAM, LPARAM);
		static LRESULT CALLBACK StaticWndProc(HWND, UINT, WPARAM, LPARAM);
		LRESULT CALLBACK        ActualWndProc(HWND, UINT, WPARAM, LPARAM);

	public:
		BOOL Register(UINT, HINSTANCE, HICON hIcon = NULL, HCURSOR hCursor = ::LoadCursor(NULL, IDC_ARROW), HBRUSH hbrBackground = reinterpret_cast<HBRUSH>(COLOR_BACKGROUND), LPCTSTR lpszMenuName = NULL);
		BOOL RegisterEx(UINT, HINSTANCE, HICON hIcon = NULL, HICON hIconSmall = NULL, HCURSOR hCursor = ::LoadCursor(NULL, IDC_ARROW), HBRUSH hbrBackground = reinterpret_cast<HBRUSH>(COLOR_BACKGROUND), LPCTSTR lpszMenuName = NULL);
		BOOL Create(LPCTSTR, DWORD, RECT, HWND, HMENU hMenu, HINSTANCE, LPVOID lpParam = NULL);
		BOOL CreateEx(DWORD, LPCTSTR, DWORD, RECT, HWND, HMENU hMenu, HINSTANCE, LPVOID lpParam = NULL);
	};
}