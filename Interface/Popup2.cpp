#include "../clientwnd.h"
#include "popup2.h"

using namespace transdep;

Province2::Province2()
	: CWnd()
{
	SetClass(TEXT("TVM::Popup::Menu::Button"));

	isCity = false;
	onPaint = OnPaint;
	onMouseDown = OnMouseDown;
}

Selection2::Selection2()
	: CWnd()
{
	SetClass(TEXT("TVM::Popup::Selection::Button"));

	onPaint = OnPaint;
	onMouseDown = OnMouseDown;
}

TransparentStatic2::TransparentStatic2()
	: CStatic()
{
	onEraseBackground = OnEraseBackground;
}

LPVOID TransparentStatic2::OnEraseBackground(HWND hWnd, HDC hDC)
{
	return (LPVOID)(HBRUSH)::GetStockObject(BLACK_BRUSH);
}

Menu2::Menu2()
	: CPanel()
{
	SetClass(TEXT("KioskMan::Popup::Menu"));
	province = NULL;
	onShow = OnMenuShow;
	onCtlColorStatic = OnCtlColorStatic;
}

Menu2::~Menu2()
{
	SafeDelete(province);
}

Work2::Work2()
{
	SetClass(TEXT("KioskMan::Popup::Selection"));
	stops = NULL;
}

Work2::~Work2()
{
	SafeDelete(stops);
}

LPVOID Menu2::OnCtlColorStatic(HWND hWnd, HDC hStaticDC, HWND hStaticCtl)
{
	Menu2 *me = reinterpret_cast<Menu2*>(GetUserDataLongPtr(hWnd));
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

LPVOID Province2::OnPaint(HWND hWnd, HDC hDC, PAINTSTRUCT pStruct)
{
	Province2* province = reinterpret_cast<Province2*>(GetUserDataLongPtr(hWnd));

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

LPVOID Province2::OnMouseDown(HWND hWnd, UINT mButton, WPARAM sState, int X, int Y)
{
	Province2* province = reinterpret_cast<Province2*>(GetUserDataLongPtr(hWnd));

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

		SafeDelete(Transdep.popup1.work2.stops);

		Transdep.popup1.work2.stops = new Selection2[soums];
		int index = 0;
		std::for_each(Transdep.stops.begin(), Transdep.stops.end(),
			[province, &index](Stop stop) ->
			void
		{
			if (province->ID == stop.AimagID)
			{
				Transdep.popup1.work2.stops[index].Register(CS_VREDRAW | CS_HREDRAW, province->GetInstance(), NULL, NULL, NULL, NULL);
				Transdep.popup1.work2.stops[index].Create(stop.getClean().c_str(), WS_CHILD | BS_CENTER | BS_VCENTER, coord::to_rect(10, 10, 280, 60), Transdep.popup1.work2, NULL, province->GetInstance());
				Transdep.popup1.work2.stops[index].SetRegion(Rgn::Rounded(coord::size(280, 60)));
				Transdep.popup1.work2.stops[index].FitAfterSibling(10, 10);
				Transdep.popup1.work2.stops[index].ID = stop.ID;
				Transdep.popup1.work2.stops[index].Show();
				index++;
			}
		});

	}

	return 0L;
}

LPVOID Selection2::OnPaint(HWND hWnd, HDC hDC, PAINTSTRUCT pStruct)
{
	Selection2* selection = reinterpret_cast<Selection2*>(GetUserDataLongPtr(hWnd));

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

	if (selection)
	{
		//TextInfo
		Font fontAlt(&FontFamily(DEF_FONT_FACE), 13);
		StringFormat format;
		format.SetAlignment(StringAlignmentCenter);
		format.SetLineAlignment(StringAlignmentCenter);
		WCHAR szName[256];
		selection->GetText(szName, 128);
		graphics.DrawString(szName, -1, &fontAlt, Gdiplus::RectF(0, 0, 280, 60), &format, &whiteBrush);
	}

	return 0L;
}

LPVOID Selection2::OnMouseDown(HWND hWnd, UINT mButton, WPARAM sState, int X, int Y)
{
	Selection2* stopClicked = reinterpret_cast<Selection2*>(GetUserDataLongPtr(hWnd));

	if (stopClicked)
	{
		//Transdep.times.Do(stopClicked->ID);
		Worker.SetDestinationId(stopClicked->ID);
		Transdep.OnSelectionScreen();
	}

	return 0L;
}

Popup1::Popup1()
	: CPanel()
{
	SetClass(TEXT("KioskMan::Popup"));
}

bool Popup1::Init(HINSTANCE hInst, HWND hParent, RECT rcParent)
{
	Register(CS_DBLCLKS | CS_PARENTDC | CS_SAVEBITS, hInst, NULL, NULL, (HBRUSH) ::GetStockObject(HOLLOW_BRUSH));
	Create(TEXT("Transdep::Popup::Menu"), WS_CHILD, rcParent, hParent, NULL, hInst);

	RECT myRect = rcParent;
	myRect.bottom = coord::height(myRect) - 100;
	myRect.right = coord::width(myRect) - 10;
	myRect.top = 100;
	myRect.left = myRect.right - 400;

	menu2.Register(CS_VREDRAW | CS_HREDRAW, GetInstance(), NULL, NULL, (HBRUSH) ::GetStockObject(HOLLOW_BRUSH));
	menu2.Create(TEXT(""), WS_CHILD, myRect, *this, NULL, GetInstance());

	myRect.left = 10;
	myRect.right = coord::width(GetRect()) - 410;

	work2.Register(CS_VREDRAW | CS_HREDRAW, GetInstance(), NULL, NULL, (HBRUSH) ::GetStockObject(HOLLOW_BRUSH));
	work2.Create(TEXT(""), WS_CHILD, myRect, *this, NULL, GetInstance());



	menu2.Show();
	work2.Show();

	return true;
}

LPVOID Menu2::OnMenuShow(HWND hWnd, UINT nShow)
{
	Menu2* me = reinterpret_cast<Menu2*>(GetUserDataLongPtr(hWnd));

	if (me)
	{
		if (me->province)
			return 0L;

		me->aimag.Create(me->GetInstance(), *me, TEXT("ОРОН НУТАГ"), coord::to_rect(3, 10, coord::width(me->GetRect()) - 6, 30), cfg.Font(26), WS_CHILD | WS_VISIBLE | WS_GROUP | SS_CENTER | SS_NOTIFY);
		me->aimag.Show();
		me->province = new Province2[me->Aimags()];
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
		me->city = new Province2[2];
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
		me->foreign = new Province2[2];
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

const UINT Menu2::Aimags()
{
	return Transdep.aimags.size();
}