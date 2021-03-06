/**
* Dlg.cpp
*
* @program		Global-Pay
* @author		АНКУ ХХК
* @copyright	Copyright (c) 2017 он, АНКУ ХХК.
* @version		0.1.0.0
* @date			2017.09.19
*/

#include "dlg.h"

using namespace globalpay;

CDialog::CDialog(void)
: CWndDef()
{
	SetClass(DIALOGCLASSNAME);
}

CDialog::~CDialog(void)
{
}

BOOL CDialog::InitialDialogProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    if (message == WM_INITDIALOG)
	{
		CDialog *thisObjectPtr = reinterpret_cast<CDialog*>(lParam);
		thisObjectPtr->m_hWnd = hWnd;
		::SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(thisObjectPtr));
		::SetWindowLongPtr(hWnd, DWLP_DLGPROC, reinterpret_cast<LONG_PTR>(CDialog::StaticDialogProc));
		return thisObjectPtr->ActualDlgProc(hWnd, message, wParam, lParam);
    }
    
	return FALSE;
}

BOOL CDialog::StaticDialogProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{	
	LONG_PTR lpUserData = GetUserDataLong(hWnd);
	if (lpUserData)
	{
		CDialog *thisObjectPtr = reinterpret_cast<CDialog*>(lpUserData);
		return thisObjectPtr->ActualDlgProc(hWnd, message, wParam, lParam);
	}
	return FALSE;
}

BOOL CDialog::ActualDlgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	BOOL   isReallyProcessed;
	LPVOID vpResult = MessageHandler(hWnd, message, wParam, lParam, &isReallyProcessed);
	if (isReallyProcessed)
		return reinterpret_cast<BOOL>(vpResult);
	else
		return FALSE;
}

BOOL CDialog::Create(HINSTANCE hInst, LPCTSTR lpTemplate, HWND hParentWnd)
{
	m_hWnd = CreateDialogParam(hInst, lpTemplate, hParentWnd, reinterpret_cast<DLGPROC>(InitialDialogProc), reinterpret_cast<LPARAM>(this));
	
	if (!m_hWnd)
		return FALSE;

	return TRUE;
}

BOOL CDialog::Create(HINSTANCE hInst, UINT resDlgId, HWND hParentWnd)
{
	return Create(hInst, MAKEINTRESOURCE(resDlgId), hParentWnd);
}

int CDialog::ShowModal(HINSTANCE hInst, LPCTSTR lpTemplate, HWND hParentWnd)
{
	return ::DialogBoxParam(hInst, lpTemplate, hParentWnd, reinterpret_cast<DLGPROC>(InitialDialogProc), reinterpret_cast<LPARAM>(this));
}

int CDialog::ShowModal(HINSTANCE hInst, UINT resDlgId, HWND hParentWnd)
{
	return ::DialogBoxParam(hInst, MAKEINTRESOURCE(resDlgId), hParentWnd, reinterpret_cast<DLGPROC>(InitialDialogProc), reinterpret_cast<LPARAM>(this));
}