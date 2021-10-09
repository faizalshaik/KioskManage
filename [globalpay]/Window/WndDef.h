/**
* Window Default - WndDef.h
*
* @program		Global-Pay
* @author		АНКУ ХХК
* @copyright	Copyright (c) 2017 он, АНКУ ХХК.
* @version		0.1.0.0
* @date			2017.09.19
*/

#pragma once

#include "wndbasic.h"
#include "msg.h"

namespace globalpay {
#define CSAUR_CHILD_ID_START 40010

	class CWndDef : public CWndBasic, public CMsg
	{
	public:
		CWndDef(void);
		~CWndDef(void);

		static UINT CHILD_ID;

		void SetClickEvent(void(*OnClickEvent)(HWND));
		void(*onClick)(HWND);
	};
}