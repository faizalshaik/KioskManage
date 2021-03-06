/**
* ListBox.cpp
*
* @program		Global-Pay
* @author		АНКУ ХХК
* @copyright	Copyright (c) 2017 он, АНКУ ХХК.
* @version		0.1.0.0
* @date			2017.09.19
*/

#include "listbox.h"

using namespace globalpay;

CListBox::CListBox(void)
	: CSystemControl()
{
	SetClass(CLISTBOXCLASS);
}

bool CListBox::Create(HINSTANCE hInst, HWND hParent, LPCTSTR szDefaultText, RECT rcRect, HFONT hFont, DWORD WINSTYLE, DWORD WINSTYLEEX, LPVOID lpParam)
{
	return CSystemControl::Create(hInst, hParent, szDefaultText, rcRect, hFont, WINSTYLE, WINSTYLEEX, lpParam);
}

void CListBox::AddString(LPCTSTR szStr)
{
	SendMessage(LB_ADDSTRING, 0, (LPARAM)szStr);
}

void CListBox::DeleteString(int nIndex)
{
	SendMessage(LB_DELETESTRING, nIndex, NULL);
}

void CListBox::InsertString(LPCTSTR szStr, int nIndex)
{
	SendMessage(LB_INSERTSTRING, nIndex, (LPARAM)szStr);
}

int CListBox::GetItemCount()
{
	return SendMessage(LB_GETCOUNT, 0, 0);
}

void CListBox::GetItemText(LPTSTR szBuf, UINT nItemId)
{
	SendMessage(LB_GETTEXT, (WPARAM)nItemId , (LPARAM)szBuf);
}

int CListBox::GetFirstSelectedIndex()
{
	int nCount = GetItemCount();
	int nSelected = -1;
	for (int i = 0; i < nCount; i++)
	{
		if (SendMessage(LB_GETSEL, i, 0) > 0)
		{
			nSelected = i;
			break;
		}
	}
	return nSelected;
}

void CListBox::SelectIndexMulti(int nIndex, BOOL bHighlight)
{
	SendMessage(LB_SETSEL, (WPARAM)bHighlight, (LPARAM)nIndex);
}

void CListBox::DeselectAllMulti()
{
	SendMessage(LB_SETSEL, (WPARAM)FALSE, (LPARAM)-1);
}