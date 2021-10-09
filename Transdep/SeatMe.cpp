#include "../clientwnd.h"
#include "control.h"

#include "seatme.h"

using namespace transdep;

Autobus::Autobus()
	: CPicturePanel()
{
	SetClass(TEXT("KioskMan::Autobus"));
	
	dispatcher = 0;
	seat       = NULL;
	onPaint    = OnPaint;
}

Autobus::~Autobus()
{
	SafeDelete(seat);
}

void Autobus::SetBus(UINT bCase)
{
	busCase = bCase;
}

UINT Autobus::GetBus()
{
	return busCase;
}

void Autobus::Set(UINT dispatcherID, UINT car_type_id, UINT sitcount)
{
	for (int i = 0; i < TRANSDEP_MAX_RESERV; i++)
	{
		reserve[i] = 0;
	}

	size = static_cast<CarSize>(car_type_id);
	seat_count = sitcount;

	SafeDelete(seat);
	seat = new Seat[GetSeats()];

	DeleteBitmap();

	switch (size)
	{
	case csCab:
	{
		AssignBitmap(Bmp::LoadGDI(cfg.DataPath(TEXT("transdep\\car-cab.png")).c_str()));
		SetBus(csCab);
		car = cCab4;
	} break;
	case csSmall:
	{
		AssignBitmap(Bmp::LoadGDI(cfg.DataPath(TEXT("transdep\\car-bus-small.png")).c_str()));
		SetBus(csSmall);

		switch (seat_count)
		{
		case 10: car = cSmall10; break;
		case 11: car = cSmall11; break;
		case 14: car = cSmall14; break;
		}
	} break;
	case csMid:
	{
		AssignBitmap(Bmp::LoadGDI(cfg.DataPath(TEXT("transdep\\car-bus-mid.png")).c_str()));
		SetBus(csMid);

		switch (seat_count)
		{
		case 24: car = cMid24; break;
		case 25: car = cMid25; break;
		case 26: car = cMid26; break;
		case 28: car = cMid28; break;
		case 29: car = cMid29; break;
		case 34: car = cMid34; break;
		case 36: car = cMid36; break;
		}
	} break;
	case csLarge:
	{
		AssignBitmap(Bmp::LoadGDI(cfg.DataPath(TEXT("transdep\\car-bus-large.png")).c_str()));
		SetBus(csLarge);

		switch (seat_count)
		{
		case 35: car = cLarge35; break;
		case 38: car = cLarge38; break;
		case 40: car = cLarge40; break;
		case 41: car = cLarge41; break;
		case 43: car = cLarge43; break;
		case 44: car = cLarge44; break;
		case 45: car = cLarge45; break;
		case 48: car = cLarge48; break;
		case 49: car = cLarge49; break;
		case 53: car = cLarge53; break;
		case 54: car = cLarge54; break;
		}
	} break;
	}

	dispatcher = dispatcherID;

	SetSeats();
}

void Autobus::SetSeats()
{
	UINT SeatMatrix = 1;
	for (UINT i = 0; i < GetSeats(); i++)
	{
		seat[i].ID = (i + 1); 

		if (i < 2)// && size == csLarge) Эхний 1 - 2 суудлуудыг хөгжлийн бэрхшээлтэй иргэдэд зориулав
			seat[i].State = ssDisabledUnavailable;

		seat[i].Instant(GetInstance(), *this, coord::rect(GetSeatXY(seat[i].ID, &SeatMatrix), coord::size(46, 43)),
			Bmp::LoadGDI(cfg.DataPath(TEXT("transdep\\bus-seat.png")).c_str()), Seat::OnSeat, false);
	}
}

void Autobus::EnableSeats(const std::vector<UINT> empty_seats)
{
	for (UINT i = 0; i < GetSeats(); i++)
	{
		if (std::find(empty_seats.begin(), empty_seats.end(), seat[i].ID) != empty_seats.end())
			seat[i].State = (seat[i].State == ssDisabledUnavailable) ? ssDisabledAvailable : ssAvailable;

		seat[i].Show();
	}
}

POINT Autobus::GetSeatXY(UINT index, UINT* SeatToMatrix)
{
	LONG x = 315, y = 310;
	UINT iWidth = 56, iHeight = 48, rowCount = 5;

	switch (car)
	{
	case cCab4: // Done
	{
		x        = 465;
		y = 315;
		iWidth = 146;
		iHeight = 73;
		rowCount = 3;
		if (*SeatToMatrix == 1)
			*SeatToMatrix += 2;
	} break;
	case cSmall10: // Done
	{
		x        = 190;
		y        = 320;
		iWidth   = 126;
		iHeight  = 83;
		rowCount = 3;
		if (*SeatToMatrix == 1)
			*SeatToMatrix += 2;
	} break;
	case cSmall11: // Done
	{
		x        = 190;
		y        = 320;
		iWidth   = 126;
		iHeight  = 83;
		rowCount = 3;
		if (*SeatToMatrix == 1)
			++*SeatToMatrix;
	} break;
	case cSmall14: // Done
	{
		x        = 190;
		y        = 320;
		iWidth   = 96;
		iHeight  = 83;
		rowCount = 3;
		if (*SeatToMatrix == 1)
			++*SeatToMatrix;
	} break;
	case cMid24: // Done
	{
		x        = 190;
		y        = 335;
		iHeight  = 63;
		iWidth   = 66;
		rowCount = 3;
		if (*SeatToMatrix == 1)
			*SeatToMatrix += 2;
		if (*SeatToMatrix == 4)
			*SeatToMatrix += 3;
		if (*SeatToMatrix == 12)
			++*SeatToMatrix;
		if (*SeatToMatrix > 12)
		{
			iWidth = 56;
			x     += 30;
		}
	} break;
	case cMid25: // Done
	{
		x        = 290;
		y        = 320;
		iWidth   = 56;
		rowCount = 4;
		if (*SeatToMatrix == 1)
			*SeatToMatrix += 3;
		if (*SeatToMatrix == 7)
			*SeatToMatrix += 2;
		if (*SeatToMatrix == 11)
			*SeatToMatrix += 2;
		if (*SeatToMatrix > 3)
			x += 30;
	} break;
	case cMid26: //Done
	{
		x        = 300;
		y        = 320;
		iWidth   = 70;
		rowCount = 4;
		if (*SeatToMatrix == 1)
			*SeatToMatrix += 3;
		if (*SeatToMatrix == 7)
			*SeatToMatrix += 2;
		if (*SeatToMatrix == 12)
			++*SeatToMatrix;
	} break;
	case cMid28: // Done
	{
		x       = 300;
		iHeight = 43;
		iWidth  = 70;
		if (*SeatToMatrix == 1)
			*SeatToMatrix += 4;
		if (*SeatToMatrix < 6)
			break;
		if (*SeatToMatrix == 8)
			*SeatToMatrix += 3;
	} break;
	case cMid29: // Done
	{
		x       = 300;
		iHeight = 43;
		iWidth  = 70;
		if (*SeatToMatrix == 1)
			*SeatToMatrix += 3;
		//if (*SeatToMatrix < 6)
			//break;
		if (*SeatToMatrix > 5)
			x = 310;
		if (*SeatToMatrix == 8)
			*SeatToMatrix += 3;
	} break;
	case cMid34: // Done
	{
		x       = 300;
		iHeight = 43;
		iWidth  = 54;
		if (*SeatToMatrix == 1)
		{
			*SeatToMatrix = *SeatToMatrix + 4;
			break;
		}
		if (*SeatToMatrix == 6)
			*SeatToMatrix += 5;
		if (*SeatToMatrix > 6)
			x = 325;
	} break;
	case cMid36: // Done
	{
		x      = 390;
		iHeight = 43;
		if ((*SeatToMatrix >= 26) && *SeatToMatrix <= 30)
		{
			if (*SeatToMatrix % rowCount == 0)
				y += iHeight * (UINT)(*SeatToMatrix % rowCount);
			else
				y += iHeight * (rowCount - (UINT)(*SeatToMatrix % rowCount));
			++*SeatToMatrix;
			return coord::point(300, y + 30);
		}
		
		if (*SeatToMatrix == 3)
			*SeatToMatrix += 3;
		if ((*SeatToMatrix % rowCount == 3) && *SeatToMatrix < 40)
			++*SeatToMatrix;

		if (*SeatToMatrix > 30)
		{
			if (*SeatToMatrix % rowCount == 0)
			{
				x += iWidth * (UINT)(*SeatToMatrix / rowCount - 2);
				y += iHeight * (UINT)(*SeatToMatrix % rowCount);
			}
			else {
				x += iWidth * (UINT)(*SeatToMatrix / rowCount - 1);
				y += iHeight * (rowCount - (UINT)(*SeatToMatrix % rowCount));
			}
			++*SeatToMatrix;
			return coord::point(x, y + 30);
		}
	} break;
	case cLarge35: // Done
	{
		iWidth = 70;
		if (*SeatToMatrix == 38)
			*SeatToMatrix += 3;
	} break;
	case cLarge38: // Done
	{
		x      = 315;
		iWidth = 66;
		if (*SeatToMatrix == 1)
			*SeatToMatrix += 4;
	} break;
	case cLarge40: // Done
	{
		x      = 315;
		iWidth = 66;
		if (*SeatToMatrix % rowCount == 3)
			++*SeatToMatrix;
	} break;
	case cLarge41: // Done
	{
		x      = 315;
		iWidth = 66;
		if ((*SeatToMatrix % rowCount == 3) && *SeatToMatrix < 45)
			++*SeatToMatrix;
	} break;
	case cLarge43: // Done
	{
		x = 315;
		if (*SeatToMatrix == 3)
			*SeatToMatrix += 3;
	} break;
	case cLarge44: // Done
	case cLarge45: x = 315; break; // Done
	case cLarge48: // Done
	case cLarge49: // Done
	{
		x      = 315;
		iWidth = 56;
	} break;
	case cLarge53: // Done
	{
		x      = 280;
		iWidth = 56;
	} break;
	case cLarge54: // Done
	{

		x      = 280;
		iWidth = 51;
		if (*SeatToMatrix == 1)
			*SeatToMatrix += 4;
	} break;
	}

	if ((*SeatToMatrix % rowCount == 3) && rowCount == 5)
	{
		if (car == cLarge40 || car == cLarge44 || car == cLarge48)
			++*SeatToMatrix;
		else
			if ((GetSeats() - 5) > index)
				++*SeatToMatrix;
	}

	if (*SeatToMatrix % rowCount == 0)
	{
		x += iWidth * (UINT)(*SeatToMatrix / rowCount-1);
		y += iHeight * (UINT)(*SeatToMatrix % rowCount);
	}
	else
	{
		x += iWidth * (UINT)(*SeatToMatrix / rowCount);
		y += iHeight * (rowCount - (UINT)(*SeatToMatrix % rowCount));
	}

	++*SeatToMatrix;

	return coord::point(x, y + 30);
}

Seat* Autobus::SeatbyID(UINT seat_id)
{
	for (UINT i = 0; i < GetSeats(); i++)
	{
		if (seat[i].ID == seat_id)
			return &seat[i];
	}
	return NULL;
}

UINT Autobus::GetSeats()
{
	return seat_count;
}

Seat::Seat()
	: CHoverBitmapWindow()
{
	SetClass(TEXT("KioskMan::Autobus::Seat"));

	ID      = 0;
	State   = ssUnavailable;
	onPaint = OnPaint;

	MakeFixable();
}

LPVOID Seat::OnPaint(HWND OwnerWnd, HDC hDC, PAINTSTRUCT pStruct)
{
	Seat *seat = reinterpret_cast<Seat*>(GetUserDataLong(OwnerWnd));

	if (seat)
	{
		RECT rc = seat->GetRect();
		if (seat->GetBitmap())
		{
			HDC dcImage;
			dcImage = ::CreateCompatibleDC(hDC);
			if (dcImage)
			{
				::SelectObject(dcImage, seat->GetBitmap());
				::BitBlt(hDC, 0, 0, coord::width(rc), coord::height(rc), dcImage, coord::width(rc) * seat->GetFrame(), 0, SRCCOPY);
				::DeleteDC(dcImage);
			}
		}

		rc = seat->GetClientRect();
		rc.top = 0;
		rc.left = -10;
		::SelectObject(hDC, cfg.Font(16));
		::SetBkMode(hDC, TRANSPARENT);
		::SetTextColor(hDC, RGB_WHITE);
		::DrawText(hDC, std::to_wstring(seat->ID).c_str(), -1, &rc, DT_CENTER | DT_SINGLELINE | DT_VCENTER);
	}
	return 0L;
}

UINT Seat::GetFrame()
{
	switch (State)
	{
	case ssAvailable: return 1;
	case ssSelected: return 2;
	case ssDisabledAvailable: return 3;
	case ssDisabledUnavailable: return 4;
	case ssDisabledSelected: return 5;
	}
	return 0;
}

void Seat::OnSeat(HWND hWnd)
{
	Seat *seat = reinterpret_cast<Seat*>(GetUserDataLong(hWnd));

	if (seat)
	{
		if (seat->State == ssUnavailable || seat->State == ssDisabledUnavailable)
			return;

		switch (seat->State)
		{
		case ssAvailable: seat->State = ssSelected; break;
		case ssSelected: seat->State = ssAvailable; break;
		case ssDisabledAvailable: seat->State = ssDisabledSelected; break;
		case ssDisabledSelected: seat->State = ssDisabledAvailable; break;
		}

		Transdep.Bus.Reserve(seat->ID, (seat->State == ssSelected || seat->State == ssDisabledSelected));
		seat->Redraw();
	}
}

UINT Autobus::Reserved()
{
	for (int i = 0; i < TRANSDEP_MAX_RESERV; i++)
	{
		if (!reserve[i])
			return i;
	}
	return TRANSDEP_MAX_RESERV;
}

void Autobus::Reserve(UINT ID, bool Add)
{
	if (Add)
	{
		if (Reserved() == TRANSDEP_MAX_RESERV)
		{
			Seat* deselect = SeatbyID(reserve[0]);
			for (int i = 0; i < TRANSDEP_MAX_RESERV - 1; i++)
			{
				reserve[i] = reserve[i + 1];
			}
			if (deselect)
			{
				deselect->State = (deselect->State == ssSelected) ? ssAvailable : ssDisabledAvailable;
				deselect->Redraw();
			}
		}		
	}
	else
	{
		for (int i = 0; i < TRANSDEP_MAX_RESERV - 1; i++)
		{
			if (reserve[i] == ID)
			{
				for (int j = i + 1; j < TRANSDEP_MAX_RESERV; j++)
				{
					reserve[j-1] = reserve[j];
				}
				break;
			}
		}
	}

	reserve[TRANSDEP_MAX_RESERV - 1] = 0;

	if (Add)
		reserve[Reserved()] = ID;

	if (Reserved())
	{
		Client.footer.right.ShowText((TEXT("(") + std::to_wstring(Reserved()) + TEXT(") ТАСАЛБАР ЗАХИАЛАХ")).c_str());
		Client.footer.right.AssignBitmap(Bmp::LoadGDI(cfg.DataPath(TEXT("footer\\next-arrow.png")).c_str()));
		Client.footer.right.SetColor(BRUSH_TERTIARY);
	}
	else
		Client.footer.right.Hide();
}

void Autobus::CancelAll()
{
	for (int i = 0; i < TRANSDEP_MAX_RESERV; i++)
	{
		if (reserve[i])
		{
			Seat* deselect = SeatbyID(reserve[i]);
			if (deselect)
			{
				deselect->State = (deselect->State == ssSelected) ? ssAvailable : ssDisabledAvailable;
				deselect->Redraw();
			}
			reserve[i] = 0;
		}
	}
}

LPVOID Autobus::OnPaint(HWND OwnerWnd, HDC hDC, PAINTSTRUCT pStruct)
{
	Autobus *bus = reinterpret_cast<Autobus*>(GetUserDataLong(OwnerWnd));
	if (bus)
	{
		RECT rc = bus->GetClientRect();
		if (bus->GetBitmap())
		{
			HDC dcImage;
			dcImage = ::CreateCompatibleDC(hDC);
			SIZE bmpSize = bus->GetBitmapDimension();
			if (dcImage)
			{
				::SelectObject(dcImage, bus->GetBitmap());
				::BitBlt(hDC, 113, 283, bmpSize.cx, bmpSize.cy, dcImage, 0, 0, SRCCOPY);
				if (bus->GetBus() == csLarge)
					::SelectObject(dcImage, Bmp::LoadGDI(cfg.DataPath(TEXT("transdep\\car-bus-large-case.png")).c_str()));
				else if (bus->GetBus() == csMid)
					::SelectObject(dcImage, Bmp::LoadGDI(cfg.DataPath(TEXT("transdep\\car-bus-mid-case.png")).c_str()));
				else if (bus->GetBus() == csSmall)
					::SelectObject(dcImage, Bmp::LoadGDI(cfg.DataPath(TEXT("transdep\\car-bus-small-case.png")).c_str()));
				else if (bus->GetBus() == csCab)
					::SelectObject(dcImage, Bmp::LoadGDI(cfg.DataPath(TEXT("transdep\\car-cab-case.png")).c_str()));
				::BitBlt(hDC, 82, 881, bmpSize.cx, bmpSize.cy, dcImage, 0, 0, SRCCOPY);
				::DeleteDC(dcImage);
			}
		}

		::SelectObject(hDC, ::CreatePen(NULL, 1, Color::MakeARGB(0, 217, 217, 217)));
		::MoveToEx(hDC, 567, 867, 0);
		::LineTo(hDC, 567, 1060);

		::SelectObject(hDC, ::CreatePen(NULL, 1, Color::MakeARGB(0, 217, 217, 217)));
		::MoveToEx(hDC, 966, 48, 0);
		::LineTo(hDC, 63, 48);

		::SelectObject(hDC, ::CreatePen(NULL, 1, Color::MakeARGB(0, 217, 217, 217)));
		::MoveToEx(hDC, 966, 214, 0);
		::LineTo(hDC, 63, 214);

		::SelectObject(hDC, cfg.Font(15));
		::SetBkMode(hDC, TRANSPARENT);
		::SetTextColor(hDC, RGB_BLACK);

		POINT p = coord::point(84, 93);
		std::wstring Direction = TEXT("ЧИГЛЭЛ: ") + Transdep.passage.StartAimag + TEXT(" ") + Transdep.passage.StartStop + TEXT(" зогсоолоос ") + Transdep.passage.DirEndStop + TEXT(" хүртэл.");
		::TextOut(hDC, p.x, p.y, Direction.c_str(), Direction.size());

		p.y += 30;
		std::wstring LeaveDate = TEXT("ОГНОО: ") + Transdep.passage.LeaveDate + TEXT(" өдөр / ") + Transdep.passage.LeaveTime + TEXT(" цагаас");
		::TextOut(hDC, p.x, p.y, LeaveDate.c_str(), LeaveDate.size());

		p.y += 30;
		std::wstring EndStop = TEXT("БУУХ ЗОГСООЛ: ") + Transdep.passage.EndAimag + TEXT(" / ") + Transdep.passage.EndStop;
		::TextOut(hDC, p.x, p.y, EndStop.c_str(), EndStop.size());

		//
		RECT rect = bus->GetClientRect();
		::Rectangle(hDC, bus->GetClientRect().left + 575, bus->GetClientRect().top + 70, bus->GetClientRect().right -108, bus->GetClientRect().bottom - 874);

		p = coord::point(585, 80);

		std::wstring Info = TEXT("ТЭЭВЭРЛЭГЧ");
		::TextOut(hDC, p.x, p.y, Info.c_str(), Info.size());

		p.y += 20;
		std::wstring Company1 = TEXT("ААН нэр: ");
		::TextOut(hDC, p.x, p.y, Company1.c_str(), Company1.size());
		p.x += 105;
		std::wstring Company = Transdep.passage.Company + TEXT(" ");
		::TextOut(hDC, p.x, p.y, Company.c_str(), Company.size());

		p.y += 15;
		p.x = 585;
		std::wstring CarMark1 = TEXT("Марк загвар: ");
		::TextOut(hDC, p.x, p.y, CarMark1.c_str(), CarMark1.size());
		p.x += 105;
		std::wstring CarMark = Transdep.passage.Car + TEXT(" ");
		::TextOut(hDC, p.x, p.y, CarMark.c_str(), CarMark.size());

		p.y += 15;
		p.x = 585;
		std::wstring CarNo1 = TEXT("Улсын дугаар: ");
		::TextOut(hDC, p.x, p.y, CarNo1.c_str(), CarNo1.size());
		p.x += 105;
		std::wstring CarNo = Transdep.passage.Register + TEXT(" ");
		::TextOut(hDC, p.x, p.y, CarNo.c_str(), CarNo.size());

		p.y += 15;
		p.x = 585;
		std::wstring CarType1 = TEXT("Ангилал: ");
		::TextOut(hDC, p.x, p.y, CarType1.c_str(), CarType1.size());
		p.x += 105;
		std::wstring CarType = Transdep.passage.Type + TEXT(" ");
		::TextOut(hDC, p.x, p.y, CarType.c_str(), CarType.size());

		p.y += 15;
		p.x = 585;
		std::wstring SeatCnt1 = TEXT("Нийт суудлын тоо: ");
		::TextOut(hDC, p.x, p.y, SeatCnt1.c_str(), SeatCnt1.size());
		p.x += 105;
		std::wstring SeatCnt = Transdep.passage.Seats + TEXT(" ");
		::TextOut(hDC, p.x, p.y, SeatCnt.c_str(), SeatCnt.size());

		p.y += 15;
		p.x = 585;
		std::wstring Insurance1 = TEXT("Даатгалын ААН: ");
		::TextOut(hDC, p.x, p.y, Insurance1.c_str(), Insurance1.size());
		p.x += 105;
		std::wstring Insurance = Transdep.passage.Insurance + TEXT(" ");
		::TextOut(hDC, p.x, p.y, Insurance.c_str(), Insurance.size());

		//
		::SelectObject(hDC, cfg.Font(24));
		p = coord::point(406, 254);

		std::wstring chooseSeat = TEXT("ТА СУУДЛАА СОНГОНО УУ");
		::TextOut(hDC, p.x, p.y, chooseSeat.c_str(), chooseSeat.size());

		//
		if (bus->seat[0].GetBitmap())
		{
			HDC dcImage;
			dcImage = ::CreateCompatibleDC(hDC);
			RECT rcSeat = bus->seat[0].GetRect();
			if (dcImage)
			{
				::SelectObject(dcImage, bus->seat[0].GetBitmap());
				::SelectObject(hDC, cfg.Font(24));//24

				::StretchBlt(hDC, 600, 878, 46, 43, dcImage, 0, 0, coord::width(rcSeat), coord::height(rcSeat), SRCCOPY);
				::StretchBlt(hDC, 600, 878 + 46 * 1, 46, 43, dcImage, 1 * coord::width(rcSeat), 0, coord::width(rcSeat), coord::height(rcSeat), SRCCOPY);
				::StretchBlt(hDC, 600, 878 + 46 * 2, 46, 43, dcImage, 2 * coord::width(rcSeat), 0, coord::width(rcSeat), coord::height(rcSeat), SRCCOPY);
				::StretchBlt(hDC, 600, 878 + 46 * 3, 46, 43, dcImage, 3 * coord::width(rcSeat), 0, coord::width(rcSeat), coord::height(rcSeat), SRCCOPY);

				p = coord::point(659, 888);

				std::wstring seat_desc = TEXT("Сонгох боломжгүй. Захиалга хийгдсэн.");
				::TextOut(hDC, p.x, p.y, seat_desc.c_str(), seat_desc.size());

				seat_desc = TEXT("Сонгох боломжтой.");
				p.y += 46;
				::TextOut(hDC, p.x, p.y, seat_desc.c_str(), seat_desc.size());

				seat_desc = TEXT("Таны сонгосон суудал.");
				p.y += 45;
				::TextOut(hDC, p.x, p.y, seat_desc.c_str(), seat_desc.size());

				seat_desc = TEXT("Хөгжлийн бэрхшээлтэй иргэдэд ");
				p.y += 38;
				::TextOut(hDC, p.x, p.y, seat_desc.c_str(), seat_desc.size());
				seat_desc = TEXT("зориулагдсан суудал.");
				p.y += 20;
				::TextOut(hDC, p.x, p.y, seat_desc.c_str(), seat_desc.size());

				::DeleteDC(dcImage);
			}
		}

		//
	}
	return 0L;
}

void Autobus::GetEmptySeats(LPVOID arg)
{
	std::vector<UINT> empty_seats;
	if (CInternet::CheckConnection())
	{
		std::wstring hdr = TEXT("Content-Type: application/x-www-form-urlencoded; charset=utf-8\r\n");
		std::string data = "machine_id=" + std::to_string(cfg.GetID());
		data            += "&id_dispatcher=" + std::to_string(Transdep.Bus.dispatcher);

		pugi::xml_document get_Empty_Seat;
		XML::LoadFromBuffer(&get_Empty_Seat,
			Server::Post(TEXT("service/get_empty_seat"), data.c_str()));

/*
		XML::LoadFromBuffer(&get_Empty_Seat,
			CInternet::PostData(
			METAKIOSK_SERVER,
			TEXT("service/get_empty_seat"),
			hdr.c_str(),
			hdr.size(),
			(LPVOID)data.c_str(),
			data.length()));
*/

		if (get_Empty_Seat)
		{
			pugi::xml_node element = get_Empty_Seat.child(TEXT("DataTable"))
				.child(TEXT("diffgr:diffgram"))
				.child(TEXT("DocumentElement"))
				.first_child();

			UINT seat_no;
			while (element != NULL)
			{
				RawData::SetInt(seat_no, element.child_value(TEXT("seat_no")));
				empty_seats.push_back(seat_no);
				element = element.next_sibling();
			}
		}
	}

	Transdep.Bus.EnableSeats(empty_seats);

	Client.loading.Hide();
	Client.footer.left.ShowText(TEXT("БУЦАХ"));
}