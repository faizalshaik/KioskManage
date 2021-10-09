/**
* ClientWnd.cpp
*
* @program		Global-Pay
* @author		АНКУ ХХК
* @copyright	Copyright (c) 2017 он, АНКУ ХХК.
* @version		0.1.0.0
* @date			2017.09.19
*/

#include "stdafx.h"

#include "resource.h"

#include "touchwnd.h"
#include "clientwnd.h"
#include "flashwnd.h"

CClientWnd Client;

CClientWnd::CClientWnd(void)
	: CBitmapWindow()
{
	SetClass(KIOSKMAN_CLIENT);

	Active = ST_UNDEFINED;

	onCreate = OnCreate;
	onPaint = OnPaint;
}


BOOL CClientWnd::ClearPrevState()
{
	footer.HideButtons();

	switch (GetLastState())
	{
	case ST_TRANSDEP_SELECTION:		
		Worker.Painter(NULL);
		break;
	case ST_TRANSDEP_ROUTER:		
		Transdep.router.setVisible(false);
		break;
	case ST_TRANSDEP_TIMETABLE:
		Transdep.times.Hide();
		break;
	case ST_TRANSDEP_SEATME:
		Transdep.Bus.Hide();
		break;
	case ST_TRANSDEP_TICKET:
		for (UINT i = 0; i < Transdep.Bus.Reserved(); i++)
		{
			Transdep.ticket[i].Hide();
		}
		Keyboard.Hide();
		break;
	case ST_TRANSDEP_PAYPREVIEW:
		Transdep.pay.Hide();
		break;
	case ST_TRANSDEP_PAYPROCESSING:
		Client.message.Hide();
		break;
	case ST_TRANSDEP_PAYDATABANK:
		Client.message.Hide();
		break;
	case ST_KIOSKLOCATION_SWF:
		mapper.Hide();
		break;
	case ST_DISCONNECTED:
		break;
	}
	return TRUE;
}

BOOL CClientWnd::State(MACHINE_STATE state)
{
	Last = GetState();
	Active = state;

	if (!IsStateChanged())
		return FALSE;

	ClearPrevState();	

	if (IsMain())
		Worker.Hide();

	if (Touch.service.IsWorking())
	{
		Touch.service.Stop(true);
	}

	switch (GetState())
	{
	case ST_TRANSDEP_SELECTION:    	
		Transdep.OnSelectionScreen(); 
		break;
	case ST_TRANSDEP_ROUTER:		
		Transdep.OnRouter(); 
		break;
	case ST_TRANSDEP_TIMETABLE:		
		Transdep.OnDispatcher(); 
		break;
	case ST_TRANSDEP_SEATME:		
		Transdep.OnSeat(); 
		break;
	case ST_TRANSDEP_TICKET:		
		Transdep.OnTicket(); 
		break;
	case ST_TRANSDEP_PAYPREVIEW:	
		Transdep.OnPayPreview(); 
		break;
	case ST_TRANSDEP_PAYPROCESSING:	
		Transdep.OnPayProcess(); 
		break;
	case ST_TRANSDEP_PAYDATABANK:	
		Transdep.OnPayDatabank(); 
		break;
	case ST_KIOSKLOCATION_SWF:      
		OnMapper(); 
		break;
	case ST_DISCONNECTED:           
		return Touch.OnDisconnect();
	}

	if (GetLastState() == ST_ROOT)
		Worker.Show();

	return TRUE;
}

BOOL CClientWnd::GoBack()
{
	switch (GetState())
	{
	case ST_TRANSDEP_SELECTION:     return State(ST_ROOT);
//	case ST_TRANSDEP_ROUTER:		return State(ST_TRANSDEP_SELECTION);
	case ST_TRANSDEP_ROUTER:		return State(ST_ROOT);
	case ST_TRANSDEP_TIMETABLE:		return State(ST_TRANSDEP_ROUTER);
	case ST_TRANSDEP_SEATME:		return State(ST_TRANSDEP_TIMETABLE);
	case ST_TRANSDEP_TICKET:		return State(ST_TRANSDEP_SEATME);
	case ST_TRANSDEP_PAYPREVIEW:	return State(ST_TRANSDEP_TICKET);
	case ST_TRANSDEP_PAYPROCESSING:	return State(ST_TRANSDEP_PAYPREVIEW);
	case ST_TRANSDEP_PAYDATABANK:	return State(ST_ROOT);
	case ST_KIOSKLOCATION_SWF:	    return State(ST_ROOT);
	}
	return FALSE;
}

BOOL CClientWnd::GoNext()
{
	switch (GetState())
	{
		case ST_ROOT:          
			return State(ST_TRANSDEP_SELECTION);
		case ST_TRANSDEP_ROUTER:	
			return State(ST_TRANSDEP_TIMETABLE);
		case ST_TRANSDEP_SELECTION:    
			return State(ST_TRANSDEP_ROUTER);
		case ST_TRANSDEP_TIMETABLE:
			return State(ST_TRANSDEP_SEATME);
		case ST_TRANSDEP_SEATME:		
			return State(ST_TRANSDEP_TICKET);
		case ST_TRANSDEP_TICKET:		
			return State(ST_TRANSDEP_PAYPREVIEW);
		case ST_TRANSDEP_PAYPREVIEW:	
			return State(ST_TRANSDEP_PAYPROCESSING);
		case ST_TRANSDEP_PAYPROCESSING:	
			return State(ST_ROOT);
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
		for (UINT i = 0; i < TRANSDEP_MAX_RESERV; i++)
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
	CClientWnd *me = reinterpret_cast<CClientWnd*>(GetUserDataLong(hWnd));
	if (me)
	{
		cfg.SetClientRect(me->GetClientRect());

		me->AssignBitmap(Bmp::LoadPicture(PATH_CLIENT_BK));
		me->SetBkBrush(BRUSH_CLIENT_BK);
		me->SetForeground();	

		me->menu.Create(*me);
		
		Client.header.Initialize(me->GetInstance(), *me, cfg.Header());
		Client.footer.Initialize(me->GetInstance(), *me, cfg.Footer());

		if (Worker.Register(CS_DBLCLKS | CS_OWNDC | CS_SAVEBITS, me->GetInstance(), NULL, NULL, BRUSH_TRANSPARENT)
		{
			if (Worker.Create(TEXT_EMPTY, WS_CHILD, cfg.Worker(), *me, NULL, me->GetInstance()))
			{
				if (me->mapper.Init(Worker.GetClientRect(), Worker, me->GetInstance()))
				{
					me->mapper.ActivateControl();
					me->mapper.SetMovie(SWF_MAPPER);
				}
			}
		}

		Keyboard.Assign(me->GetInstance(), *me);
	}
	return 0L;
}

LPVOID CClientWnd::OnPaint(HWND hWnd, HDC hDC, PAINTSTRUCT pStruct)
{
	return CBitmapWindow::OnPaint(hWnd, hDC, pStruct);
}

void CClientWnd::OnMapper()
{
	Worker.Color(BRUSH_KIOSKLOCATION);
	Worker.Icon(BITMAP_MAPPER);
	Worker.Timer(INTERVAL_KIOSKLOCATION);
	Worker.Status(TEXT_MAPPER_MN);
	Worker.Left(TEXT_HOME_MN, BRUSH_KIOSKLOCATION_BTN);

	mapper.Show();
	mapper.CallFunction((TCHAR*)mapper.GetExternalXML(FLASH_EXTERNAL_RESTART, cfg.IsMongolian() ? FLASH_EXTERNAL_TRUE : FLASH_EXTERNAL_FALSE).c_str());
}