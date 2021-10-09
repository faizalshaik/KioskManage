#include <windows.h>
#include <tchar.h>

#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_IA64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='ia64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif

#include "resource.h"
#include "updater.h"

Config cfg;

int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	if (IsWindowExists(UPDATER_CLASS, UPDATER_TITLE, TRUE))
		return 0;

	if (!Updater.Register(::IsDebuggerPresent() ? CS_DBLCLKS | WS_EX_TOPMOST : CS_NOCLOSE | CS_DBLCLKS | WS_EX_TOPMOST, hInstance))
		return 0;

	if (!Updater.CreateEx(WS_EX_APPWINDOW, UPDATER_TITLE, WS_POPUP, RECT_UPDATER, NULL, NULL, hInstance))
		return 0;
	
	Updater.Show();
	Updater.GetClientVersion();

	//::SendMessage(Updater.Client, WM_CLOSE, NULL, NULL);

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