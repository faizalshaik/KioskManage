#include "window/bitmapwnd.h"
#include "internet/internet.h"

#include "../version.hpp"
#include "resource.h"

#define UPDATER_TITLE  TEXT("Updater for KioskMan")
#define UPDATER_CLASS  TEXT("KioskMan::Updater")

#define RECT_UPDATER   coord::to_rect(MAIN_LEFT, MAIN_TOP, MAIN_WIDTH, MAIN_HEIGHT)

#define BITMAP_UPDATER Bmp::LoadGDI()

using namespace globalpay;

typedef enum
{
	usNone,
	usChecking,
	usKilling,
	usDownloading,
	usVerifying,
	usUnpacking,
	usCopying,
	usFinalizing,
	usIdle
} UpdateStatus;

class CUpdater : public CBitmapWindow
{
public:
	CUpdater();

	HWND         Client;
	std::wstring Module;
	CVersion     Version;

	UpdateStatus Status;

	HWND SetClient(LPCTSTR, LPCTSTR);
	void GetClientVersion();
	void Initiate();

protected:
	static LPVOID CALLBACK OnCreate(HWND hWnd, LPARAM lParam);
	static LPVOID CALLBACK OnClose(HWND hWnd);
};

extern CUpdater Updater;