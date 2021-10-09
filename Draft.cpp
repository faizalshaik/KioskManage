// original name : ClientWnd.cpp
// more info @   : ClientWnd.h

#include "stdafx.h"

#include "resource.h"

#include "clientwnd.h"

CClientWnd Client;

CClientWnd::CClientWnd(void)
	: CBitmapWindow()
{
	SetClass(KIOSKMAN_CLIENT);

	Active = ST_UNDEFINED;

	onCreate = OnCreate;
	onPaint = OnPaint;
}

BOOL CClientWnd::State(MACHINE_STATE state)
{
	Last = GetState();
	Active = state;

	if (!IsStateChanged())
		return FALSE;

	HidePopups();

	if (IsMain())
	{
		menu.Show();

		Header.timer.Stop();

		Header.Hide();
		Footer.Hide();
		Worker.Hide();

		if (!Keyboard.IsMongolian())
		{
			Keyboard.SetLanguage(LANG_MONGOLIAN);
			Keyboard.CreateKeys();
		}

		Redraw();

		StopServiceThreads();

		return TRUE;
	}
	else if (GetLastState() == ST_ROOT)
	{
		Redraw();

		menu.Hide();

		Header.SetIcon();

		Header.Show();
		Footer.Show();
		Worker.Show();
	}

	Footer.HideButtons();

	message.SetText(TEXT(""));

	switch (GetState())
	{
	case ST_TRANSDEP_SELECTION:    	Transdep.OnSelectionScreen(); break;
	case ST_TRANSDEP_ROUTER:		Transdep.OnRouter(); break;
	case ST_TRANSDEP_TIMETABLE:		Transdep.OnDispatcher(); break;
	case ST_TRANSDEP_SEATME:		Transdep.OnSeat(); break;
	case ST_TRANSDEP_TICKET:		Transdep.OnTicket(); break;
	case ST_TRANSDEP_PAYPREVIEW:	Transdep.OnPayPreview(); break;
	case ST_TRANSDEP_PAYPROCESSING:	Transdep.OnPayProcess(); break;
	case ST_TRANSDEP_PAYDATABANK:	Transdep.OnPayDatabank(); break;
	}

	Header.Redraw();

	return TRUE;
}

BOOL CClientWnd::GoBack()
{
	switch (GetState())
	{
	case ST_TRANSDEP_SELECTION:     return State(ST_ROOT);
	case ST_TRANSDEP_ROUTER:		return State(ST_TRANSDEP_SELECTION);
	case ST_TRANSDEP_TIMETABLE:		return State(ST_TRANSDEP_ROUTER);
	case ST_TRANSDEP_SEATME:		return State(ST_TRANSDEP_TIMETABLE);
	case ST_TRANSDEP_TICKET:		return State(ST_TRANSDEP_SEATME);
	case ST_TRANSDEP_PAYPREVIEW:	return State(ST_TRANSDEP_TICKET);
	case ST_TRANSDEP_PAYPROCESSING:	return State(ST_TRANSDEP_PAYPREVIEW);
	case ST_TRANSDEP_PAYDATABANK:	return State(ST_ROOT);
	}
	return FALSE;
}

BOOL CClientWnd::GoNext()
{
	switch (GetState())
	{
	case ST_ROOT:                   return State(ST_TRANSDEP_SELECTION);
	case ST_TRANSDEP_ROUTER:		return State(ST_TRANSDEP_TIMETABLE);
	case ST_TRANSDEP_SELECTION:     return State(ST_TRANSDEP_ROUTER);
	case ST_TRANSDEP_TIMETABLE:		return State(ST_TRANSDEP_SEATME);
	case ST_TRANSDEP_SEATME:		return State(ST_TRANSDEP_TICKET);
	case ST_TRANSDEP_TICKET:		return State(ST_TRANSDEP_PAYPREVIEW);
	case ST_TRANSDEP_PAYPREVIEW:	return State(ST_TRANSDEP_PAYPROCESSING);
	case ST_TRANSDEP_PAYPROCESSING:	return State(ST_ROOT);
	case ST_TRANSDEP_PAYDATABANK:
	{
		Active = ST_TRANSDEP_PAYPROCESSING;
		return State(ST_TRANSDEP_PAYDATABANK);
	}
	}
	return FALSE;
}

MACHINE_STATE CClientWnd::GetState()
{
	return Active;
}

MACHINE_STATE CClientWnd::GetLastState()
{
	return Last;
}

bool CClientWnd::IsStateChanged()
{
	return (GetState() != GetLastState());
}

bool CClientWnd::IsMain(MACHINE_STATE state)
{
	return ((state == ST_UNDEFINED) ? GetState() : state) == ST_ROOT;
}

bool CClientWnd::IsTransdep(MACHINE_STATE state)
{
	if (state == ST_UNDEFINED)
		state = GetState();

	return (state >= ST_TRANSDEP_SELECTION && state <= ST_TRANSDEP_PAYDATABANK);
}

void CClientWnd::JumpStart()
{
	switch (GetState())
	{
	case ST_TRANSDEP_TICKET:
	{
		for (UINT i = 0; i < TRANSDEP_MAX_RESERVATION; i++)
		{
			Transdep.ticket[i].Hide();
		}
	} break;
	}

	State(ST_ROOT);
}

void CClientWnd::HidePopups()
{
	if (Keyboard.IsVisible())
		Keyboard.Hide();

	if (message.IsVisible())
		message.Hide();

	if (loading.IsVisible())
		loading.Hide();

	if (Transdep.pay.swiping.IsVisible())
		Transdep.pay.swiping.Hide();
}

void CClientWnd::StopServiceThreads()
{
	if (Transdep.service.IsWorking())
		Transdep.service.Stop(true);

	if (Transdep.pay.Transaction.IsWorking())
		Transdep.pay.Transaction.Stop(true);
}

LPVOID CClientWnd::OnCreate(HWND hWnd, LPARAM lParam)
{
	CClientWnd *me = reinterpret_cast<CClientWnd*>(GetUserDataLongPtr(hWnd));
	if (me)
	{
		me->SetBkBrush(::CreateSolidBrush(RGB(233, 233, 233)));
		me->AssignBitmap(Bmp::LoadPicture(CLIENT_BK));

		me->SetForeground();

		cfg.SetClientRect(me->GetClientRect(), 0, 0);

		me->menu.Create(*me);
		
		Header.Initialize(me->GetInstance(), *me, cfg.Header());
		Footer.Initialize(me->GetInstance(), *me, cfg.Footer());

		if (!Worker.Register(CS_DBLCLKS | CS_OWNDC | CS_SAVEBITS, me->GetInstance(), NULL, NULL, (HBRUSH) ::GetStockObject(HOLLOW_BRUSH)))
			return 0L;

		if (!Worker.Create(TEXT(""), WS_CHILD, cfg.Worker(), *me, NULL, me->GetInstance()))
			return 0L;

		Worker.Show();

		Keyboard.Assign(me->GetInstance(), *me);

		me->message.Create(me->GetInstance(), *me, cfg.Worker());
		me->message.SetBkBrush(BRUSH_WHITE);

		me->loading.Create(me->GetInstance(), *me, cfg.Worker());
		me->loading.SetBkBrush(BRUSH_WHITE);
		if (me->loading.animator.Initialize(me->GetInstance(), *me, coord::to_rect(0, 0, 200, 200)))
		{
			me->loading.SetAnimation(GIF_GETTING_DATA);
			me->loading.animator.SetParent(me->loading);
		}

		me->JumpStart();
	}
	return 0L;
}

LPVOID CClientWnd::OnPaint(HWND OwnerWnd, HDC hDC, PAINTSTRUCT pStruct)
{
	CClientWnd *me = reinterpret_cast<CClientWnd*>(GetUserDataLongPtr(OwnerWnd));

	if (me)
	{
		me->PaintBitmap(hDC);

		if (!me->IsMain() &&
			(cfg.Client().left != cfg.Rect().left || cfg.Client().right != cfg.Rect().right
			|| cfg.Client().top != cfg.Rect().top || cfg.Client().bottom != cfg.Rect().bottom))
		{
			RECT rect = cfg.Client();
			HPEN hPenOld = static_cast<HPEN>(::SelectObject(hDC, ::GetStockObject(DC_PEN)));
			HBRUSH hBrushOld = static_cast<HBRUSH>(::SelectObject(hDC, ::GetStockObject(NULL_BRUSH)));
			::SelectObject(hDC, ::CreatePen(PS_SOLID, CLIENT_RC_WIDTH, RGB_PRIMARY));
			::InflateRect(&rect, CLIENT_RC_WIDTH, CLIENT_RC_WIDTH);
			::Rectangle(hDC, rect.left, rect.top, rect.right, rect.bottom);
			::SelectObject(hDC, hPenOld);
			::SelectObject(hDC, hBrushOld);
		}
	}
	return 0L;
}