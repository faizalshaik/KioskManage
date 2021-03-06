/**
* WndBasic.cpp
*
* @program		Global-Pay
* @author		АНКУ ХХК
* @copyright	Copyright (c) 2017 он, АНКУ ХХК.
* @version		0.1.0.0
* @date			2017.09.19
*/

#include "wndbasic.h"

using namespace globalpay;

CWndBasic::CWndBasic(void)
{
	SetClass(CWNDBASICCLASS);

	m_hWnd = NULL;
}

CWndBasic::~CWndBasic(void)
{
	DestroyWindow();
}

HWND CWndBasic::GetHandle()
{
	return m_hWnd;
}

void CWndBasic::SetHandleForced(HWND hNewHandle)
{
	m_hWnd = hNewHandle;
}

CWndBasic::operator HWND()
{
	return GetHandle();
}

CWndBasic::operator UINT()
{
	return GetWindowID(*this);
}

HWND CWndBasic::GetParent()
{
	return GetParentWnd(GetHandle());
}

HINSTANCE CWndBasic::GetInstance()
{
	return GetInstanceWnd(GetHandle());
}

void CWndBasic::SetParent(HWND hNewParent)
{
	::SetParent(GetHandle(), hNewParent);
}

void CWndBasic::SetClass(LPTSTR classname)
{
	if (classname != TEXT(""))
		m_sClassName = classname;
}

LPTSTR CWndBasic::GetClass()
{
	return m_sClassName;
}

void CWndBasic::Show()
{
	ShowWnd(GetHandle());
}

void CWndBasic::ShowNormal()
{
	ShowNormalWnd(GetHandle());
}

void CWndBasic::Restore()
{
	RestoreWnd(GetHandle());
}

void CWndBasic::SetForeground()
{
	::SetForegroundWindow(GetHandle());
}

void CWndBasic::SetAfter(HWND hWnd)
{
	::SetWindowPos(*this, hWnd, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
}

BOOL CWndBasic::IsVisible()
{
	return ::IsWindowVisible(GetHandle());
}

void CWndBasic::Minimize()
{
	MinimizeWnd(GetHandle());
}

void CWndBasic::Maximize()
{
	MaximizeWnd(GetHandle());
}

void CWndBasic::Hide()
{
	HideWnd(GetHandle());
}

void CWndBasic::Close()
{
	::CloseWindow(GetHandle());
}

void CWndBasic::DestroyWindow()
{
	if (GetHandle())
		::DestroyWindow(GetHandle());
}

void CWndBasic::Activate()
{
	ActivateWnd(GetHandle());
}

void CWndBasic::Enable(bool bEnable)
{
	::EnableWindow(GetHandle(), bEnable);
}

void CWndBasic::Disable()
{
	::EnableWindow(GetHandle(), FALSE);
}

BOOL CWndBasic::IsEnabled()
{
	return ::IsWindowEnabled(GetHandle());
}

void CWndBasic::SetFocus()
{
	::SetFocus(GetHandle());
}

void CWndBasic::Invalidate(bool bErase)
{
	InvalidateWnd(GetHandle(), bErase);
}

void CWndBasic::InvalidateRect(RECT rcArea, bool bErase)
{
	InvalidateRectWnd(GetHandle(), rcArea, bErase);
}

void CWndBasic::InvalidateRegion(HRGN hRgn, bool bErase)
{
	InvalidateRgnWnd(GetHandle(), hRgn, bErase);
}

void CWndBasic::Validate()
{
	ValidateWnd(GetHandle());
}

void CWndBasic::ValidateRect(RECT rcArea)
{
	ValidateRectWnd(GetHandle(), rcArea);
}

void CWndBasic::ValidateRegion(HRGN hRgn)
{
	ValidateRgnWnd(GetHandle(), hRgn);
}

void CWndBasic::Update()
{
	UpdateWnd(GetHandle());
}

void CWndBasic::Redraw()
{
	RedrawWnd(GetHandle());
}

RECT CWndBasic::GetRect()
{
	return GetRectWnd(GetHandle());
}

RECT CWndBasic::GetClientRect()
{
	return GetClientRectWnd(GetHandle());
}

RECT CWndBasic::GetParentRect()
{
	return GetParentRectWnd(GetHandle());
}

RECT CWndBasic::GetParentClientRect()
{
	return GetClientRectWnd(GetParent());
}

void CWndBasic::SetRegion(HRGN rgn, bool bRepaint)
{
	SetRegionWnd(GetHandle(), rgn, bRepaint);
}

void CWndBasic::SetRect(RECT rcNewRect)
{
	SetRectWnd(GetHandle(), rcNewRect);
}

void CWndBasic::SetWidth(unsigned int nWidth)
{
	SetWidthWnd(GetHandle(), nWidth);
}

void CWndBasic::SetHeight(unsigned int nHeight)
{
	SetHeightWnd(GetHandle(), nHeight);
}

void CWndBasic::SetSize(unsigned int nWidth, unsigned int nHeight)
{
	SetSizeWnd(GetHandle(), nWidth, nHeight);
}

void CWndBasic::MoveTo(unsigned int x, unsigned int y, bool bRepaint)
{
	MoveWnd(GetHandle(), x, y, bRepaint);
}

void CWndBasic::Center(bool bDesktop, bool bRepaint)
{
	CenterWnd(GetHandle(), bDesktop, bRepaint);
}

void CWndBasic::GetText(LPTSTR szBuffer, UINT length)
{
	GetTextWnd(GetHandle(), szBuffer, length);
}

void CWndBasic::SetText(LPCTSTR szNewText)
{
	SetTextWnd(GetHandle(), szNewText);
}

int CWndBasic::GetTextLength()
{
	return ::GetWindowTextLength(GetHandle());
}

SIZE CWndBasic::GetTextExtendPoint(LPCTSTR szText, HDC hDC)
{
	if (szText == NULL)
		return { 0 };

	SIZE sz;
	::GetTextExtentPoint32(hDC != NULL ? hDC : GetDC(*this), szText, ::wcslen(szText), &sz);

	return sz;
}

void CWndBasic::SetCursor(HCURSOR hcur)
{
	SetCursorWnd(GetHandle(), hcur);
}

void CWndBasic::SetIcon(HICON hIco)
{
	SetIconWnd(GetHandle(), hIco);
}

void CWndBasic::SetIconSmall(HICON hIcoSmall)
{
	SetIconSmWnd(GetHandle(), hIcoSmall);
}

void CWndBasic::SetBkBrush(HBRUSH newBrush)
{
	SetBkBrushWnd(GetHandle(), newBrush);
}

void CWndBasic::SetCursorHand()
{
	SetCursor(::LoadCursor(NULL, IDC_HAND));
}

void CWndBasic::SetFont(HFONT hFont)
{
	SendMessage(WM_SETFONT, (WPARAM)hFont, (LPARAM)TRUE);
}

UINT_PTR CWndBasic::SetTimer(UINT uTimerId, UINT uElapse, TIMERPROC lpTimerFunc)
{
	return ::SetTimer(GetHandle(), uTimerId, uElapse, (TIMERPROC) lpTimerFunc);
}

void CWndBasic::KillTimer(UINT uTimerId)
{
	::KillTimer(GetHandle(), uTimerId);
}

HCURSOR CWndBasic::LoadCursorStd(LPCTSTR szCursorName)
{
	return ::LoadCursor(GetInstance(), szCursorName);
}

HCURSOR CWndBasic::LoadCursorResource(UINT nCursorId)
{
	return ::LoadCursor(GetInstance(), MAKEINTRESOURCE(nCursorId));
}

HBITMAP CWndBasic::LoadBitmapResource(UINT nResourceId)
{
	return ::LoadBitmap(GetInstance(), MAKEINTRESOURCE(nResourceId));
}

HICON CWndBasic::LoadIconResource(UINT nIconResource)
{
	return ::LoadIcon(GetInstance(), MAKEINTRESOURCE(nIconResource));
}

int CWndBasic::MessageBox(LPCTSTR lpText, LPCTSTR lpCaption, UINT uType)
{
	return ::MessageBox(GetHandle(), lpText, lpCaption, uType);
}

LRESULT CWndBasic::SendMessage(UINT Msg, WPARAM wParam, LPARAM lParam)
{
	return ::SendMessage(GetHandle(), Msg, wParam, lParam);
}

LRESULT CWndBasic::SendItemMessage(int nItemId, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	return ::SendDlgItemMessage(GetHandle(), nItemId, Msg, wParam, lParam);
}

HWND CWndBasic::GetPreviousSibling()
{
	return ::GetNextWindow(*this, GW_HWNDPREV);
}

HWND CWndBasic::GetNextSibling()
{
	return ::GetNextWindow(*this, GW_HWNDNEXT);
}

void CWndBasic::FitAfterSibling(LONG x, LONG y, LONG diffX, LONG diffY)
{
	HWND sibling = GetPreviousSibling();
	if (sibling)
	{
		POINT sibpoint = { 0 };
		::MapWindowPoints(sibling, GetParent(), &sibpoint, 1);

		RECT sibrect = GetRectWnd(sibling);
		RECT parent = GetParentRect();
		RECT rect = GetRect();

		LONG width = coord::width(rect);
		LONG height = coord::height(rect);

		if ((sibrect.right - parent.left + x + width) < coord::width(parent))
		{
			x += sibrect.right - parent.left + diffX;
			y = sibpoint.y + diffY;
		}
		else
			y += sibrect.bottom - parent.top + diffY;

		MoveTo(x, y);
	}
}

UINT CWndBasic::GetID()
{
	return GetWindowID(GetHandle());
}

HWND CWndBasic::GetItem(int nItemId)
{
	return ::GetDlgItem(GetHandle(), nItemId);
}

void CWndBasic::ShowItem(int nItem)
{
	ShowWnd(GetItem(nItem));
}

void CWndBasic::HideItem(int nItem)
{
	HideWnd(GetItem(nItem));
}

void CWndBasic::EnableItem(int nItemId, bool bEnable)
{
	::EnableWindow(GetItem(nItemId), bEnable);
}

void CWndBasic::DisableItem(int nItemId)
{
	EnableItem(nItemId, false);
}

void CWndBasic::SetItemText(int nItemId, LPCTSTR szText)
{
	::SetTextWnd(GetItem(nItemId), szText);
}

void CWndBasic::GetItemText(int nItemId, LPTSTR szText)
{
	::GetTextWnd(GetItem(nItemId), szText);
}

int CWndBasic::GetItemTextLength(int nItemId)
{
	return ::GetWindowTextLength(GetItem(nItemId));
}

void CWndBasic::SetTextLimit(UINT nLimit)
{
	SendMessage(EM_LIMITTEXT, nLimit);
}

UINT CWndBasic::GetTextLimit()
{
	return SendMessage(EM_GETLIMITTEXT);
}

void CWndBasic::MakeBorderless()
{
	::SetWindowLong(GetHandle(), GWL_EXSTYLE, 0);
}

LONG CWndBasic::SetStyleEx(DWORD exStyle)
{
	return ::SetWindowLong(*this, GWL_EXSTYLE, exStyle);
}

LONG CWndBasic::AddStyleEx(DWORD exStyle)
{
	LONG CurrentExStyle = ::GetWindowLong(*this, GWL_EXSTYLE);
	CurrentExStyle &= exStyle;
	return ::SetWindowLong(*this, GWL_EXSTYLE, CurrentExStyle);
}

std::wstring CWndBasic::AsString()
{
	UINT nTextLength = GetTextLength();
	std::wstring szText;	
	if (nTextLength > 0)
	{
		nTextLength++;

		WCHAR *szBuffer = static_cast<WCHAR*>(::malloc(nTextLength * sizeof(WCHAR*)));
		::GetWindowText(GetHandle(), szBuffer, nTextLength);
		szText = szBuffer;
		::free(szBuffer);
	}
	return szText;
}