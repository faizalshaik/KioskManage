/**
* Wnd.cpp
*
* @program		Global-Pay
* @author		АНКУ ХХК
* @copyright	Copyright (c) 2017 он, АНКУ ХХК.
* @version		0.1.0.0
* @date			2017.09.19
*/

#include "wnd.h"

using namespace globalpay;

CWnd::CWnd(void)
: CWndDef()
{
	SetClass(CWNDCLASS);
}

CWnd::~CWnd(void)
{
}

LRESULT CWnd::InitialWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (message == WM_NCCREATE)
	{
		LPCREATESTRUCT lpCreateStruct = reinterpret_cast<LPCREATESTRUCT>(lParam);
		void * lpCreateParam = lpCreateStruct->lpCreateParams;
		CWnd* thisObjectPtr = reinterpret_cast<CWnd*>(lpCreateParam);
		thisObjectPtr->m_hWnd = hWnd;
		::SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(thisObjectPtr));
		::SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(StaticWndProc));
		return thisObjectPtr->ActualWndProc(hWnd, message, wParam, lParam);
	}
	return ::DefWindowProc(hWnd, message, wParam, lParam);
}

LRESULT CWnd::StaticWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{	
	LONG_PTR lpUserData = GetUserDataLong(hWnd);
	if (lpUserData)
	{
		CWnd* thisObjectPtr = reinterpret_cast<CWnd*>(lpUserData);
		return thisObjectPtr->ActualWndProc(hWnd, message, wParam, lParam);
	}	
	return ::DefWindowProc(hWnd, message, wParam, lParam);
}

LRESULT CWnd::ActualWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	BOOL   isReallyProcessed;
	LPVOID vpResult = MessageHandler(hWnd, message, wParam, lParam, &isReallyProcessed);
	if (isReallyProcessed)
		return reinterpret_cast<LRESULT>(vpResult);
	else
		return ::DefWindowProc(hWnd, message, wParam, lParam);
}

BOOL CWnd::Register(UINT style, HINSTANCE hInst, HICON hIcon, HCURSOR hCursor, HBRUSH hbrBackground, LPCTSTR lpszMenuName)
{
	if (GetClass() == TEXT(""))
		return FALSE;

	WNDCLASS wndclass = { 0 };

	if (::GetClassInfo(hInst, GetClass(), &wndclass))
		return TRUE;

	wndclass.style = style;
	wndclass.lpfnWndProc = InitialWndProc;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = sizeof(CWnd*);
	wndclass.hInstance = hInst;
	wndclass.hIcon = hIcon;
	wndclass.hCursor = hCursor;
	wndclass.hbrBackground = hbrBackground;
	wndclass.lpszMenuName = lpszMenuName;
	wndclass.lpszClassName = GetClass();

	if (!::RegisterClass(&wndclass))
		return FALSE;

	return TRUE;
}

BOOL CWnd::RegisterEx(UINT style, HINSTANCE hInst, HICON hIcon, HICON hIconSmall, HCURSOR hCursor, HBRUSH hbrBackground, LPCTSTR lpszMenuName)
{
	if (GetClass() == TEXT(""))
		return FALSE;

	WNDCLASSEX wndclassEx = { 0 };

	wndclassEx.cbSize = sizeof(wndclassEx);

	if (::GetClassInfoEx(hInst, GetClass(), &wndclassEx))
		return TRUE;

	wndclassEx.style = style;
	wndclassEx.lpfnWndProc = InitialWndProc;
	wndclassEx.cbClsExtra = 0;
	wndclassEx.cbWndExtra = sizeof(CWnd*);
	wndclassEx.hInstance = hInst;
	wndclassEx.hIcon = hIcon;
	wndclassEx.hCursor = hCursor;
	wndclassEx.hbrBackground = hbrBackground;
	wndclassEx.lpszMenuName = lpszMenuName;
	wndclassEx.lpszClassName = GetClass();
    wndclassEx.hIconSm = hIconSmall;
	
	if (!::RegisterClassEx(&wndclassEx))
		return FALSE;
	
	return TRUE;
}

BOOL CWnd::Create(LPCTSTR szText, DWORD dwStyle, RECT rcRect, HWND hParentWnd, HMENU hMenu, HINSTANCE hInst, LPVOID lpParam)
{
	if (GetClass() == TEXT(""))
		return FALSE;

	bool bChildIdentified = false;
	if (hMenu == NULL)
	{
		if ((dwStyle & WS_CHILD) || (dwStyle & WS_CHILDWINDOW))
		{
			hMenu = reinterpret_cast<HMENU>(CHILD_ID);
			bChildIdentified = true;
		}
	}
	
	m_hWnd = ::CreateWindow(
		GetClass(), szText, dwStyle, rcRect.left, rcRect.top, coord::width(rcRect), coord::height(rcRect),
		hParentWnd, hMenu, hInst, lpParam != NULL ? lpParam : this);

	if (!GetHandle())
        return FALSE;

	if (bChildIdentified)
		CHILD_ID++;

	return TRUE;
}

BOOL CWnd::CreateEx(DWORD dwExStyle, LPCTSTR szText, DWORD dwStyle, RECT rcRect, HWND hParentWnd, HMENU hMenu, HINSTANCE hInst, LPVOID lpParam)
{
	if (GetClass() == TEXT(""))
		return FALSE;

	bool bChildIdentified = false;
	if (hMenu == NULL)
	{
		if ((dwStyle & WS_CHILD) || (dwStyle & WS_CHILDWINDOW))
		{
			hMenu = reinterpret_cast<HMENU>(CHILD_ID);
			bChildIdentified = true;
		}
	}	

	m_hWnd = ::CreateWindowEx(
		dwExStyle, GetClass(), szText, dwStyle, rcRect.left, rcRect.top, coord::width(rcRect), coord::height(rcRect),
		hParentWnd, hMenu, hInst, lpParam != NULL ? lpParam : this);

	if (!GetHandle())
        return FALSE;

	if (bChildIdentified)
		CHILD_ID++;

	return TRUE;
}