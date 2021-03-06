/**
* WndFunc.cpp
*
* @program		Global-Pay
* @author		АНКУ ХХК
* @copyright	Copyright (c) 2017 он, АНКУ ХХК.
* @version		0.1.0.0
* @date			2017.09.19
*/

#include "wndfunc.h"

using namespace globalpay;

BOOL IsWindowExists(LPCTSTR lpClassName, LPCTSTR lpWndName, BOOL bRestoreIfExists)
{
	HWND hPrevWnd;
	if (hPrevWnd = ::FindWindow(lpClassName, lpWndName))
	{
		if (bRestoreIfExists)
		{
			::ShowWindow(hPrevWnd, SW_RESTORE);
			::SetForegroundWindow(hPrevWnd);
		}
		return TRUE;
	}

	return FALSE;
}

BOOL IsWindowWithChildExists(LPCTSTR lpClassName, LPCTSTR lpWndName, BOOL bRestoreIfExists) 
{
	HWND hPrevWnd, hChildWnd;

	if (hPrevWnd = ::FindWindow(lpClassName, lpWndName))
	{
		if (bRestoreIfExists)
		{
			hChildWnd = ::GetLastActivePopup(hPrevWnd);
			
			if (IsIconic(hPrevWnd))
				::ShowWindow(hPrevWnd, SW_RESTORE);
			
			if (IsWindow(hChildWnd))
				::SetForegroundWindow(hChildWnd);
		}
		return TRUE;
	}

	return FALSE;
}

HINSTANCE GetInstanceWnd(HWND hWnd)
{
	return (HINSTANCE)::GetWindowLongPtr(hWnd, GWLP_HINSTANCE);
}

HWND GetParentWnd(HWND hWnd)
{
	return (HWND)::GetWindowLongPtr(hWnd, GWLP_HWNDPARENT);
}

HWND GetChildWnd(HWND hWnd)
{
	return ::GetWindow(hWnd, GW_CHILD);
}

HWND GetEnabledPopupWnd(HWND hWnd)
{
	return ::GetWindow(hWnd, GW_ENABLEDPOPUP);
}

HWND GetFirstWnd(HWND hWnd)
{
	return ::GetWindow(hWnd, GW_HWNDFIRST);
}

HWND GetLastWnd(HWND hWnd)
{
	return ::GetWindow(hWnd, GW_HWNDLAST);
}

HWND GetNextWnd(HWND hWnd)
{
	return ::GetWindow(hWnd, GW_HWNDNEXT);
}

HWND GetPrevWnd(HWND hWnd)
{
	return ::GetWindow(hWnd, GW_HWNDPREV);
}

HWND GetOwnerWnd(HWND hWnd)
{
	return ::GetWindow(hWnd, GW_OWNER);
}

void ShowWnd(HWND hWnd)
{
	::ShowWindow(hWnd, SW_SHOW);
}

void ShowNormalWnd(HWND hWnd)
{
	::ShowWindow(hWnd, SW_SHOWNORMAL);
}

void RestoreWnd(HWND hWnd)
{
	::ShowWindow(hWnd, SW_RESTORE);
}

void HideWnd(HWND hWnd)
{
	::ShowWindow(hWnd, SW_HIDE);
}

void MinimizeWnd(HWND hWnd)
{
	::ShowWindow(hWnd, SW_MINIMIZE);
}

void MaximizeWnd(HWND hWnd)
{
	::ShowWindow(hWnd, SW_MAXIMIZE);
}

void ActivateWnd(HWND hWnd)
{
	::SendMessage(hWnd, WM_ACTIVATE, 0, 0);
}

void InvalidateWnd(HWND hWnd, bool bErase)
{
	::InvalidateRect(hWnd, NULL, bErase);
}

void InvalidateRectWnd(HWND hWnd, RECT rcArea, bool bErase)
{
	::InvalidateRect(hWnd, &rcArea, bErase);
}

void InvalidateRgnWnd(HWND hWnd, HRGN rcRgn, bool bErase)
{
	::InvalidateRgn(hWnd, rcRgn, bErase);
}

void ValidateWnd(HWND hWnd)
{
	::ValidateRect(hWnd, NULL);
}

void ValidateRectWnd(HWND hWnd, RECT rcArea)
{
	::ValidateRect(hWnd, &rcArea);
}

void ValidateRgnWnd(HWND hWnd, HRGN rcRgn)
{
	::ValidateRgn(hWnd, rcRgn);
}

void UpdateWnd(HWND hWnd)
{
	::UpdateWindow(hWnd);
}

void RedrawWnd(HWND hWnd, UINT rdFlags)
{
	::RedrawWindow(hWnd, NULL, NULL, rdFlags);
}

void RedrawRectWnd(HWND hWnd, RECT rcArea, UINT rdFlags)
{
	::RedrawWindow(hWnd, &rcArea, NULL, rdFlags);
}

void RedrawRgnWnd(HWND hWnd, HRGN hRgn, UINT rdFlags)
{
	::RedrawWindow(hWnd, NULL, hRgn, rdFlags);
}

void SetCursorWnd(HWND hWnd,HCURSOR hcur)
{
	if (hcur != NULL)
		::SetClassLong(hWnd, GCL_HCURSOR, (LONG) hcur);
}

void SetIconWnd(HWND hWnd, HICON hIco)
{
	if (hIco != NULL)
		::SetClassLong(hWnd, GCL_HICON, (LONG) hIco);
}

void SetIconSmWnd(HWND hWnd, HICON hIcoSmall)
{
	if (hIcoSmall != NULL)
		::SetClassLong(hWnd, GCL_HICONSM, (LONG) hIcoSmall);
}

void SetBkBrushWnd(HWND hWnd, HBRUSH newBrush)
{
	::SetClassLong(hWnd, GCL_HBRBACKGROUND, (DWORD) newBrush);
}

void SetParentBkBrushWnd(HWND hWnd, HBRUSH newBrush)
{
	::SetClassLong(GetParentWnd(hWnd), GCL_HBRBACKGROUND, (DWORD) newBrush);
}

void GetTextWnd(HWND hWnd, LPTSTR szText, UINT nLength)
{
	if (nLength == 0)
		nLength = ::GetWindowTextLength(hWnd);
	::GetWindowText(hWnd, szText, (nLength + 1) * sizeof(szText[0]));
}

void SetTextWnd(HWND hWnd, LPCTSTR szNewText)
{
	::SetWindowText(hWnd, szNewText);
}

int GetWidthWnd(HWND hWnd)
{
	return coord::width(GetRectWnd(hWnd));
}

int GetHeightWnd(HWND hWnd)
{
	return coord::height(GetRectWnd(hWnd));
}

RECT GetRectWnd(HWND hWnd)
{
	RECT rcRect;
	::GetWindowRect(hWnd, &rcRect);
	return rcRect;
}

RECT GetClientRectWnd(HWND hWnd)
{
	RECT rcClient;
	::GetClientRect(hWnd, &rcClient);
	return rcClient;
}

RECT GetParentRectWnd(HWND hWnd)
{
	RECT rcParent;
	::GetWindowRect(GetParentWnd(hWnd), &rcParent);
	return rcParent;
}

void SetRegionWnd(HWND hWnd, HRGN rgn, bool bRepaint)
{
	::SetWindowRgn(hWnd, rgn, bRepaint);
}

void SetRectWnd(HWND hWnd, RECT rcNewRect, UINT uFlags)
{
	::SetWindowPos(hWnd, NULL, rcNewRect.left, rcNewRect.top, rcNewRect.right - rcNewRect.left, rcNewRect.bottom - rcNewRect.top, uFlags);
}

void SetLeftWnd(HWND hWnd, int nX, UINT uFlags)
{
	RECT rcRect = GetRectWnd(hWnd);
	::SetWindowPos(hWnd, NULL, nX, rcRect.top, 0, 0, uFlags);
}

void SetTopWnd(HWND hWnd, int nY, UINT uFlags)
{
	RECT rcRect = GetRectWnd(hWnd);
	::SetWindowPos(hWnd, NULL, rcRect.left, nY, 0, 0, uFlags);
}

void SetLeftTopWnd(HWND hWnd, int nX, int nY, UINT uFlags)
{
	RECT rcRect = GetRectWnd(hWnd);
	::SetWindowPos(hWnd, NULL, nX, nY, 0, 0, uFlags);
}

void SetRightWnd(HWND hWnd, int ncX, UINT uFlags)
{
	RECT rcRect = GetRectWnd(hWnd);
	::SetWindowPos(hWnd, NULL, 0, 0, ncX - rcRect.left, coord::height(rcRect), uFlags);
}

void SetBottomWnd(HWND hWnd, int ncY, UINT uFlags)
{
	RECT rcRect = GetRectWnd(hWnd);
	::SetWindowPos(hWnd, NULL, 0, 0, coord::width(rcRect), ncY - rcRect.top, uFlags);
}

void SetWidthWnd(HWND hWnd, UINT nWidth, UINT uFlags)
{
	RECT rcRect = GetRectWnd(hWnd);
	::SetWindowPos(hWnd, NULL, 0, 0, nWidth, coord::height(rcRect), uFlags);
}

void SetHeightWnd(HWND hWnd, UINT nHeight, UINT uFlags)
{
	RECT rcRect = GetRectWnd(hWnd);
	::SetWindowPos(hWnd, NULL, 0, 0, coord::width(rcRect), nHeight, uFlags);
}

void SetSizeWnd(HWND hWnd, UINT nWidth, UINT nHeight, UINT uFlags)
{
	::SetWindowPos(hWnd, NULL, 0, 0, nWidth, nHeight, uFlags);
}

BOOL MoveWnd(HWND hWnd, int nX, int nY, int nWidth, int nHeight, bool bRepaint)
{
	return ::MoveWindow(hWnd, nX, nY, nWidth, nHeight, bRepaint);
}

BOOL MoveWnd(HWND hWnd, int nX, int nY, bool bRepaint)
{
	return MoveWnd(hWnd, nX, nY, GetWidthWnd(hWnd), GetHeightWnd(hWnd), bRepaint);	
}

BOOL MoveWnd(HWND hWnd, RECT rcRect, bool bRepaint)
{
	return MoveWnd(hWnd, rcRect.left, rcRect.top, coord::width(rcRect), coord::height(rcRect), bRepaint);
}

void CenterWnd(HWND hWnd, bool bDesktop, bool bRepaint)
{
	RECT rcRect = GetRectWnd(hWnd);
	RECT parentRect = GetClientRectWnd(GetParentWnd(hWnd));

	if (bDesktop)
		MoveWnd(hWnd, GetSystemMetrics(SM_CXSCREEN)/2 - coord::width(rcRect)/2, GetSystemMetrics(SM_CYSCREEN)/2 - coord::height(rcRect)/2, bRepaint);
	else
	{
		int left = coord::width(parentRect)/2 - coord::width(rcRect)/2;
		int top = coord::height(parentRect)/2 - coord::height(rcRect)/2;
		MoveWnd(hWnd, parentRect.left + left, parentRect.top + top, bRepaint);
	}
}

BOOL StartTrackMouseLeaveWnd(HWND hWnd)
{
	TRACKMOUSEEVENT tme = {sizeof(TRACKMOUSEEVENT), TME_LEAVE, hWnd, HOVER_DEFAULT};
	return _TrackMouseEvent(&tme);
}

BOOL IsMouseOverWnd(HWND hWnd, const bool inClientSpace)
{
	POINT cursorPos;
	::GetCursorPos(&cursorPos);

	RECT wndRect = GetRectWnd(hWnd);

	if (inClientSpace)
	{
		POINT offset;
		offset.x = offset.y = 0;

		::ClientToScreen(hWnd, &offset);

		wndRect.left -= offset.x;
		wndRect.top  -= offset.y;
		wndRect.right -= offset.x;
		wndRect.bottom -= offset.y;
	}

	return ::PtInRect(&wndRect, cursorPos);
}