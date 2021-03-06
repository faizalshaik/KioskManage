/**
* CPanel class - Panel.h - Initial
*
* @program		Global-Pay
* @author		АНКУ ХХК
* @copyright	Copyright (c) 2017 он, АНКУ ХХК.
* @version		0.1.0.0
* @date			2017.09.19
*/

#pragma once

#include "../window/wnd.h"

namespace globalpay {
#define CPANELCLASS TEXT("codesaur::panel")

	class CPanel : public CWnd
	{
	public:
		CPanel(void);

		BOOL Initialize(HINSTANCE hInst, HWND hParent, RECT rcRect = coord::to_rect(20, 10, 200, 100), LPCTSTR szText = TEXT("Panel"), DWORD WINSTYLE = WS_CHILD);
		BOOL InitializeHollow(HINSTANCE hInst, HWND hParent, RECT rcRect = coord::to_rect(20, 10, 200, 100), LPCTSTR szText = TEXT("Panel"), DWORD WINSTYLE = WS_CHILD);
		BOOL InitializeEx(DWORD dwExStyle, HINSTANCE hInst, HWND hParent, RECT rcArea = coord::to_rect(20, 10, 200, 100), LPCTSTR szText = TEXT("Panel"), DWORD dwWinStyle = WS_CHILD | WS_CLIPSIBLINGS);
	};
}