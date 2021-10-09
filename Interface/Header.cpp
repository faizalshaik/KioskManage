#include "../clientwnd.h"

#include "header.h"

#pragma comment(lib, "msimg32.lib")

CSessionTimer::CSessionTimer()
	: CBitmapWindow()
{
	SetClass(KIOSKMAN_HEADER_SESSION);

	Interval = 1;
	exp = RGB_RED;

	onTimer = OnTimer;
	onPaint = OnPaint;
}

CHeader::CHeader()
	: CPanel()
{
	SetClass(KIOSKMAN_HEADER);

	onCreate = OnCreate;
	onCtlColorStatic = OnCtlColorStatic;
}

LPVOID CHeader::OnCreate(HWND hWnd, LPARAM lParam)
{
	CHeader *me = reinterpret_cast<CHeader*>(GetUserDataLong(hWnd));
	if (me)
	{
		RECT rect = me->GetClientRect();
		RECT rcIcon = rect, rcStatus = rect, rcTimer = rect;
		rcIcon.right = rcIcon.left + 100;
		rcTimer.left = rcTimer.right - 100;
		rcStatus.left = rcIcon.right + 10;
		rcStatus.top += 15;
		rcStatus.right = rcTimer.left - 10;
		rcStatus.bottom -= 5;

		me->icon.Instant(me->GetInstance(), *me, rcIcon, NULL, IconClicked);
		me->icon.onPaint = CHeader::OnIconPaint;

		me->status.Create(me->GetInstance(), *me, TEXT_EMPTY, rcStatus, cfg.Font(34, 0, 0, 0, FW_BOLD), WS_CHILD | WS_VISIBLE | WS_GROUP | SS_NOTIFY | SS_LEFT);

		me->timer.Initialize(me->GetInstance(), *me, KIOSKMAN_TIMER, rcTimer);
		me->timer.AssignBitmap(BITMAP_TIMER);
		me->timer.Show();
	}
	return 0L;
}

void CHeader::Hide()
{
	timer.Stop();
	CWndBasic::Hide();
}

void CHeader::Show()
{
	SetBkBrush(Worker.brush);
	icon.SetBkBrush(Worker.brush);
	timer.SetBkBrush(Worker.brush);

	CWndBasic::Show();
}

void CHeader::init()
{
	icon.DeleteBitmap();
	SetStatus(L"");
}

void CHeader::SetStatus(LPCTSTR szText)
{
	HDC dc = ::GetDC(status);
	::SelectObject(dc, cfg.Font(34, 0, 0, 0, FW_BOLD));
	SIZE size = GetTextExtendPoint(szText, dc);
	SafeDeleteObject(dc);

	RECT stc = status.GetRect();
	stc.top = GetRect().top + ((coord::width(stc) < size.cx) ? 15 : 30);

	POINT point = { stc.left, stc.top };
	::ScreenToClient(*this, &point);

	status.MoveTo(point.x, point.y, TRUE);
	status.SetText(szText);
}

void CSessionTimer::Start(int seconds)
{
	Interval = seconds;
	SetTimer(TIMER_HEADER_SESSION, 1000);
}

void CSessionTimer::Stop()
{
	KillTimer(TIMER_HEADER_SESSION);
}

void CSessionTimer::GetRemaining(int *min, int *sec)
{
	double time = Interval / 60;
	*min = (int)::floor(time);
	*sec = (int)(Interval - (*min * 60));
}

bool CSessionTimer::IsExpiring()
{
	return Interval < 120;
}

void CHeader::IconClicked(HWND hWnd)
{
	if (Client.IsTransdep())
	{
		return;
	}
}

LPVOID CHeader::OnCtlColorStatic(HWND hWnd, HDC hStaticDC, HWND hStaticCtl)
{
	CHeader *me = reinterpret_cast<CHeader*>(GetUserDataLong(hWnd));
	if (me)
	{
		if (me->status.GetID() == GetWindowID(hStaticCtl))
		{
			::SetBkMode(hStaticDC, TRANSPARENT);
			::SetTextColor(hStaticDC, RGB_WHITE);

			return (LPVOID)Worker.brush;
		}
	}
	return 0L;
}

LPVOID CSessionTimer::OnTimer(HWND OwnerWnd, UINT uTimerId)
{
	CSessionTimer *timer = reinterpret_cast<CSessionTimer*>(GetUserDataLong(OwnerWnd));
	if (timer)
	{
		timer->Interval--;

		if (timer->Interval < 1)
			Client.JumpStart();
		else
			timer->Redraw();
	}

	return 0L;
}

LPVOID CSessionTimer::OnPaint(HWND hWnd, HDC hDC, PAINTSTRUCT pStruct)
{
	CSessionTimer *timer = reinterpret_cast<CSessionTimer*>(GetUserDataLong(hWnd));
	if (timer)
	{
		if (timer->GetBitmap())
		{
			HDC dcImage;
			dcImage = ::CreateCompatibleDC(hDC);
			if (dcImage)
			{
				RECT rc = timer->GetClientRect();
				SIZE bmp = timer->GetBitmapDimension();
				rc.left += 5 + coord::width(rc) / 2 - bmp.cx / 2;
				rc.top += 10;
				::SelectObject(dcImage, timer->GetBitmap());
				::TransparentBlt(
					hDC, rc.left, rc.top, bmp.cx, bmp.cy,
					dcImage, 0, 0, bmp.cx, bmp.cy, RGB_WHITE);
				::DeleteDC(dcImage);
			}
		}

		::SetBkMode(hDC, TRANSPARENT);
		::SetTextColor(hDC, timer->IsExpiring() ? timer->exp : RGB_WHITE);
		::SelectObject(hDC, cfg.Font(36, 0, 0, 0, FW_BOLD));

		RECT rect = timer->GetClientRect();
		rect.top += coord::height(rect) / 2 + 5;
		rect.left += 5;
		UINT minute = (UINT)::floor(timer->Interval / 60);
		UINT seconds = (UINT)(timer->Interval - minute * 60);
		::DrawText(hDC, (std::to_wstring(minute) + ((seconds < 10) ? TEXT(":0") : TEXT(":")) + std::to_wstring(seconds)).c_str(), -1, &rect, DT_CENTER | DT_SINGLELINE | DT_VCENTER);

		rect = timer->GetClientRect();
		rect.top += 5;
		rect.bottom -= 5;
		rect.right = rect.left + 5;
		::Rectangle(hDC, rect.left, rect.top, rect.right, rect.bottom);
	}
	return 0L;
}

LPVOID CHeader::OnIconPaint(HWND hWnd, HDC hDC, PAINTSTRUCT pStruct)
{
	CHoverBitmapWindow *icon = reinterpret_cast<CHoverBitmapWindow*>(GetUserDataLong(hWnd));
	if (icon)
	{
		if (icon->GetBitmap())
		{
			HDC dcImage;
			dcImage = ::CreateCompatibleDC(hDC);
			if (dcImage)
			{
				RECT rc = icon->GetClientRect();
				SIZE bmp = icon->GetBitmapDimension();
				rc.right -= 5;
				rc.left = rc.left + coord::width(rc) / 2 - bmp.cx / 2;
				rc.top = rc.top + coord::height(rc) / 2 - bmp.cy / 2;
				::SelectObject(dcImage, icon->GetBitmap());
				::TransparentBlt(
					hDC, rc.left, rc.top, bmp.cx, bmp.cy,
					dcImage, 0, 0, bmp.cx, bmp.cy, RGB_WHITE);
				::DeleteDC(dcImage);
			}
		}
		
		RECT rect = icon->GetClientRect();
		rect.top += 5;
		rect.bottom -= 5;
		rect.left = rect.right - 5;
		::Rectangle(hDC, rect.left, rect.top, rect.right, rect.bottom);
	}
	return 0L;
}