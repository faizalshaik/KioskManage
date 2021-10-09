#include "../clientwnd.h"
#include "router.h"

using namespace transdep;

Province::Province()
	: CWnd()
{
	SetClass(TEXT("TVM::Router::Menu::Button"));

	isCity = false;
	sState = Enabled;
	onPaint = OnPaint;
	onMouseDown = OnMouseDown;
}

Destination::Destination()
	: CWnd()
{
	SetClass(TEXT("TVM::Router::Destination::Button"));

	onPaint = OnPaint;
	onMouseDown = OnMouseDown;
}

Router::Router()
	: CBitmapWindow()
{
	SetClass(TEXT("KioskMan::Transdep::Router"));
}

TransparentStatic::TransparentStatic()
	: CStatic()
{
	onEraseBackground = OnEraseBackground;
}

LPVOID TransparentStatic::OnEraseBackground(HWND hWnd, HDC hDC)
{
	return (LPVOID)(HBRUSH)::GetStockObject(BLACK_BRUSH);
}

Menu::Menu()
	: CPanel()
{
	SetClass(TEXT("KioskMan::Router::Menu"));
	province = NULL;
	onShow = OnMenuShow;
	onCtlColorStatic = OnCtlColorStatic;
}

Menu::~Menu()
{
	SafeDelete(province);
}

LPVOID Menu::OnCtlColorStatic(HWND hWnd, HDC hStaticDC, HWND hStaticCtl)
{
	Menu *me = reinterpret_cast<Menu*>(GetUserDataLong(hWnd));
	if (me)
	{
		//if (me->aimag.GetID() == GetWindowID(hStaticCtl) 
			//|| me->khot.GetID() == GetWindowID(hStaticCtl) 
			//|| me->gadaad.GetID() == GetWindowID(hStaticCtl))
		{
			::SetTextColor(hStaticDC, RGB_WHITE);
			::SetBkMode(hStaticDC, TRANSPARENT);
			::SetBkColor(hStaticDC, RGB(31, 42, 74));

			return (LPVOID)::CreateSolidBrush(RGB(31, 42, 74));//(LPVOID)(HBRUSH)::GetStockObject(HOLLOW_BRUSH);
		}
	}
	return 0L;
}

Work::Work()
{
	SetClass(TEXT("KioskMan::Router::Selection"));	
	stops = NULL;
}

Work::~Work()
{
	SafeDelete(stops);
}

LPVOID Province::OnPaint(HWND hWnd, HDC hDC, PAINTSTRUCT pStruct)
{
	Province *province = reinterpret_cast<Province*>(GetUserDataLong(hWnd));

	ULONG_PTR gdiplusToken;
	GdiplusStartupInput gdiplusStartupInput;
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

	Gdiplus::Graphics graphics(hDC);

	//Drawing Button
	SolidBrush whiteBrush(Color(91, 91, 91));
	SolidBrush blueBrush(Color(31, 42, 74));
	Pen blackPen(Color(255, 0, 0, 0), 3);
	//graphics.FillRectangle(&blueBrush, 0, 0, 130, 50);
	graphics.DrawRectangle(&blackPen, 0, 0, 130, 50);

	if (province)
	{
		RECT rc = province->GetRect();
		//TextInfo
		Font fontAlt(&FontFamily(DEF_FONT_FACE), 13);
		StringFormat format;
		format.SetAlignment(StringAlignmentCenter);
		format.SetLineAlignment(StringAlignmentCenter);
		WCHAR szName[256];
		province->GetText(szName, 128);
		graphics.DrawString(szName, -1, &fontAlt, Gdiplus::RectF(0, 0, 130, 50), &format, &whiteBrush);
	}

	return 0L;
}

LPVOID Province::OnMouseDown(HWND hWnd, UINT mButton, WPARAM sState, int X, int Y)
{
	Province *province = reinterpret_cast<Province*>(GetUserDataLong(hWnd));

	if (province)
	{
		if (province->isCity)
		{
			Transdep.times.Do(province->ID);
			return 0L;
		}

		int soums = std::count_if(Transdep.stops.begin(), Transdep.stops.end(), [province](const Stop stop)
		{
			return province->ID == stop.AimagID;
		});

		SafeDelete(Transdep.router.work.stops);

		Transdep.router.work.stops = new Destination[soums];
		int index = 0;
		std::for_each(Transdep.stops.begin(), Transdep.stops.end(),
			[province, &index](Stop stop) ->
			void
		{
			if (province->ID == stop.AimagID)
			{
				Transdep.router.work.stops[index].Register(CS_VREDRAW | CS_HREDRAW, province->GetInstance(), NULL, NULL, NULL, NULL);
				Transdep.router.work.stops[index].Create(stop.getClean().c_str(), WS_CHILD | BS_CENTER | BS_VCENTER, coord::to_rect(10, 10, 280, 60), Transdep.router.work, NULL, province->GetInstance());
				Transdep.router.work.stops[index].SetRegion(Rgn::Rounded(coord::size(280, 60)));
				Transdep.router.work.stops[index].FitAfterSibling(10, 10);
				Transdep.router.work.stops[index].ID = stop.ID;
				Transdep.router.work.stops[index].Show();
				index++;
			}
		});

	}

	return 0L;
}

LPVOID Destination::OnPaint(HWND hWnd, HDC hDC, PAINTSTRUCT pStruct)
{
	Destination *destination = reinterpret_cast<Destination*>(GetUserDataLong(hWnd));

	ULONG_PTR gdiplusToken;
	GdiplusStartupInput gdiplusStartupInput;
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

	Gdiplus::Graphics graphics(hDC);

	//Drawing Button
	SolidBrush whiteBrush(Color(0, 0, 0));
	/*
	SolidBrush blueBrush(Color(18, 51, 92));
	graphics.FillRectangle(&blueBrush, 0, 0, 280, 60);
	*/

	//Drawing GradientFill
	TRIVERTEX vertex[2];
	vertex[0].x = 0;
	vertex[0].y = 0;
	vertex[0].Red = 0x0000;
	vertex[0].Green = 0x8000;
	vertex[0].Blue = 0x8000;
	vertex[0].Alpha = 0x0000;

	vertex[1].x = 300;
	vertex[1].y = 80;
	vertex[1].Red = 0x0000;
	vertex[1].Green = 0xd000;
	vertex[1].Blue = 0xd000;
	vertex[1].Alpha = 0x0000;

	// Create a GRADIENT_RECT structure that 
	// references the TRIVERTEX vertices. 
	GRADIENT_RECT gRect;
	gRect.UpperLeft = 0;
	gRect.LowerRight = 1;

	// Draw a shaded rectangle. 
	::GdiGradientFill(hDC, vertex, 2, &gRect, 1, GRADIENT_FILL_RECT_H);

	if (destination)
	{
		//TextInfo
		Font fontAlt(&FontFamily(DEF_FONT_FACE), 13);
		StringFormat format;
		format.SetAlignment(StringAlignmentCenter);
		format.SetLineAlignment(StringAlignmentCenter);
		WCHAR szName[256];
		destination->GetText(szName, 128);
		graphics.DrawString(szName, -1, &fontAlt, Gdiplus::RectF(0, 0, 280, 60), &format, &whiteBrush);
	}

	return 0L;
}

LPVOID Destination::OnMouseDown(HWND hWnd, UINT mButton, WPARAM sState, int X, int Y)
{
	Destination *stopClicked = reinterpret_cast<Destination*>(GetUserDataLong(hWnd));

	if (stopClicked)
	{
		Transdep.times.Do(stopClicked->ID);
	}

	return 0L;
}
void Router::setVisible(bool bVisible)
{
	if (bVisible)
	{
		work.Show();
		menu.Show();
		Show();
	}
	else
	{
		work.Hide();
		menu.Hide();
		Hide();
	}
}

bool Router::Init(HINSTANCE hInst, HWND hParent, RECT rcParent)
{	
	if (!Initialize(hInst, hParent, TEXT("Transdep::Router"), rcParent, WS_CHILD))
		return FALSE;
	
	SetBkBrush(BRUSH_WHITE);

	RECT myRect = GetClientRect();
	myRect.bottom = coord::height(myRect);
	myRect.top = 0;
	myRect.left = myRect.right - MENU_WITH;
	if (!menu.Initialize(hInst, *this, myRect, TEXT("Transdep::Router::Menu")))
		return FALSE;

	myRect.left = 0;
	myRect.right = coord::width(GetRect()) - MENU_WITH;
	if (!work.Initialize(hInst, *this, myRect, TEXT("Transdep::Router::Selection")))
		return FALSE;
	
	menu.Show();
	work.Show();

	return true;
}

LPVOID Menu::OnMenuShow(HWND hWnd, UINT nShow)
{
	Menu *me = reinterpret_cast<Menu*>(GetUserDataLong(hWnd));

	if (me)
	{
		if (me->province)
			return 0L;

		me->aimag.Create(me->GetInstance(), *me, TEXT("ОРОН НУТАГ"), coord::to_rect(3, 10, coord::width(me->GetRect()) - 6, 30), cfg.Font(26), WS_CHILD | WS_VISIBLE | WS_GROUP | SS_CENTER | SS_NOTIFY);
		me->aimag.Show();
		me->province = new Province[me->Aimags()];
		for (UINT i = 0; i < me->Aimags(); i++)
		{
			me->province[i].Register(CS_VREDRAW | CS_HREDRAW, me->GetInstance(), NULL, NULL, NULL, NULL);
			me->province[i].Create(Transdep.aimags[i].getClean().c_str(), WS_CHILD | BS_CENTER | BS_VCENTER, coord::to_rect(3, 10, 130, 50), *me, NULL, me->GetInstance());
			me->province[i].FitAfterSibling(3, 5);
			me->province[i].Show();
			me->province[i].ID = Transdep.aimags[i].ID;
		}

		me->khot.Create(me->GetInstance(), *me, TEXT("ХОТУУД"), coord::to_rect(3, 10, coord::width(me->GetRect()) - 6, 30), cfg.Font(26), WS_CHILD | WS_VISIBLE | WS_GROUP | SS_CENTER | SS_NOTIFY);
		me->khot.Show();
		me->khot.FitAfterSibling(3, 30);
		me->city = new Province[2];
		me->city[0].isCity = true;
		me->city[0].Register(CS_VREDRAW | CS_HREDRAW, me->GetInstance(), NULL, NULL, NULL, NULL);
		if (me->city[0].Create(TEXT("Дархан"), WS_CHILD | BS_CENTER | BS_VCENTER, coord::to_rect(3, 10, 130, 50), *me, NULL, me->GetInstance()))
		{
			me->city[0].FitAfterSibling(3, 5);
			me->city[0].Show();
			me->city[0].ID = 10;
		}
		me->city[1].isCity = true;
		me->city[1].Register(CS_VREDRAW | CS_HREDRAW, me->GetInstance(), NULL, NULL, NULL, NULL);
		if (me->city[1].Create(TEXT("Эрдэнэт"), WS_CHILD | BS_CENTER | BS_VCENTER, coord::to_rect(3, 10, 130, 50), *me, NULL, me->GetInstance()))
		{
			me->city[1].FitAfterSibling(3, 5);
			me->city[1].Show();
			me->city[1].ID = 34;
		}

		me->gadaad.Create(me->GetInstance(), *me, TEXT("ОЛОН УЛС"), coord::to_rect(3, 10, coord::width(me->GetRect()) - 6, 30), cfg.Font(26), WS_CHILD | WS_VISIBLE | WS_GROUP | SS_CENTER | SS_NOTIFY);
		me->gadaad.Show();
		me->gadaad.FitAfterSibling(3, 30);
		me->foreign = new Province[2];
		me->foreign[0].Register(CS_VREDRAW | CS_HREDRAW, me->GetInstance(), NULL, NULL, NULL, NULL);
		if (me->foreign[0].Create(TEXT("ОХУ"), WS_CHILD | BS_CENTER | BS_VCENTER, coord::to_rect(3, 10, 130, 50), *me, NULL, me->GetInstance()))
		{
			me->foreign[0].FitAfterSibling(3, 5);
			me->foreign[0].Show();
			me->foreign[0].ID = 25;
		}
		me->foreign[1].Register(CS_VREDRAW | CS_HREDRAW, me->GetInstance(), NULL, NULL, NULL, NULL);
		if (me->foreign[1].Create(TEXT("БНХАУ"), WS_CHILD | BS_CENTER | BS_VCENTER, coord::to_rect(3, 10, 130, 50), *me, NULL, me->GetInstance()))
		{
			me->foreign[1].FitAfterSibling(3, 5);
			me->foreign[1].Show();
			me->foreign[1].ID = 26;
		}

	}

	return 0L;
}

const UINT Menu::Aimags()
{
	return Transdep.aimags.size();
}