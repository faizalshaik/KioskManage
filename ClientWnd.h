/**
* TicketMachine -> Client Window - Initial version
*
* @program		Global-Pay
* @author		АНКУ ХХК
* @copyright	Copyright (c) 2017 он, АНКУ ХХК.
* @version		0.1.0.0
* @date			2017.09.19
*/

#pragma once

#include "window/bitmapwnd.h"
#include "control/gif.h"
#include "control/shockwaveflash.h"

#include "interface/header.h"
#include "interface/footer.h"
#include "interface/worker.h"
#include "interface/menu.h"
#include "interface/keyboard.h"
#include "interface/message.h"

#include "transdep/control.h"

using namespace globalpay;

typedef enum
{
	ST_UNDEFINED,
	ST_DISCONNECTED,
	ST_ROOT,

	ST_TRANSDEP_SELECTION,
	ST_TRANSDEP_ROUTER,
	ST_TRANSDEP_TIMETABLE,
	ST_TRANSDEP_SEATME,
	ST_TRANSDEP_TICKET,
	ST_TRANSDEP_PAYPREVIEW,
	ST_TRANSDEP_PAYPROCESSING,
	ST_TRANSDEP_PAYDATABANK,

	ST_KIOSKLOCATION_SWF
} MACHINE_STATE;

class CClientWnd : public CBitmapWindow
{
public:
	CMainMenu     menu;
	CHeader       header;
	CFooter       footer;

	CFlashControl mapper;

	ClientMessage message;
	ClientAnimate loading;

public:
	CClientWnd(void);

	BOOL ClearPrevState();
	BOOL State(MACHINE_STATE);
	void JumpStart();
	BOOL GoBack();
	BOOL GoNext();

	bool IsStateChanged();
	MACHINE_STATE GetState();
	MACHINE_STATE GetLastState();

	bool IsMain(MACHINE_STATE state = ST_UNDEFINED);
	bool IsTransdep(MACHINE_STATE state = ST_UNDEFINED);

	void OnMapper();

	void HidePopups();
	void StopServiceThreads();

protected:
	MACHINE_STATE Active;
	MACHINE_STATE Last;

	static LPVOID CALLBACK OnCreate(HWND hWnd, LPARAM lParam);
	static LPVOID CALLBACK OnPaint(HWND hWnd, HDC hDC, PAINTSTRUCT pStruct);
};

extern CClientWnd Client;