/**
* Msg.cpp
*
* @program		Global-Pay
* @author		АНКУ ХХК
* @copyright	Copyright (c) 2017 он, АНКУ ХХК.
* @version		0.1.0.0
* @date			2017.09.19
*/

#include "msg.h"

using namespace globalpay;

CMsg::CMsg(void)
{
	m_bMouseTracking = FALSE;

	onInitDialog = NULL;
	onCreate = NULL;
	onClose = NULL;
	onDestroy = NULL;
	onShow = NULL;
	onHide = NULL;
	onClose = NULL;
	onCommand = NULL;
	onSetText = NULL;
	onMove = NULL;
	onSize = NULL;
	onGetMinMaxInfo = NULL;
	onPaint = NULL;
	onEraseBackground = NULL;
	onCtlColorDialog = NULL;
	onCtlColorButton = NULL;
	onCtlColorStatic = NULL;
	onMouseMove = NULL;
	onMouseLeave = NULL;
	onMouseDown = NULL;
	onMouseUp = NULL;
	onMouseDblClick = NULL;
	onKeyDown = NULL;
	onKeyUp = NULL;
	onTimer = NULL;
	onPowerBroadcast = NULL;
	onRasDialFunc = NULL;
	onChangeLanguage = NULL;
	onRefresh = NULL;
	onKillFocus = NULL;
	onActivate = NULL;

	custom = NULL;
	cmessages = 0;
}

CMsg::~CMsg(void)
{
	SafeDelete(custom);
}

CustomMessage::CustomMessage()
{
	Function = NULL;
}

LPVOID CALLBACK CMsg::MessageHandler(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam, BOOL *bProcessed)
{
	*bProcessed = TRUE;
	switch (message)
	{
	case WM_INITDIALOG:
		if (onInitDialog != NULL)
			return (*onInitDialog)(hWnd, wParam, lParam);
		break;
	case WM_CREATE:
		if (onCreate != NULL)
			return (*onCreate)(hWnd, lParam);
		break;
	case WM_DESTROY:
		if (onDestroy != NULL)
			return (*onDestroy)(hWnd);
		break;
	case WM_SHOWWINDOW:
		if ((BOOL)wParam)
		{
			if (onShow != NULL)
				return (*onShow)(hWnd, (UINT)lParam);
		}
		else
			if (onHide != NULL)
				return (*onHide)(hWnd, (UINT)lParam);
		break;
	case WM_CLOSE:
		if (onClose != NULL)
			return (*onClose)(hWnd);
		break;
	case WM_MOVE:
		if (onMove != NULL)
			return (*onMove)(hWnd, (int)LOWORD(lParam), (int)HIWORD(lParam));
		break;
	case WM_COMMAND:
		if (onCommand != NULL)
			return (*onCommand)(hWnd, wParam, lParam);
		break;
	case WM_SETTEXT:
		if (onSetText != NULL)
			return (*onSetText)(hWnd, wParam, lParam);
		break;
	case WM_PAINT:
		if (onPaint != NULL)
		{
			PAINTSTRUCT ps;
			HDC hdc = ::BeginPaint(hWnd, &ps);
			LPVOID result = (*onPaint)(hWnd, hdc, ps);
			::EndPaint(hWnd, &ps);
			return result;
		} break;
	case WM_ERASEBKGND:
		if (onEraseBackground != NULL)
			return (*onEraseBackground)(hWnd, (HDC)wParam);
		break;
	case WM_CTLCOLORDLG:
		if (onCtlColorDialog != NULL)
			return (*onCtlColorDialog)((HWND)lParam, (HDC)wParam);
	case WM_CTLCOLORBTN:
		if (onCtlColorButton != NULL)
			return (*onCtlColorButton)(hWnd, (HDC)wParam, (HWND)lParam);
		break;
	case WM_CTLCOLORSTATIC:
		if (onCtlColorStatic != NULL)
			return (*onCtlColorStatic)(hWnd, (HDC)wParam, (HWND)lParam);
		break;
	case WM_SIZE:
		if (onSize != NULL)
			return (*onSize)(hWnd, (UINT)wParam, (int)(short)LOWORD(lParam), (int)(short)HIWORD(lParam));
		break;
	case WM_GETMINMAXINFO:
		if (onGetMinMaxInfo != NULL)
			return (*onGetMinMaxInfo)(hWnd, (LPMINMAXINFO)lParam);
		break;
	case WM_MOUSEMOVE:
	{
		if (!IsMouseTracking())
		{
			if (StartTrackMouseLeaveWnd(hWnd))
				SetMouseTracking(TRUE);
		}
		if (onMouseMove != NULL)
			return (*onMouseMove)(hWnd, wParam, (int)(short)LOWORD(lParam), (int)(short)HIWORD(lParam));
	} break;
	case WM_MOUSELEAVE:
	{
		m_bMouseTracking = FALSE;
		if (onMouseLeave != NULL)
			return (*onMouseLeave)(hWnd);
	} break;
	case WM_LBUTTONDOWN:
		if (onMouseDown != NULL)
			return (*onMouseDown)(hWnd, mButtonLeft, wParam, (int)(short)LOWORD(lParam), (int)(short)HIWORD(lParam));
		break;
	case WM_LBUTTONUP:
		if (onMouseUp != NULL)
			return (*onMouseUp)(hWnd, mButtonLeft, wParam, (int)(short)LOWORD(lParam), (int)(short)HIWORD(lParam));
		break;
	case WM_MBUTTONDOWN:
		if (onMouseDown != NULL)
			return (*onMouseDown)(hWnd, mButtonMiddle, wParam, (int)(short)LOWORD(lParam), (int)(short)HIWORD(lParam));
		break;
	case WM_MBUTTONUP:
		if (onMouseUp != NULL)
			return (*onMouseUp)(hWnd, mButtonMiddle, wParam, (int)(short)LOWORD(lParam), (int)(short)HIWORD(lParam));
		break;
	case WM_RBUTTONDOWN:
		if (onMouseDown != NULL)
			return (*onMouseDown)(hWnd, mButtonRight, wParam, (int)(short)LOWORD(lParam), (int)(short)HIWORD(lParam));
		break;
	case WM_RBUTTONUP:
		if (onMouseUp != NULL)
			return (*onMouseUp)(hWnd, mButtonRight, wParam, (int)(short)LOWORD(lParam), (int)(short)HIWORD(lParam));
		break;
	case WM_LBUTTONDBLCLK:
		if (onMouseDblClick != NULL)
			return (*onMouseDblClick)(hWnd, mButtonLeft, wParam, (int)(short)LOWORD(lParam), (int)(short)HIWORD(lParam));
		break;
	case WM_RBUTTONDBLCLK:
		if (onMouseDblClick != NULL)
			return (*onMouseDblClick)(hWnd, mButtonRight, wParam, (int)(short)LOWORD(lParam), (int)(short)HIWORD(lParam));
		break;
	case WM_MBUTTONDBLCLK:
		if (onMouseDblClick != NULL)
			return (*onMouseDblClick)(hWnd, mButtonMiddle, wParam, (int)(short)LOWORD(lParam), (int)(short)HIWORD(lParam));
		break;
	case WM_KEYDOWN:
		if (onKeyDown != NULL)
			return (*onKeyDown)(hWnd, (UINT)wParam, (UINT)lParam);
		break;
	case WM_KEYUP:
		if (onKeyUp != NULL)
			return (*onKeyUp)(hWnd, (UINT)wParam, (UINT)lParam);
		break;
	case WM_TIMER:
		if (onTimer != NULL)
			return (*onTimer)(hWnd, (UINT)wParam);
		break;
	case WM_TRAYICON:
		if (onTrayIcon)
			return (*onTrayIcon)(hWnd, (UINT)wParam, lParam);
	case WM_POWERBROADCAST:
		if (onPowerBroadcast != NULL)
			return (*onPowerBroadcast)(hWnd, wParam, lParam);
		break;
	case WM_RASDIALFUNC:
		if (onRasDialFunc != NULL)
			return (*onRasDialFunc)(hWnd, wParam, lParam);
		break;
	case WM_CHANGELANGUAGE:
		if (onChangeLanguage != NULL)
			return (*onChangeLanguage)(hWnd, (UINT)wParam);
		break;
	case WM_REFRESH:
		if (onRefresh != NULL)
			return (*onRefresh)(hWnd, wParam, lParam);
		break;
	case WM_KILLFOCUS:
		if (onKillFocus != NULL)
			return (*onKillFocus)(hWnd, (HWND)wParam);
		break;
	case WM_ACTIVATE:
		if (onActivate != NULL)
			return (*onActivate)(hWnd, (UINT)LOWORD(wParam), (HIWORD(wParam) == 0), (HWND)lParam);
		break;
	}

	for (UINT i = 0; i < cmessages; i++)
	{
		if (custom[i].Message == message)
			if (custom[i].Function != NULL)
				return (*custom[i].Function)(hWnd, wParam, lParam);
	}

	*bProcessed = FALSE;
	return 0L;
}

void CMsg::AllocCustomMessages(UINT size)
{
	SafeDelete(custom);

	cmessages = size;
	custom = new CustomMessage[cmessages];
}

bool CMsg::SetCustomMessage(UINT index, UINT msg, LPVOID(CALLBACK *func)(HWND hWnd, WPARAM wParam, LPARAM lParam))
{
	if (index >= cmessages)
		return false;

	custom[index].Message = msg;
	custom[index].Function = func;
	return true;
}

BOOL CMsg::IsMouseTracking()
{
	return m_bMouseTracking;
}

void CMsg::SetMouseTracking(const BOOL track)
{
	m_bMouseTracking = track;
}