/**
* WindowBasic object - WndBasic.h
*
* @program		Global-Pay
* @author		АНКУ ХХК
* @copyright	Copyright (c) 2017 он, АНКУ ХХК.
* @version		0.1.0.0
* @date			2017.09.19
*/

#pragma once

#include <string>

#include "wndfunc.h"

namespace globalpay {
#define CWNDBASICCLASS TEXT("cdn:wndbasic")

	class CWndBasic
	{
	public:
		CWndBasic(void);
		~CWndBasic(void);

		HWND     m_hWnd;
		operator HWND();
		operator UINT();		

	private:
		LPTSTR m_sClassName;

	public:
		HWND	  GetHandle();
		void	  SetHandleForced(HWND);
		HWND	  GetParent();
		void	  SetParent(HWND);
		HINSTANCE GetInstance();
		void	  SetClass(LPTSTR);
		LPTSTR	  GetClass();

		void Show();
		void ShowNormal();
		void Restore();
		void SetForeground();
		void SetAfter(HWND);
		BOOL IsVisible();
		void Minimize();
		void Maximize();
		void Hide();
		void Close();
		void DestroyWindow();

		void Activate();
		void Enable(bool bEnable = true);
		void Disable();
		BOOL IsEnabled();
		void SetFocus();

		void Invalidate(bool bErase = false);
		void InvalidateRect(RECT, bool bErase = false);
		void InvalidateRegion(HRGN, bool bErase = false);
		void Validate();
		void ValidateRect(RECT);
		void ValidateRegion(HRGN);
		void Update();
		void Redraw();

		RECT GetRect();
		RECT GetClientRect();
		RECT GetParentRect();
		RECT GetParentClientRect();
		void SetRegion(HRGN, bool bRepaint = false);
		void SetRect(RECT);
		void SetWidth(unsigned int);
		void SetHeight(unsigned int);
		void SetSize(unsigned int, unsigned int);

		void MoveTo(unsigned int, unsigned int, bool bRepaint = false);
		void Center(bool bDesktop = false, bool bRepaint = false);

		void GetText(LPTSTR, UINT length = 0);
		void SetText(LPCTSTR);
		int  GetTextLength();
		SIZE GetTextExtendPoint(LPCTSTR szText = NULL, HDC hDC = NULL);

		void SetCursor(HCURSOR);
		void SetIcon(HICON);
		void SetIconSmall(HICON);
		void SetBkBrush(HBRUSH);
		void SetCursorHand();
		void SetFont(HFONT);

		UINT_PTR SetTimer(UINT, UINT, TIMERPROC lpTimerFunc = NULL);
		void     KillTimer(UINT);

		HCURSOR LoadCursorStd(LPCTSTR);
		HCURSOR LoadCursorResource(UINT);
		HBITMAP LoadBitmapResource(UINT);
		HICON   LoadIconResource(UINT);

		int MessageBox(LPCTSTR, LPCTSTR, UINT);

		LRESULT SendMessage(UINT, WPARAM wParam = 0, LPARAM lParam = 0);
		LRESULT SendItemMessage(int, UINT, WPARAM, LPARAM);

		HWND GetPreviousSibling();
		HWND GetNextSibling();
		void FitAfterSibling(LONG x = 10, LONG y = 10, LONG diffX = 0, LONG diffY = 0);

		UINT GetID();
		HWND GetItem(int);
		void ShowItem(int);
		void HideItem(int);
		void EnableItem(int, bool bEnable = true);
		void DisableItem(int);
		void SetItemText(int, LPCTSTR);
		void GetItemText(int, LPTSTR);
		int  GetItemTextLength(int);
		void SetTextLimit(UINT);
		UINT GetTextLimit();

		void MakeBorderless();

		LONG SetStyleEx(DWORD);
		LONG AddStyleEx(DWORD);

		std::wstring AsString();
	};
}