#include "../clientwnd.h"
#include "mapcontrol.h"

using namespace transdep;

MapControl::MapControl()
	: CBitmapWindow()
{
	SetClass(MAP_WND_CLASS);

	onPaint = OnPaint;
}

MapControl::~MapControl()
{
}

LPVOID MapControl::OnPaint(HWND OwnerWnd, HDC hDC, PAINTSTRUCT pStruct)
{
	return CBitmapWindow::OnPaint(OwnerWnd, hDC, pStruct);
}

bool MapControl::Init(HINSTANCE hInst, HWND hParent)
{	
	if (!Initialize(hInst, hParent, TEXT(""), cfg.Worker(), WS_CHILD))
		return FALSE;

	SetBkBrush(BRUSH_WHITE);

	route0.Initialize(hInst, *this, TEXT("route0"), coord::to_rect(1115, 15, 150, 50), WS_CHILD);
	route0.AssignBitmap(Bmp::LoadGDI((cfg.DataPath() + TEXT("route0btn.jpg")).c_str()));
	route0.SetCursorHand();
	route0.Show();
	
	if (!route[0].Register(CS_DBLCLKS | CS_OWNDC | CS_SAVEBITS, GetInstanceWnd(*this), NULL, ::LoadCursor(NULL, IDC_HAND)))
		return FALSE;

	for (int i = 0; i < 10; i++)
	{
		route[i].Create(std::to_wstring(i).c_str(), WS_CHILD, coord::to_rect(1115, 80 + i * 50 + i * 15, 150, 50), *this, NULL, hInst);
		route[i].SetRegion(Rgn::Rounded(coord::size(150, 50)));
		route[i].Show();
	}

	route[0].SetBkBrush(::CreateSolidBrush(RGB(50, 125, 255)));

	//if (!up.Register(CS_DBLCLKS | CS_OWNDC | CS_SAVEBITS, GetInstanceWnd(*this), NULL, ::LoadCursor(NULL, IDC_HAND)))
//		return FALSE;

	up.Create(TEXT("up"), WS_CHILD, coord::to_rect(510, 10, 80, 45), *this, NULL, hInst);
	up.SetRegion(Rgn::Triangle(coord::point(0, 45), coord::point(40, 0), coord::point(80, 45)));
	up.Show();

	right.Create(TEXT("right"), WS_CHILD, coord::to_rect(1045, 372, 45, 80), *this, NULL, hInst);
	right.SetRegion(Rgn::Triangle(coord::point(0, 0), coord::point(45, 40), coord::point(0, 80)));
	right.Show();

	down.Create(TEXT("down"), WS_CHILD, coord::to_rect(510, 769, 80, 45), *this, NULL, hInst);
	down.SetRegion(Rgn::Triangle(coord::point(0, 0), coord::point(40, 45), coord::point(80, 0)));
	down.Show();

	left.Create(TEXT("left"), WS_CHILD, coord::to_rect(10, 372, 45, 80), *this, NULL, hInst);
	left.SetRegion(Rgn::Triangle(coord::point(0, 40), coord::point(45, 0), coord::point(45, 80)));
	left.Show();

	up.onMouseDown = OnMapMove;
	right.onMouseDown = OnMapMove;
	down.onMouseDown = OnMapMove;
	left.onMouseDown = OnMapMove;

	router.SetClass(TEXT("KioskMan::Router"));
	if (!router.Initialize(hInst, *this, TEXT("Router"), coord::to_rect(0, 0, 1100, 824), WS_CHILD | WS_CLIPSIBLINGS))
		return FALSE;

	router.AssignBitmap(Bmp::LoadGDI((cfg.DataPath() + TEXT("map\\route0.jpg")).c_str()));
	router.Show();

	return TRUE;
}

LPVOID MapControl::OnMapMove(HWND hWnd, UINT mButton, WPARAM sState, int X, int Y)
{
	CWnd *btn = reinterpret_cast<CWnd*>(GetUserDataLong(hWnd));

	if (btn)
	{
	}

	return 0L;
}