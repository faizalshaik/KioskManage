/**
* Window related functions - WndFunc.h
*
* @program		Global-Pay
* @author		АНКУ ХХК
* @copyright	Copyright (c) 2017 он, АНКУ ХХК.
* @version		0.1.0.0
* @date			2017.09.19
*/

#pragma once

#include <windows.h>
#include <commctrl.h>

#pragma comment(lib, "comctl32.lib")

#include "../globalpay.hpp"
#include "../types/coordinate.h"

#define GetUserDataLong(x) ::GetWindowLongPtr(x, GWLP_USERDATA)
#define GetWindowID(x)     ::GetWindowLong(x, GWL_ID)

BOOL IsWindowExists(LPCTSTR, LPCTSTR, BOOL bRestoreIfExists = FALSE);
BOOL IsWindowWithChildExists(LPCTSTR, LPCTSTR, BOOL bRestoreIfExists = FALSE);

#define	mButtonLeft	  0
#define	mButtonRight  1
#define	mButtonMiddle 2
#define	mButtonX1	  3
#define	mButtonX2	  4

HINSTANCE GetInstanceWnd(HWND);
HWND GetParentWnd(HWND);
HWND GetChildWnd(HWND);
HWND GetEnabledPopupWnd(HWND);
HWND GetFirstWnd(HWND);
HWND GetLastWnd(HWND);
HWND GetNextWnd(HWND);
HWND GetPrevWnd(HWND);
HWND GetOwnerWnd(HWND);

void ShowWnd(HWND);
void ShowNormalWnd(HWND);
void RestoreWnd(HWND);
void HideWnd(HWND);
void MinimizeWnd(HWND);
void MaximizeWnd(HWND);
void ActivateWnd(HWND);

void InvalidateWnd(HWND, bool bErase = false);
void InvalidateRectWnd(HWND, RECT, bool bErase = false);
void InvalidateRgnWnd(HWND, HRGN, bool bErase = false);
void ValidateWnd(HWND);
void ValidateRectWnd(HWND, RECT);
void ValidateRgnWnd(HWND, HRGN);
void UpdateWnd(HWND);
void RedrawWnd(HWND, UINT rdFlags = RDW_ERASE|RDW_INVALIDATE);
void RedrawRectWnd(HWND, RECT, UINT rdFlags = RDW_ERASE|RDW_INVALIDATE);
void RedrawRgnWnd(HWND, HRGN, UINT rdFlags = RDW_ERASE|RDW_INVALIDATE);

void SetCursorWnd(HWND, HCURSOR);
void SetIconWnd(HWND, HICON);
void SetIconSmWnd(HWND, HICON);
void SetBkBrushWnd(HWND, HBRUSH);
void SetParentBkBrushWnd(HWND, HBRUSH);

void GetTextWnd(HWND, LPTSTR, UINT nLength = 0);
void SetTextWnd(HWND, LPCTSTR);

int GetWidthWnd(HWND);
int	GetHeightWnd(HWND);
RECT GetRectWnd(HWND);
RECT GetClientRectWnd(HWND);
RECT GetParentRectWnd(HWND);
void SetRegionWnd(HWND, HRGN, bool bRepaint = false);
void SetRectWnd(HWND, RECT, UINT uFlags = SWP_NOZORDER);
void SetLeftWnd(HWND, int, UINT uFlags = (SWP_NOZORDER|SWP_NOSIZE|SWP_NOREDRAW));
void SetTopWnd(HWND, int, UINT uFlags = (SWP_NOZORDER|SWP_NOSIZE|SWP_NOREDRAW));
void SetLeftTopWnd(HWND, int, int, UINT uFlags = (SWP_NOZORDER|SWP_NOSIZE|SWP_NOREDRAW));
void SetRightWnd(HWND, int, UINT uFlags = (SWP_NOZORDER|SWP_NOMOVE|SWP_NOREDRAW));
void SetBottomWnd(HWND, int, UINT uFlags = (SWP_NOZORDER|SWP_NOMOVE|SWP_NOREDRAW));
void SetWidthWnd(HWND, UINT, UINT uFlags = (SWP_NOZORDER|SWP_NOMOVE|SWP_NOREDRAW));
void SetHeightWnd(HWND, UINT, UINT uFlags = (SWP_NOZORDER|SWP_NOMOVE|SWP_NOREDRAW));
void SetSizeWnd(HWND, UINT, UINT, UINT uFlags = (SWP_NOZORDER|SWP_NOMOVE|SWP_NOREDRAW));

BOOL MoveWnd(HWND, int, int, int, int, bool bRepaint = false);
BOOL MoveWnd(HWND, int, int, bool bRepaint = false);
BOOL MoveWnd(HWND, RECT, bool bRepaint = false);
void CenterWnd(HWND, bool bDesktop = false, bool bRepaint = false);

BOOL StartTrackMouseLeaveWnd(HWND);
BOOL IsMouseOverWnd(HWND, const bool inClientSpace = false);