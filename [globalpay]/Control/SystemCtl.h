/**
* CSystemControl class - SystemCtl.h
*
* @program		Global-Pay
* @author		АНКУ ХХК
* @copyright	Copyright (c) 2017 он, АНКУ ХХК.
* @version		0.1.0.0
* @date			2017.09.19
*/

#pragma once

#include "../window/wnddef.h"

namespace globalpay {
	class CSystemControl : public CWndDef
	{
	public:
		static UINT_PTR CLASS_PTR;

		CSystemControl(void);

		static LRESULT CALLBACK ControlProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData);

		virtual ~CSystemControl(void) = 0;
		virtual bool Create(HINSTANCE, HWND, LPCTSTR, RECT, HFONT, DWORD, DWORD, LPVOID) = 0;

		void SetText(LPCTSTR szText);
	};
}