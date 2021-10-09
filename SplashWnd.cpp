#include "resource.h"

#include "splashwnd.h"

#include "touchwnd.h"
#include "transdep/control.h"

CSplashWnd::CSplashWnd(void)
	: CBitmapWindow()
{
	SetClass(KIOSKMAN_SPLASH);

	onPaint = OnPaint;
	onTimer = OnTimer;
	onDestroy = OnDestroy;
}

void CSplashWnd::Init(HINSTANCE hInst)
{
	HBITMAP splash = Bmp::LoadGDI(PATH_SPLASH);
	Initialize(hInst, NULL, MAIN_TITLE, RECT_SPLASH, WS_POPUP);
	AssignBitmap(splash, true);
	SetRegion(Rgn::Bitmap(Bmp::GetMonochrome(splash)));
	SetIcon(LoadIconResource(IDI_GEREGE));
	Center(true);

	if (Touch.Register(0, hInst, LoadIconResource(IDI_GEREGE)))
		if (Touch.CreateEx(WS_EX_APPWINDOW, MAIN_TITLE, WS_POPUP | WS_CLIPCHILDREN | WS_CLIPSIBLINGS, cfg.Primary(), NULL, NULL, hInst))
			return;

	::PostQuitMessage(0);
}

void CSplashWnd::Display(UINT showTime)
{
	SetTimer(TIMER_SPLASH, showTime);
	Show();
}

LPVOID CSplashWnd::OnTimer(HWND OwnerWnd, UINT uTimerId)
{
	if (uTimerId == TIMER_SPLASH)
		::DestroyWindow(OwnerWnd);

	return 0L;
}

LPVOID CSplashWnd::OnDestroy(HWND OwnerWnd)
{
	::KillTimer(OwnerWnd, TIMER_SPLASH);

	Touch.Show();

	return 0L;
}

LPVOID CSplashWnd::OnPaint(HWND OwnerWnd, HDC hDC, PAINTSTRUCT pStruct)
{
	LPVOID ret = CBitmapWindow::OnPaint(OwnerWnd, hDC, pStruct);

	::SetBkMode(hDC, TRANSPARENT);
	::SetTextColor(hDC, RGB_WHITE);
	::SelectObject(hDC, cfg.Font(17));
	::TextOut(hDC, 410, 455, cfg.version.AsString().c_str(), cfg.version.AsString().length());

	if (Transdep.LoadRaws())
	{
		Transdep.SetMaster();
		Transdep.CookIt();
		Transdep.GenerateRoutes();
	}

	return ret;
}