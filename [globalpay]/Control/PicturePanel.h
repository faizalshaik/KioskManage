/**
* CPicturePanel class - PicturePanel.h - Initial
*
* @program		Global-Pay
* @author		АНКУ ХХК
* @copyright	Copyright (c) 2017 он, АНКУ ХХК.
* @version		0.1.0.0
* @date			2017.09.19
*/

#pragma once

#include "../window/bitmapwnd.h"

namespace globalpay {
#define CPICTUREPANELCLASS TEXT("codesaur::picturepanel")

	class CPicturePanel : public CBitmapWindow
	{
	public:
		CPicturePanel(void);

		bool Initialize(HINSTANCE hInst, HWND hParent, RECT rcRect = coord::to_rect(20, 10, 200, 100), HBITMAP hBmp = NULL, LPCTSTR szText = TEXT("PicturePanel"), DWORD WINSTYLE = WS_CHILD);
	};
}