#include "../clientwnd.h"
#include "../transdep/control.h"

#include "message.h"

ClientMessage::ClientMessage()
	: CPicturePanel()
{
	SetClass(TEXT("KioskMan::Client::MessageWindow"));

	onPaint = DefaultPainter;
}

BOOL ClientMessage::Create(HINSTANCE hInst, HWND hParent, RECT rect, DWORD WinStyle, DWORD ExWinStyle)
{
	if (!Register(CS_DBLCLKS | CS_OWNDC | CS_SAVEBITS, hInst))
		return FALSE;

	return CreateEx(ExWinStyle, TEXT(""), WinStyle, rect, hParent, NULL, hInst, this);
}

void ClientMessage::SetImage(HBITMAP hBmp)
{
	AssignBitmap(hBmp);
}

void ClientMessage::SetImage(LPCTSTR FileName)
{
	SetImage(Bmp::LoadGDI(FileName));
}

LPVOID ClientMessage::DefaultPainter(HWND hWnd, HDC hDC, PAINTSTRUCT pStruct)
{
	ClientMessage *panel = reinterpret_cast<ClientMessage*>(GetUserDataLong(hWnd));
	if (panel)
	{
		RECT rect = panel->GetRect();
		if (panel->GetBitmap())
		{
			HDC dcImage = ::CreateCompatibleDC(hDC);
			if (dcImage != NULL)
			{
				::SelectObject(dcImage, panel->GetBitmap());

				SIZE bmpSize = panel->GetBitmapDimension();

				::BitBlt(hDC, (coord::width(rect) - bmpSize.cx) / 2, 50, bmpSize.cx, bmpSize.cy, dcImage, 0, 0, SRCCOPY);
				::DeleteDC(dcImage);
			}
		}

		if (panel->GetTextLength() > 0)
		{
			::SetBkMode(hDC, TRANSPARENT);
			::SelectObject(hDC, cfg.Font(42));

			rect.left += 100;
			rect.right -= 100;
			rect.top = rect.bottom - 200;
			rect.bottom = rect.top + 150;

			WCHAR *szBuffer = new WCHAR[(panel->GetTextLength()) * sizeof(WCHAR)];
			panel->GetText(szBuffer);
			::DrawText(hDC, szBuffer, -1, &rect, DT_CENTER | DT_VCENTER);
			delete [] szBuffer;
		}
	}
	return 0L;
}

ClientAnimate::ClientAnimate()
	: CPanel()
{
	SetClass(TEXT("KioskMan::Client::AnimationWindow"));
}

BOOL ClientAnimate::Create(HINSTANCE hInst, HWND hParent, RECT rect, DWORD WinStyle, DWORD ExWinStyle)
{
	if (!Register(CS_DBLCLKS | CS_OWNDC | CS_SAVEBITS, hInst))
		return FALSE;

	return CreateEx(ExWinStyle, TEXT(""), WinStyle, rect, hParent, NULL, hInst, this);
}

void ClientAnimate::SetAnimation(LPCTSTR szGifFile)
{
	animator.LoadFile(szGifFile);
	animator.Center();
	animator.Show();
}

void ClientAnimate::SetRect(RECT rect)
{
	CWndBasic::SetRect(rect);
	animator.Center();
}
/*
ClientNotice::ClientNotice()
	: ClientMessage()
{
	SetClass(TEXT("KioskMan::Client::NoticeWindow"));

	onCreate  = OnCreate;
	onPaint   = NoticePainter;
	onCommand = CButton::OnParentCommand;
}

LPVOID ClientNotice::OnCreate(HWND hWnd, LPARAM lParam)
{
	ClientNotice *panel = reinterpret_cast<ClientNotice*>(GetUserDataLong(hWnd));
	if (panel)
	{
		RECT rect = panel->GetClientRect();
		rect.left = rect.left - 150 + coord::width(rect) / 2;
		rect.top = rect.bottom - 500;
		rect.right = rect.left + 300;
		rect.bottom = rect.top + 100;

		panel->OK.Initialize(panel->GetInstance(), *panel, TEXT("Ойлголоо"), rect);
		//, cfg.Font(32))
		panel->OK.SetHoverClick(OkClicked);
		panel->OK.Hide();

		rect.left -= 175;
		rect.right = rect.left + 300;
		panel->Retry.Initialize(panel->GetInstance(), *panel, TEXT("Дахин оролдох"), rect);
		panel->Retry.SetHoverClick(RetryClicked);
		panel->Retry.Hide();

		rect.left += 375;
		rect.right = rect.left + 300;
		panel->Cancel.Initialize(panel->GetInstance(), *panel, TEXT("Болих"), rect, WS_CHILD | WS_CLIPSIBLINGS);
		panel->Cancel.SetHoverClick(CancelClicked);
		panel->Cancel.Hide();
	}
	return 0L;
}

LPVOID ClientNotice::NoticePainter(HWND hWnd, HDC hDC, PAINTSTRUCT pStruct)
{
	ClientNotice *panel = reinterpret_cast<ClientNotice*>(GetUserDataLong(hWnd));
	if (panel)
	{
		RECT rect = panel->GetRect();
		if (panel->GetBitmap())
		{
			HDC dcImage = ::CreateCompatibleDC(hDC);
			if (dcImage != NULL)
			{
				::SelectObject(dcImage, panel->GetBitmap());

				SIZE bmpSize = panel->GetBitmapDimension();

				::BitBlt(hDC, (coord::width(rect) - bmpSize.cx) / 2, 50, bmpSize.cx, bmpSize.cy, dcImage, 0, 0, SRCCOPY);
				::DeleteDC(dcImage);
			}
		}

		if (panel->GetTextLength() > 0)
		{
			::SetBkMode(hDC, TRANSPARENT);
			::SelectObject(hDC, cfg.Font(38));

			rect.left += 100;
			rect.right -= 100;
			rect.top = rect.bottom - 200;
			rect.bottom = rect.top + 150;

			WCHAR *szBuffer = new WCHAR[(panel->GetTextLength()) * sizeof(WCHAR)];
			panel->GetText(szBuffer);
			::DrawText(hDC, szBuffer, -1, &rect, DT_CENTER | DT_VCENTER);
			delete [] szBuffer;
		}
	}
	return 0L;
}

void ClientNotice::OkClicked(HWND hWnd)
{
	Client.JumpStart();
}

void ClientNotice::RetryClicked(HWND hWnd)
{

}

void ClientNotice::CancelClicked(HWND hWnd)
{
	Client.JumpStart();
}
*/