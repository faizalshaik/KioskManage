// original name : TouchWnd.cpp
// more info @   : TouchWnd.h

#include "stdafx.h"

#include "resource.h"

#include "touchwnd.h"
#include "clientwnd.h"
#include "flashwnd.h"
#include "lowerthirdwnd.h"

using namespace globalpay;

CTouchWnd Touch;

CTouchWnd::CTouchWnd(void)
	: CWnd()
{
	SetClass(MAIN_CLASS);

	onCreate = OnCreate;
	onClose = OnClose;
	onTimer = OnTimer;
}

LPVOID CTouchWnd::OnCreate(HWND hWnd, LPARAM lParam)
{
	CTouchWnd *TouchWnd = reinterpret_cast<CTouchWnd*>(GetUserDataLong(hWnd));
	if (TouchWnd)
	{
		TouchWnd->SetForeground();

		RECT rect = TouchWnd->GetClientRect();
		RECT rcFlash = rect, rcClient = rect, rcLowerThird = rect;

		if ((HEIGHT_BANNER + HEIGHT_CLIENT) > coord::height(rect))
		{
			rcFlash.bottom = rect.top + (coord::height(rect) / 100) * PERCENTAGE_BANNER;
			rcClient.top = rcFlash.bottom;
			rcClient.bottom = rcFlash.bottom + (coord::height(rect) / 100) * PERCENTAGE_CLIENT;
		}
		else
		{
			rcFlash.bottom = rect.top + HEIGHT_BANNER;
			rcClient.top = rcFlash.bottom;
			rcClient.bottom = rcFlash.bottom + HEIGHT_CLIENT;
		}
		rcLowerThird.top = rcClient.bottom;

		if (Replayer.Init(rcFlash, *TouchWnd, TouchWnd->GetInstance()))
		{
			Replayer.ActivateControl();
			Replayer.SetMovie(SWF_REPLAYER);
			Replayer.Show();
		}

		if (Tutorial.Init(rcFlash, *TouchWnd, TouchWnd->GetInstance()))
		{
			Tutorial.ActivateControl();
			Tutorial.SetMovie(SWF_TUTORIAL);
		}

		if (LowerThird.Init(rcLowerThird, *TouchWnd, TouchWnd->GetInstance()))
		{
			LowerThird.ActivateControl();
			LowerThird.SetMovie(SWF_MARQUEE);
			LowerThird.MarqueeSetText(TEXT_FLASH_MARQUEE_MN);
			LowerThird.Show();
		}

		if (TouchWnd->loader.Register(CS_DBLCLKS | CS_OWNDC | CS_SAVEBITS, TouchWnd->GetInstance()))
		{
			
			if (TouchWnd->loader.CreateEx(WS_EX_STATICEDGE, TEXT_EMPTY, WS_CHILD, coord::to_rect(0, 0, 10, 10), *TouchWnd, NULL, TouchWnd->GetInstance()))
			{
				TouchWnd->loader.SetBkBrush(BRUSH_WHITE);
				if (TouchWnd->loader.gif.Initialize(TouchWnd->GetInstance(), TouchWnd->loader, coord::to_rect(0, 0, 10, 10)))
				{
					TouchWnd->loader.gif.LoadFile(PATH_LOADING_HOURGLASS);
					TouchWnd->loader.gif.MoveTo(0, 50);
					TouchWnd->loader.gif.Show();
					TouchWnd->loader.SetBkBrush(BRUSH_LOADING);
					TouchWnd->loader.SetText(TEXT_CONNECTINGSERVER_MN);
				}
			}
		}

		if (Client.Register(CS_DBLCLKS | CS_PARENTDC | CS_SAVEBITS, TouchWnd->GetInstance()))
		{
			if (Client.Create(TEXT_EMPTY, WS_CHILD, rcClient, *TouchWnd, NULL, TouchWnd->GetInstance()))
			{
				Client.Show();
			}
		}

		RECT rcLoading = Client.GetClientRect();
		TouchWnd->loader.SetParent(Client);
		TouchWnd->loader.SetRect(coord::to_rect(rcLoading.left + coord::width(rcLoading) / 2 - 250, rcLoading.top + coord::height(rcLoading) / 2 - 275, 500, 550));
		Client.State(ST_DISCONNECTED);
		
	}
	return 0L;
}

LPVOID CTouchWnd::OnClose(HWND hWnd)
{
	::PostQuitMessage(0);
	return 0L;
}

LPVOID CTouchWnd::OnTimer(HWND OwnerWnd, UINT uTimerId)
{
	if (uTimerId == TIMER_REPORTER)
	{
		if (CInternet::CheckConnection())
		{
			std::wstring Response = Server::Post(OBJ_REPORT_ME, ("ID=" + cfg.GetID() + std::string("&version=") + cfg.version.AsStringA()));
			if (Response == TEXT_OK_EN)
			{
				//::MessageBox(NULL, L"I alive", L"", MB_OK);
			}
			else if (Response == TEXT_UPDATE_EN)
			{
				//::MessageBox(NULL, L"Need update", L"", MB_OK);
			}
			else
			{
				//::MessageBox(NULL, L"Can't interact server.", L"", MB_OK);
			}
		}
	}
	
	return 0L;
}

void CTouchWnd::IdentifyMe(LPVOID)
{
	while (true)
	{
		UINT ID = Touch.GetID();
		if (ID)
		{
			cfg.SetID(ID);
			Touch.loader.Hide();
			
			Touch.SetTimer(TIMER_REPORTER, INTERVAL_1SECOND * 2);
			
			Client.JumpStart();

		}
		else
		{
			::Sleep(INTERVAL_1SECOND * (::IsDebuggerPresent() ? 3 : 15));
		}
	}
}

UINT CTouchWnd::GetID()
{
	if (CInternet::CheckConnection())
	{

		std::wstring ID = Server::Post(OBJ_REQUIRE_ID, ("name=" + ComputerName()));

		try
		{
			return std::stoi(ID);
		}
		catch (...)
		{
			::MessageBox(NULL, TEXT_FAILED_EN, TEXT_ERROR_EN, MB_OK | MB_ICONSTOP);
			::PostQuitMessage(0);
		}
	}
	return 0;
}

BOOL CTouchWnd::OnDisconnect()
{
	Worker.Hide();
	loader.Show();

	service.SetWorker(CTouchWnd::IdentifyMe);
	service.Start();

	return TRUE;
}

CLoader::CLoader()
	: CWnd()
{
	SetClass(KIOSKMAN_LOADER);
	onPaint = OnPaint;
	onSetText = OnSetText;
}

LPVOID CLoader::OnPaint(HWND OwnerWnd, HDC hDC, PAINTSTRUCT pStruct)
{
	CLoader *me = reinterpret_cast<CLoader*>(GetUserDataLong(OwnerWnd));
	if (me)
	{
		::SelectObject(hDC, cfg.Font(22));
		std::wstring message = me->AsString();
		SIZE size = me->GetTextExtendPoint(message.c_str(), hDC);
		RECT rect = coord::to_rect(coord::width(me->GetClientRect()) / 2 - size.cx / 2, 25 - size.cy / 2 , size.cx, size.cy);
		::SetBkMode(hDC, TRANSPARENT);
		::DrawText(hDC, message.c_str(), message.length(), &rect, NULL);
	}
	return 0L;
}

LPVOID CLoader::OnSetText(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	CLoader *me = reinterpret_cast<CLoader*>(GetUserDataLong(hWnd));
	if (me)
	{
		me->Redraw();
	}
	return (LPVOID)::DefWindowProc(hWnd, WM_SETTEXT, wParam, lParam);
}