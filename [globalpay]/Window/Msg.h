/**
* Windows Message handling object - Msg.h
*
* @program		Global-Pay
* @author		АНКУ ХХК
* @copyright	Copyright (c) 2017 он, АНКУ ХХК.
* @version		0.1.0.0
* @date			2017.09.19
*/

#pragma once

#include "wndbasic.h"

namespace globalpay {
#define voidTRUE		  reinterpret_cast<LPVOID>(TRUE)
#define voidFALSE         FALSE
#define WM_TRAYICON       (WM_USER + 4747)
#define WM_RASDIALFUNC    (WM_USER + 4748)
#define WM_CHANGELANGUAGE (WM_USER + 4749)
#define WM_REFRESH        (WM_USER + 4750)
#define WM_DELAY_TIMER    (WM_USER + 4751)

	struct CustomMessage
	{
		UINT Message;

		CustomMessage();
		
		LPVOID(CALLBACK *Function)(HWND, WPARAM, LPARAM);
	};

	class CMsg
	{
	public:
		CMsg(void);
		~CMsg(void);

		LPVOID(CALLBACK *onInitDialog)(HWND hWnd, WPARAM wParam, LPARAM lParam);
		LPVOID(CALLBACK *onCreate)(HWND hWnd, LPARAM lParam);
		LPVOID(CALLBACK *onDestroy)(HWND hWnd);
		LPVOID(CALLBACK *onShow)(HWND hWnd, UINT);
		LPVOID(CALLBACK *onHide)(HWND hWnd, UINT);
		LPVOID(CALLBACK *onClose)(HWND hWnd);
		LPVOID(CALLBACK *onCommand)(HWND hWnd, WPARAM wParam, LPARAM lParam);
		LPVOID(CALLBACK *onSetText)(HWND hWnd, WPARAM wParam, LPARAM lParam);
		LPVOID(CALLBACK *onMove)(HWND hWnd, int X, int Y);
		LPVOID(CALLBACK *onSize)(HWND hWnd, UINT nType, int cx, int cy);
		LPVOID(CALLBACK *onGetMinMaxInfo)(HWND hWnd, LPMINMAXINFO lpMinMaxInfo);
		LPVOID(CALLBACK *onPaint)(HWND hWnd, HDC hDC, PAINTSTRUCT pStruct);
		LPVOID(CALLBACK *onEraseBackground)(HWND hWnd, HDC hDC);
		LPVOID(CALLBACK *onCtlColorDialog)(HWND hDlg, HDC hDlgDC);
		LPVOID(CALLBACK *onCtlColorButton)(HWND hWnd, HDC hButtonDC, HWND hButtonCtl);
		LPVOID(CALLBACK *onCtlColorStatic)(HWND hWnd, HDC hStaticDC, HWND hStaticCtl);
		LPVOID(CALLBACK *onKeyDown)(HWND hWnd, UINT vKey, UINT vAlt);
		LPVOID(CALLBACK *onKeyUp)(HWND hWnd, UINT vKey, UINT vAlt);
		LPVOID(CALLBACK *onMouseMove)(HWND hWnd, WPARAM sState, int X, int Y);
		LPVOID(CALLBACK *onMouseLeave)(HWND hWnd);
		LPVOID(CALLBACK *onMouseDown)(HWND hWnd, UINT mButton, WPARAM sState, int X, int Y);
		LPVOID(CALLBACK *onMouseUp)(HWND hWnd, UINT mButton, WPARAM sState, int X, int Y);
		LPVOID(CALLBACK *onMouseDblClick)(HWND hWnd, UINT mButton, WPARAM sState, int X, int Y);
		LPVOID(CALLBACK *onTimer)(HWND hWnd, UINT uTimerId);
		LPVOID(CALLBACK *onTrayIcon)(HWND hWnd, UINT uNotifyIconId, LPARAM lParam);
		LPVOID(CALLBACK *onPowerBroadcast)(HWND hWnd, WPARAM wParam, LPARAM lParam);
		LPVOID(CALLBACK *onRasDialFunc)(HWND hWnd, WPARAM, LPARAM);
		LPVOID(CALLBACK *onChangeLanguage)(HWND hWnd, UINT uLanguageId);
		LPVOID(CALLBACK *onRefresh)(HWND hWnd, WPARAM wParam, LPARAM lParam);
		LPVOID(CALLBACK *onActivate)(HWND hWnd, UINT State, BOOL IsMinimized, HWND hOtherWnd);
		LPVOID(CALLBACK *onKillFocus)(HWND hWnd, HWND hFocusedWnd);

		BOOL IsMouseTracking();
		void SetMouseTracking(const BOOL track);

		void AllocCustomMessages(UINT);
		bool SetCustomMessage(UINT, UINT, LPVOID(CALLBACK *)(HWND, WPARAM, LPARAM));

	protected:
		LPVOID CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM, BOOL*);

	private:
		CustomMessage *custom;
		UINT           cmessages;
		BOOL           m_bMouseTracking;
	};
}