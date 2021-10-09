/**
* Dialog Box Object - Dlg.h
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
#define DIALOGCLASSNAME WC_DIALOG

	class CDialog : public CWndDef
	{
	public:
		CDialog(void);
		~CDialog(void);

	protected:
		static BOOL CALLBACK InitialDialogProc(HWND, UINT, WPARAM, LPARAM);
		static BOOL CALLBACK StaticDialogProc(HWND, UINT, WPARAM, LPARAM);
		BOOL CALLBACK ActualDlgProc(HWND, UINT, WPARAM, LPARAM);

	public:
		BOOL Create(HINSTANCE, LPCTSTR, HWND hParentWnd = NULL);
		BOOL Create(HINSTANCE hInst, UINT resDlgId, HWND hParentWnd = NULL);
		int ShowModal(HINSTANCE, LPCTSTR, HWND hParentWnd = NULL);
		int ShowModal(HINSTANCE hInst, UINT resDlgId, HWND hParentWnd = NULL);
	};
}