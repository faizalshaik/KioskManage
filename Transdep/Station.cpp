#include "types/region.h"

#include "station.h"

#include "control.h"

using namespace transdep;

Station::Station()
	: CWnd()
{
	SetClass(TEXT("KioskMan:Station"));

	ID      = 0;
	Type    = PassStation;
	Size    = MidStation;
	Point.x = 0;
	Point.y = 0;
}

bool Station::Init(HINSTANCE hInst, HWND hParent, Stop stop)
{
	if (!Create(stop.getClean().c_str(), WS_CHILD, coord::to_rect(0, 0, 10, 10), hParent, NULL, hInst, this))
		return false;

	ID = stop.ID;
	SetBkBrush(::CreateSolidBrush(RGB(255, 0, 0)));

	if (std::find(Transdep.start.begin(), Transdep.start.end(), ID) != Transdep.start.end())
	{
		Type = StartStation;
		//SetPoint(Transdep.map.Width() / 2, Transdep.map.Height() / 2);
		ShowNormal();
	}	

	return true;
}

bool Station::IsVisible()
{
	return (::GetWindowLong(GetHandle(), GWL_STYLE) & WS_VISIBLE) != 0;
}

Stop Station::GetInfo()
{
	Stop inf = *Transdep.StopByID(ID);
	return inf;
}

void Station::SetPoint(LONG x, LONG y)
{
	Point = coord::point(x, y);

	SIZE size = GetTextExtendPoint();
	SetRect(coord::to_rect(x - size.cx / 2, y - size.cy / 2, size.cx, size.cy));

	if (Type == MidStation)
	{
		SetRegion(Rgn::Circle(Point));
	}
	else
	{
		SetRegion(Rgn::Rounded(size));
	}
}

LPVOID Station::OnPaint(HWND OwnerWnd, HDC hDC, PAINTSTRUCT ps)
{
	Station* me = reinterpret_cast<Station*>(GetUserDataLong(OwnerWnd));
	
	if (me)
	{

	}

	return 0L;
}