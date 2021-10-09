// original name : FlashWnd.cpp
// more info @   : FlashWnd.h

#include "stdafx.h"

#include "resource.h"

#include "flashwnd.h"
#include "clientwnd.h"

CFlashReplayer Replayer;
CFlashTutorial Tutorial;

CFlashReplayer::CFlashReplayer()
	: CFlashControl()
{
	onHide = OnHide;
	onShow = OnShow;
}

void CFlashReplayer::Pause()
{
	CallFunction((TCHAR*)GetExternalXML(FLASH_EXTERNAL_PAUSE).c_str());
}

void CFlashReplayer::Play()
{
	CallFunction((TCHAR*)GetExternalXML(FLASH_EXTERNAL_PLAY).c_str());
}

void CFlashReplayer::Resume()
{
	CallFunction((TCHAR*)GetExternalXML(FLASH_EXTERNAL_RESUME).c_str());
}

void CFlashReplayer::Stop()
{
	CallFunction((TCHAR*)GetExternalXML(FLASH_EXTERNAL_STOP).c_str());
}

LPVOID CFlashReplayer::OnHide(HWND hWnd, UINT)
{
	CFlashReplayer *flash = reinterpret_cast<CFlashReplayer*>(GetUserDataLong(hWnd));
	if (flash)
		flash->Pause();
	return 0L;
}

LPVOID CFlashReplayer::OnShow(HWND hWnd, UINT)
{
	if (Tutorial.IsVisible())
		Tutorial.Hide();

	CFlashReplayer *flash = reinterpret_cast<CFlashReplayer*>(GetUserDataLong(hWnd));
	if (flash)
		flash->Play();
	return 0L;
}

CFlashTutorial::CFlashTutorial()
	: CFlashControl()
{
	onHide = OnHide;
}

void CFlashTutorial::Play(std::wstring szMoviePath, LPCTSTR szLanguage)
{
	if (Replayer.IsVisible())
		Replayer.Hide();

	if (!IsVisible())
		Show();

	CallFunction((TCHAR*)GetExternalXML(FLASH_EXTERNAL_PLAY, GetArgumentXML(szMoviePath) + GetArgumentXML(szLanguage != NULL ? szLanguage : cfg.LanguageStr())).c_str());
}

void CFlashTutorial::Stop()
{
	CallFunction((TCHAR*)GetExternalXML(FLASH_EXTERNAL_STOP).c_str());
}

LPVOID CFlashTutorial::OnHide(HWND hWnd, UINT)
{
	CFlashTutorial *flash = reinterpret_cast<CFlashTutorial*>(GetUserDataLong(hWnd));
	if (flash)
		flash->Stop();
	return 0L;
}