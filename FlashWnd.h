/**
* TicketMachine -> Flash Window - Initial version
*
* @program		Global-Pay
* @author		АНКУ ХХК
* @copyright	Copyright (c) 2017 он, АНКУ ХХК.
* @version		0.1.0.0
* @date			2017.09.19
*/

#pragma once

#include "control/shockwaveflash.h"

using namespace globalpay;

class CFlashReplayer : public CFlashControl
{
public:
	CFlashReplayer();

	void Pause();
	void Play();
	void Stop();
	void Resume();

protected:
	static LPVOID CALLBACK OnHide(HWND, UINT);
	static LPVOID CALLBACK OnShow(HWND, UINT);
};

class CFlashTutorial : public CFlashControl
{
public:
	CFlashTutorial();

	void Play(std::wstring, LPCTSTR szLanguage = NULL);
	void Stop();

protected:
	static LPVOID CALLBACK OnHide(HWND, UINT);
};

extern CFlashReplayer Replayer;
extern CFlashTutorial Tutorial;