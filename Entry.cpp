#include "stdafx.h"

#include "resource.h"

#include "clientwnd.h"
#include "splashwnd.h"
#include "touchwnd.h"

Config cfg;

int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	if (IsWindowExists(KIOSKMAN_SPLASH, MAIN_TITLE, TRUE))
		return 0;

	if (IsWindowExists(MAIN_CLASS, MAIN_TITLE, TRUE))
		return 0;
	
	CSplashWnd Splash;
	Splash.Init(hInstance);
	Splash.Display(INTERVAL_1SECOND);

	MSG	msg;
	BOOL bRet;
	while (bRet = ::GetMessage(&msg, NULL, 0, 0) != 0)
	{
		if (bRet == -1)
		{
		}
		else
		{
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
		}
	}
	return (int)msg.wParam;
}