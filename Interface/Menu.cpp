#include "barcode/barcode.h"
#include "system/tools.h"

#include "../touchwnd.h"
#include "../clientwnd.h"
#include "../flashwnd.h"

#include "menu.h"

CItem::CItem()
{
	SetClass(KIOSKMAN_MENUITEM);
}

CItem *CItem::Init(LPCTSTR szText, void(*OnClickEvent)(HWND), HBITMAP hBmp, HINSTANCE hInst, HWND hParent)
{
	Instant(hInst == NULL ? Client.GetInstance() : hInst, hParent == NULL ? Client : hParent,
		coord::to_rect(0, 0, MENU_ITEM_WIDTH, MENU_ITEM_HEIGHT), hBmp, OnClickEvent, false, szText);

	return this;
}

CMainMenu::CMainMenu()
{
	items = NULL;
	count = 0;
}

CMainMenu::~CMainMenu()
{
	DeleteItems();
}

void CMainMenu::Create(HWND hParent)
{
	HBITMAP bitmap = BITMAP_LOGO;
	SIZE size = Bmp::GetSize(bitmap);
	logo.Instant(Client.GetInstance(), hParent, coord::to_rect(0, 0, size.cx, size.cy), bitmap, OnLogoClick, false, TEXT_LOGO_TITLE);
	logo.SetRegion(Rgn::Bitmap(Bmp::GetMonochrome(bitmap)));

	HBITMAP hbmpFlag = BITMAP_FLAG;
	size = Bmp::GetSize(hbmpFlag);
	flag.Instant(Client.GetInstance(), hParent, coord::to_rect(0, 0, size.cx, size.cy), hbmpFlag, OnFlagClick, false, TEXT_FLAG_TITLE);

	AllocItems(5);

	items[0].Init(TEXT_MENU_TRANSDEP, OnTransdepClick);
	items[1].Init(TEXT_MENU_RAILWAY, OnRailwayClick)->Disable();
	items[2].Init(TEXT_MENU_FLIGHT, OnFlightClick)->Disable();
	items[3].Init(TEXT_MENU_TORGUULI, OnTorguuliClick)->Disable();
	items[4].Init(TEXT_MENU_UMONEY, OnUMoneyClick)->Disable();

	SetLanguage();
	Position();

	bar.Initialize(hParent, RECT_BARMENU);
}

void CMainMenu::Show(bool bLogoOnly)
{
	logo.Show();
	flag.Show();
	
	if (bLogoOnly)
		return;

	for (UINT i = 0; i < GetItems(); i++)
	{
		items[i].Show();
	}

	bar.Show();
}

void CMainMenu::Hide()
{
	logo.Hide();
	flag.Hide();

	for (UINT i = 0; i < GetItems(); i++)
	{
		items[i].Hide();
	}

	bar.Hide();
}

void CMainMenu::AllocItems(UINT nCount)
{
	DeleteItems();

	count = nCount;
	items = new CItem[nCount];
}

void CMainMenu::SetItemBitmap(LPCTSTR szText, HBITMAP hBmp)
{
	CItem *item = GetItemBy(szText);
	if (item)
		item->AssignBitmap(hBmp);
}

void CMainMenu::DeleteItems()
{
	SafeDelete(items);
	count = 0;
}

CItem* CMainMenu::GetItemBy(LPCTSTR szText)
{
	if (GetItems())
	{
		for (UINT i = 0; i < GetItems(); i++)
			if (items[i].AsString() == szText)
				return &items[i];
	}
	return NULL;
}

UINT CMainMenu::GetItems()
{
	return count;
}

void CMainMenu::Position()
{
	HWND parent = logo.GetParent();
	RECT rect = logo.GetParentClientRect();
	POINT start;
	start.y = MENU_LOGO_TOP;
	start.x = (coord::width(rect) - MENU_ITEM_WIDTH * MENU_ITEMS_IN_ROW - MENU_ITEM_BETWEEN * (MENU_ITEMS_IN_ROW)) / 2;

	logo.MoveTo(start.x, start.y, false);
	flag.MoveTo(coord::width(rect) - start.x - coord::width(flag.GetRect()), start.y + coord::height(logo.GetRect()) / 2 - coord::height(flag.GetRect()) / 2, false);

	start.y += coord::height(logo.GetRect()) + MENU_LOGO_TOP;

	if (GetItems())
	{
		UINT row = 1, col = 1;
		HWND item = NULL;
		while (item = ::FindWindowEx(parent, item, KIOSKMAN_MENUITEM, NULL))
		{
			::MoveWindow(item, start.x + (MENU_ITEM_BETWEEN + MENU_ITEM_WIDTH) * (col - 1), start.y + (MENU_ITEM_HEIGHT + MENU_ITEM_BETWEEN) * (row - 1), MENU_ITEM_WIDTH, MENU_ITEM_HEIGHT, FALSE);

			if (col < MENU_ITEMS_IN_ROW)
				col++;
			else
			{
				col = 1;
				row++;
			}
		}
	}
}

void CMainMenu::SetLanguage()
{
	flag.AssignBitmap(BITMAP_FLAG);

	if (GetItems())
	{
		for (UINT i = 0; i < GetItems(); i++)
		{
			items[i].AssignBitmap(BITMAP_MENUITEM(items[i].AsString() + (items[i].IsEnabled() ? TEXT("") : TEXT("-disable"))));
		}
	}

	Touch.loader.SetText(cfg.IsMongolian() ? TEXT_CONNECTINGSERVER_MN : TEXT_CONNECTINGSERVER_EN);
}

void CMainMenu::OnLogoClick(HWND)
{
}

void CMainMenu::OnFlagClick(HWND hWnd)
{
	POINT point;
	RECT rect;
	DWORD clicked;
	::GetCursorPos(&point);
	::ScreenToClient(hWnd, &point);
	::GetWindowRect(hWnd, &rect);
	clicked = (point.x < coord::width(rect) / 2) ? LANG_MONGOLIAN : LANG_ENGLISH;

	if (cfg.Language() != clicked)
	{
		cfg.SetAs(cfg.IsMongolian() ? LANG_ENGLISH : LANG_MONGOLIAN);
		Client.menu.SetLanguage();
	}
}

void CMainMenu::OnTransdepClick(HWND)
{
	//Client.State(ST_TRANSDEP_SELECTION);
	Client.State(ST_TRANSDEP_ROUTER);	
}

void CMainMenu::OnRailwayClick(HWND)
{
}

void CMainMenu::OnFlightClick(HWND)
{
}

void CMainMenu::OnTorguuliClick(HWND)
{
}

void CMainMenu::OnUMoneyClick(HWND)
{
}

CBarMenu::CBarMenu()
	: CBitmapWindow()
{
	SetClass(KIOSKMAN_BARMENU);	
	onPaint = OnPaint;
}

bool CBarMenu::Initialize(HWND hParent, RECT rect)
{
	if (!Register(CS_DBLCLKS | CS_OWNDC | CS_SAVEBITS, Client.GetInstance()))
		return false;

	if (!Create(TEXT_EMPTY, WS_CHILD, rect, hParent, NULL, Client.GetInstance(), this))
		return false;

	SetBkBrush(BRUSH_WHITE);

	rect = GetClientRect();
	
	HRGN region = ::CreateRectRgn(rect.left, rect.top + 16, rect.right, rect.bottom - 16);

	if (AppVideo.Initialize(Client.GetInstance(), *this, TEXT_APPVIDEO, RECT_APPVIDEO, WS_CHILD))
	{
		HBITMAP app_video_bmp = BITMAP_APPVIDEO;
		HRGN app_video_region = Rgn::Bitmap(Bmp::GetMonochrome(app_video_bmp));
		::OffsetRgn(app_video_region, rect.left + 474, rect.top);
		::CombineRgn(region, region, app_video_region, RGN_OR);
		::OffsetRgn(app_video_region, -(rect.left + 474), -(rect.top));
		AppVideo.SetRegion(app_video_region);
		AppVideo.AssignBitmap(app_video_bmp);
		AppVideo.Show();
		AppVideo.Disable();

		SafeDeleteObject(app_video_region);
	}

	if (KioskLocation.Initialize(Client.GetInstance(), *this, TEXT_KIOSKLOCATION, RECT_KIOSKLOCATION, WS_CHILD))
	{
		HBITMAP kiosk_location_bmp = BITMAP_KIOSKLOCATION;
		HRGN kiosk_location_region = Rgn::Bitmap(Bmp::GetMonochrome(kiosk_location_bmp));
		::OffsetRgn(kiosk_location_region, rect.left + 725, rect.top + 11);
		::CombineRgn(region, region, kiosk_location_region, RGN_OR);
		::OffsetRgn(kiosk_location_region, -(rect.left + 725), -(rect.top + 11));
		KioskLocation.SetRegion(kiosk_location_region);
		KioskLocation.AssignBitmap(kiosk_location_bmp);
		KioskLocation.Show();
		KioskLocation.SetHoverClick(OnKioskLocation);

		SafeDeleteObject(kiosk_location_region);
	}
	
	SetRegion(region);
	SafeDeleteObject(region);

	AssignBitmap(BITMAP_APPQRCODE);

	return true;
}

LPVOID CBarMenu::OnPaint(HWND OwnerWnd, HDC hDC, PAINTSTRUCT pStruct)
{
	CBarMenu *menu = reinterpret_cast<CBarMenu*>(GetUserDataLong(OwnerWnd));

	if (menu)
	{
		menu->PaintBitmap(hDC);
	}

	return 0L;
}

void CBarMenu::OnKioskLocation(HWND hWnd)
{
	Client.State(ST_KIOSKLOCATION_SWF);
}

bool CBarMenu::PaintBitmap(HDC hDC)
{
	if (GetBitmap())
	{
		HDC dcImage;
		dcImage = ::CreateCompatibleDC(hDC);
		if (!dcImage)
			return false;

		RECT rc = GetRect();
		SIZE bmp = GetBitmapDimension();
		::SelectObject(dcImage, GetBitmap());
		::BitBlt(
			hDC, 47, 47,
			(coord::width(rc) < bmp.cx) ? coord::width(rc) : bmp.cx,
			(coord::height(rc) < bmp.cy) ? coord::height(rc) : bmp.cy,
			dcImage, 0, 0, SRCCOPY);
		::DeleteDC(dcImage);

		return true;
	}
	return false;
}