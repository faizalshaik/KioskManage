#include "../clientwnd.h"

#include "footer.h"

CFooterButton::CFooterButton()
	: CHoverBitmapWindow()
{
	SetClass(KIOSKMAN_FOOTER_BTN);

	pPos = fbpLeft;
	Background = NULL;
	Custom = NULL;
	onPaint = OnPaint;
	onEraseBackground = OnEraseBackground;
}

CFooterButton::~CFooterButton()
{
	SafeDeleteObject(Background);
	SafeDeleteObject(Custom);
}

CFooter::CFooter()
	: CPanel()
{
	SetClass(KIOSKMAN_FOOTER);

	onCreate = OnCreate;
	onHide = OnHide;
}

LPVOID CFooter::OnCreate(HWND hWnd, LPARAM lParam)
{
	CFooter *me = reinterpret_cast<CFooter*>(GetUserDataLong(hWnd));
	if (me)
	{
		me->InitButtons();
	}
	return 0L;
}

void CFooter::Show()
{
	SetBkBrush(Worker.brush);
	middle.SetBkBrush(Worker.brush);

	CWndBasic::Show();
}

void CFooter::InitButtons()
{
//	RECT rcWnd;
//	::GetWindowRect(this->m_hWnd, &rcWnd);

	RECT rect = GetClientRect();
	RECT rcLeft = coord::to_rect(rect.left + 10, rect.top + 10, 250, coord::height(rect) - 20);
	RECT rcMiddle = coord::to_rect(rect.left - 45 + coord::width(rect) / 2, rect.top, 90, 90);
	RECT rcRight = coord::to_rect(rect.right - 410, rect.top + 10, 400, coord::height(rect) - 20);

	left.Background = BRUSH_TERTIARY;
	left.Instant(GetInstance(), *this, rcLeft, NULL, LeftClicked, FALSE, TEXT_BACK_MN);
	left.AssignBitmap(BITMAP_BACK_ARROW);

	right.Background = BRUSH_TERTIARY;
	right.Instant(GetInstance(), *this, rcRight, NULL, RightClicked, FALSE, TEXT_CONTINUE_MN);
	right.AssignBitmap(BITMAP_NEXT_ARROW);
	right.pPos = fbpRight;

	if (middle.Register(CS_DBLCLKS | CS_OWNDC | CS_SAVEBITS, GetInstance()))
	{
		if (middle.Create(TEXT_EMPTY, WS_CHILD, rcMiddle, *this, NULL, GetInstance()))
		{
			middle.SetBkBrush(BRUSH_PRIMARY);
			middle.Show();
		}
	}
}

void CFooter::HideButtons()
{
	left.Hide();
	right.Hide();
}

void CFooter::Left(LPCTSTR szText)
{
	left.SetText(szText);
}

void CFooter::Right(LPCTSTR szText)
{
	right.SetText(szText);
}

LPVOID CFooter::OnHide(HWND hWnd, UINT)
{
	CFooter *me = reinterpret_cast<CFooter*>(GetUserDataLong(hWnd));
	if (me)
		me->HideButtons();

	return 0L;
}

void CFooter::LeftClicked(HWND hWnd)
{
	HideWnd(hWnd);

	if (Client.GetState() == ST_TRANSDEP_PAYPROCESSING)
	{
		Client.State(ST_TRANSDEP_SEATME);
	}
	else
	{
		Client.GoBack();
	}
}

void CFooter::RightClicked(HWND hWnd)
{
	HideWnd(hWnd);

	Client.GoNext();
}

LPVOID CFooterButton::OnEraseBackground(HWND hWnd, HDC hDC)
{
	CFooterButton *btn = reinterpret_cast<CFooterButton*>(GetUserDataLong(hWnd));
	if (btn)
	{
		if (btn->Background == NULL)
			btn->Background = static_cast<HBRUSH>(::SelectObject(hDC, ::GetStockObject(DC_BRUSH)));

		RECT rect = btn->GetClientRect();
		::SelectObject(hDC, btn->Background);
		::Rectangle(hDC, rect.left, rect.top, rect.right, rect.bottom);
		return (LPVOID)1;
	}
	return 0L;
}

void CFooterButton::ShowText(LPCTSTR szText)
{
	SetText(szText);
	Show();
	Redraw();
}

void CFooterButton::SetColor(HBRUSH hBrush)
{
	SafeDeleteObject(Background);
	Background = hBrush;
	Redraw();
}

CFooterButton* CFooterButton::Reset()
{
	SafeDeleteObject(Custom);
	return this;
}

LPVOID CFooterButton::OnPaint(HWND OwnerWnd, HDC hDC, PAINTSTRUCT pStruct)
{
	CFooterButton *btn = reinterpret_cast<CFooterButton*>(GetUserDataLong(OwnerWnd));
	if (btn)
	{
		if (btn->Background == NULL)
			btn->Background = static_cast<HBRUSH>(::SelectObject(hDC, ::GetStockObject(DC_BRUSH)));

		RECT rect = btn->GetClientRect();
		::SelectObject(hDC, PEN_FOOTER_BUTTON);
		::Rectangle(hDC, rect.left, rect.top, rect.right, rect.bottom);

		if (btn->GetBitmap() || btn->Custom)
		{
			HDC dcImage;
			dcImage = ::CreateCompatibleDC(hDC);
			if (dcImage)
			{
				::SelectObject(dcImage, btn->Custom ? btn->Custom : btn->GetBitmap());
				SIZE bmpSize = btn->Custom ? Bmp::GetSize(btn->Custom) : btn->GetBitmapDimension();
				POINT p;
				switch (btn->pPos)
				{
				case fbpRight:
				{
					p.x = rect.right - 10 - bmpSize.cx;
					p.y = rect.top + (coord::height(rect) / 2) - bmpSize.cy / 2;
					rect.right -= bmpSize.cx + 10;
				} break;
				default:
				{
					p.x = rect.left + 10;
					p.y = rect.top + (coord::height(rect) / 2) - bmpSize.cy / 2;
					rect.left += bmpSize.cx + 10;
				} break;
				}
				if (btn->Custom)
					::BitBlt(hDC, p.x, p.y, bmpSize.cx, bmpSize.cy, dcImage, 0, 0, SRCCOPY);
				else
					::TransparentBlt(
					hDC, p.x, p.y, bmpSize.cx, bmpSize.cy,
					dcImage, 0, 0, bmpSize.cx, bmpSize.cy, RGB_WHITE);
				::DeleteDC(dcImage);
			}
		}
		::SetBkMode(hDC, TRANSPARENT);
		::SetTextColor(hDC, RGB_WHITE);
		::SelectObject(hDC, cfg.Font(30));
		::DrawText(hDC, btn->AsString().c_str(), -1, &rect, DT_CENTER | DT_SINGLELINE | DT_VCENTER);
	}
	return 0L;
}

CHiddenExit::CHiddenExit()
	: CWnd()
{
	SetClass(TEXT_HIDDENEXITCLASS);

	Unlocking = 0;
	Key = TEXT_EMPTY;

	onPaint = OnPaint;
	onMouseDown = OnMouseDown;
	onMouseLeave = OnMouseLeave;
}

LPVOID CHiddenExit::OnPaint(HWND hWnd, HDC hDC, PAINTSTRUCT pStruct)
{
	CHiddenExit *exit = reinterpret_cast<CHiddenExit*>(GetUserDataLong(hWnd));
	if (exit)
	{
		if (CAN_UNLOCK(exit->Unlocking))
		{
			::SetBkMode(hDC, TRANSPARENT);
			::SelectObject(hDC, cfg.Font(24));
			RECT rect = exit->GetClientRect();
			::MoveToEx(hDC, rect.left + 30, rect.top + 5, NULL);
			::LineTo(hDC, rect.left + 30, rect.bottom - 5);
			::MoveToEx(hDC, rect.left + 60, rect.top + 5, NULL);
			::LineTo(hDC, rect.left + 60, rect.bottom - 5);
			::MoveToEx(hDC, rect.left + 5, rect.top + 30, NULL);
			::LineTo(hDC, rect.right - 5, rect.top + 30);
			::MoveToEx(hDC, rect.left + 5, rect.top + 60, NULL);
			::LineTo(hDC, rect.right - 5, rect.top + 60);
			::TextOut(hDC, 10, 4, TEXT_DIGIT_1, 1);
			::TextOut(hDC, 40, 4, TEXT_DIGIT_2, 1);
			::TextOut(hDC, 70, 4, TEXT_DIGIT_3, 1);
			::TextOut(hDC, 10, 34, TEXT_DIGIT_4, 1);
			::TextOut(hDC, 40, 34, TEXT_DIGIT_5, 1);
			::TextOut(hDC, 70, 34, TEXT_DIGIT_6, 1);
			::TextOut(hDC, 10, 64, TEXT_DIGIT_7, 1);
			::TextOut(hDC, 40, 64, TEXT_DIGIT_8, 1);
			::TextOut(hDC, 70, 64, TEXT_DIGIT_9, 1);
		}
	}
	return 0L;
}

LPVOID CHiddenExit::OnMouseDown(HWND hWnd, UINT mButton, WPARAM sState, int X, int Y)
{
	if (mButton == mButtonLeft)
	{
		CHiddenExit *exit = reinterpret_cast<CHiddenExit*>(GetUserDataLong(hWnd));
		if (exit)
		{
			if (CAN_UNLOCK(exit->Unlocking))
			{
				UINT digitX, digitY;
				if (X <= 30) { digitX = 1; }
				else if (X <= 60) { digitX = 2; }
				else { digitX = 3; }
				if (Y <= 30) { digitY = 0; }
				else if (Y <= 60) { digitY = 1; }
				else { digitY = 2; }
				exit->Key += std::to_wstring(digitX + digitY * 3);

				if (exit->Key == cfg.key)
				{
					::PostQuitMessage(0);
					return 0L;
				}
			}
			else
			{
				exit->Unlocking++;
			}
			exit->Redraw();
		}
	}
	return 0L;
}

LPVOID CHiddenExit::OnMouseLeave(HWND hWnd)
{
	CHiddenExit *exit = reinterpret_cast<CHiddenExit*>(GetUserDataLong(hWnd));
	if (exit)
	{
		exit->Unlocking = 0;
		exit->Key = TEXT_EMPTY;
		exit->Redraw();
	}
	return 0L;
}