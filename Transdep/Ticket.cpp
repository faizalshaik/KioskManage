#include "control.h"
#include "ticket.h"

#include "../config.hpp"
#include "../clientwnd.h"


using namespace transdep;

void Passenger::Empty()
{
	id = 0;
	LastName = TEXT("");
	FirstName = TEXT("");
	Age = 0;
	Gender = TEXT("");
	Password = TEXT("");
	Register = TEXT("");
	Address = TEXT("");
	Description = TEXT("");
	AimagKhot = 0;
	SumDuureg = 0;
	Heseg = 0;
	UpdatedAt = 0;

	QR = TEXT("");
	Lottery = TEXT("");
	DDTD = TEXT("");
	TTD = TEXT("");
}

Ticket::Ticket()
	: CPanel()
{
	SetClass(TEXT("KioskMan:Transdep::Ticket"));
	onCreate = OnCreate;
	onCommand = OnCommand;
	onPaint = OnPaint;
}

Passenger Ticket::GetByRegister(LPCTSTR szRegister)
{
	Passenger person;
	person.Empty();
	if (CInternet::CheckConnection())
	{
		std::wstring hdr = TEXT("Content-Type: application/x-www-form-urlencoded; charset=utf-8\r\n");
		std::string  rid = string::utf8encode(TEXT("register=") + std::wstring(szRegister));

		pugi::xml_document     doc;
		pugi::xml_parse_result result =
			doc.load_string(CInternet::PostData(TEXT("192.168.0.20"), TEXT("civil/"), hdr.c_str(), hdr.size(), (LPVOID)rid.c_str(), rid.length()).c_str());
		if (result)
		{
			pugi::xml_node element = doc.child(TEXT("person"));
			if (element != NULL)
			{
				RawData::SetInt(person.id, element.child_value(TEXT("id")));
				person.LastName = element.child_value(TEXT("lastname"));
				person.FirstName = element.child_value(TEXT("firstname"));
				RawData::SetInt(person.Age, element.child_value(TEXT("age")));
			}
		}
		else
			OutputDebugString(TEXT("Connection problem!"));
	}
	return person;
}

LPVOID Ticket::OnCreate(HWND hWnd, LPARAM lParam)
{
	Ticket *ticket = reinterpret_cast<Ticket*>(GetUserDataLong(hWnd));
	if (ticket)
	{
		ticket->SetBkBrush(::CreateSolidBrush(RGB(240, 240, 240)));

		RECT rc = ticket->GetClientRect();

		ticket->Register.Create(ticket->GetInstance(), *ticket, TEXT("Регистр:"), coord::to_rect(10, 45, 100, 30), cfg.Font(20),
			WS_CHILD | WS_VISIBLE | WS_GROUP | SS_RIGHT | SS_NOTIFY);
		ticket->Register.FitAfterSibling();
		ticket->txtRegister.Create(ticket->GetInstance(), *ticket, TEXT(""), coord::to_rect(10, 10, 468, 28), cfg.Font(24));
		ticket->txtRegister.FitAfterSibling(10, 10, 0, -5);
		ticket->txtRegister.onMouseDown = OnEditDown;
		ticket->txtRegister.SetLimitText(10);
		ticket->txtRegister.onPaint = CKeyboard::OnEditPaint;

		//		ticket->SendRegister.Create(ticket->GetInstance(), *ticket, TEXT("Илгээх"), coord::to_rect(10, 10, 90, 28), cfg.Font(18));
		//		ticket->SendRegister.SetClickEvent(OnSendRegister);
		//		ticket->SendRegister.FitAfterSibling();
		//		ticket->SendRegister.Hide();

		ticket->FullName.Create(ticket->GetInstance(), *ticket, TEXT("Овог нэр:"), coord::to_rect(10, 10, 100, 30), cfg.Font(20),
			WS_CHILD | WS_VISIBLE | WS_GROUP | SS_RIGHT | SS_NOTIFY);
		ticket->FullName.FitAfterSibling();
		ticket->txtFullName.Create(ticket->GetInstance(), *ticket, TEXT(""), coord::to_rect(10, 10, 468, 28), cfg.Font(24));
		ticket->txtFullName.FitAfterSibling(10, 10, 0, -5);
		ticket->txtFullName.onMouseDown = OnEditDown;
		ticket->txtFullName.SetLimitText(35);
		ticket->txtFullName.onPaint = CKeyboard::OnEditPaint;

		ticket->Age.Create(ticket->GetInstance(), *ticket, TEXT("Нас:"), coord::to_rect(rc.left + (coord::width(rc) / 2) + 5, rc.top + 30, 100, 30), cfg.Font(20),
			WS_CHILD | WS_VISIBLE | WS_GROUP | SS_RIGHT | SS_NOTIFY);
		ticket->Age.FitAfterSibling(10, 20);

		ticket->Adult.Create(ticket->GetInstance(), *ticket, TEXT("Том хүн"), coord::to_rect(10, 20, 90, 40), cfg.Font(20));
		ticket->Adult.FitAfterSibling(80, 10, 0, -10);
		ticket->Adult.Check();
		ticket->Adult.SetClickEvent(OnChangeSomething);

		ticket->Child.Create(ticket->GetInstance(), *ticket, TEXT("Хүүхэд (7-12 нас)"), coord::to_rect(10, 10, 180, 40), cfg.Font(20));
		ticket->Child.FitAfterSibling(20);
		ticket->Child.SetClickEvent(OnChangeSomething);

		RECT rcAge = ticket->Age.GetRect();
		rcAge.top += 30 - 100;
		rcAge.bottom += 30 - 100;
		rcAge.left = 10;
		rcAge.right = 110;
		ticket->Insurance.Create(ticket->GetInstance(), *ticket, TEXT("Даатгал:"), rcAge
			, cfg.Font(20), WS_CHILD | WS_VISIBLE | WS_GROUP | SS_RIGHT | SS_NOTIFY);
		ticket->checkInsurance.Create(ticket->GetInstance(), *ticket, TEXT("Амь даатгал"), coord::to_rect(10, 20, 130, 40), cfg.Font(20));
		ticket->checkInsurance.FitAfterSibling(50, 10, 0, -10);
		ticket->checkInsurance.SetClickEvent(OnChangeSomething);

		rcAge.top += 50;
		rcAge.bottom += 50;
		ticket->Seat.Create(ticket->GetInstance(), *ticket, TEXT("Суудал:"), rcAge, cfg.Font(20),
			WS_CHILD | WS_VISIBLE | WS_GROUP | SS_RIGHT | SS_NOTIFY);
		ticket->txtSeat.Create(ticket->GetInstance(), *ticket, TEXT(""), coord::to_rect(10, 10, 60, 28), cfg.Font(24), WS_CHILDWINDOW | WS_VISIBLE | WS_BORDER | WS_TABSTOP | ES_CENTER | ES_AUTOHSCROLL);
		ticket->txtSeat.FitAfterSibling();
		ticket->txtSeat.SetReadOnly(true);

		ticket->Payment.Create(ticket->GetInstance(), *ticket, TEXT("ТӨЛБӨР:"), coord::to_rect(10, 10, 100, 30), cfg.Font(20, 0, 0, 0, FW_BOLD),
			WS_CHILD | WS_VISIBLE | WS_GROUP | SS_RIGHT | SS_NOTIFY);
		ticket->Payment.FitAfterSibling(80);
		ticket->txtPayment.Create(ticket->GetInstance(), *ticket, TEXT(""), coord::to_rect(10, 10, 220, 30), cfg.Font(24), WS_CHILDWINDOW | WS_VISIBLE | WS_BORDER | WS_TABSTOP | ES_CENTER | ES_AUTOHSCROLL);
		ticket->txtPayment.FitAfterSibling();
		ticket->txtPayment.SetReadOnly(true);
	}
	return 0L;
}

void Ticket::OnSendRegister(HWND hWnd)
{
	Ticket *ticket = reinterpret_cast<Ticket*>(GetUserDataLong(GetParentWnd(hWnd)));
	if (ticket)
	{
		WCHAR szRegister[32];
		ticket->txtRegister.GetText(szRegister);

		Passenger person = ticket->GetByRegister(szRegister);
		if (person.Register != TEXT(""))
		{
			ticket->txtFullName.SetText((person.FirstName + TEXT(" ") + person.LastName).c_str());

			if (person.Age > 12)
			{
				ticket->Adult.Check();
				ticket->Child.Check(false);
			}
			else
			{
				ticket->Adult.Check(false);
				ticket->Child.Check();
			}

			ticket->txtFullName.Disable();
			ticket->Adult.Disable();
			ticket->Child.Disable();
		}
		else
		{
			ticket->txtFullName.SetText(TEXT(" "));

			ticket->txtFullName.Enable();
			ticket->Adult.Enable();
			ticket->Child.Enable();
		}
		ticket->OnChangeSomething(hWnd);
	}
}

void Ticket::OnChangeSomething(HWND hWnd)
{
	Ticket* tckt = reinterpret_cast<Ticket*>(GetUserDataLong(GetParentWnd(hWnd)));
	if (tckt)
	{
		UINT Price = (UINT)(tckt->CalculateInsurance() + tckt->CalculatePrice());
		tckt->txtPayment.SetText((std::to_wstring(Price) + TEXT("₮")).c_str());
	}
}

LPVOID Ticket::OnEditDown(HWND hWnd, UINT mButton, WPARAM sState, int X, int Y)
{
	if (!::IsWindow(Keyboard.GetHandle()))
	{
		Keyboard.Assign(Client.GetInstance(), Client);
	}

	if (!Keyboard.IsVisible())
	{
		Keyboard.Show();
	}

	if (CKey::Target != hWnd)
	{
		Keyboard.SetTarget(hWnd);
	}

	return 0L;
}

LPVOID Ticket::OnPaint(HWND OwnerWnd, HDC hDC, PAINTSTRUCT pStruct)
{
	Ticket* ticket = reinterpret_cast<Ticket*>(GetUserDataLong(OwnerWnd));

	if (ticket)
	{
		RECT rect = ticket->GetClientRect();
		HPEN hpenOld = static_cast<HPEN>(::SelectObject(hDC, ::GetStockObject(DC_PEN)));
		HBRUSH hbrushOld = static_cast<HBRUSH>(::SelectObject(hDC, ::GetStockObject(NULL_BRUSH)));
		rect.top += 10;
		::SelectObject(hDC, ::CreatePen(PS_SOLID, 1, RGB_BLACK));
		::InflateRect(&rect, -1, -1);
		::Rectangle(hDC, rect.left, rect.top, rect.right, rect.bottom);
		::InflateRect(&rect, -5, -5);
		::Rectangle(hDC, rect.left, rect.top, rect.right, rect.bottom);

		::SetBkColor(hDC, RGB(240, 240, 240));
		::SetTextColor(hDC, RGB_BLACK);
		::SelectObject(hDC, cfg.Font(26));
		rect.top -= 40;
		rect.bottom = 50;
		::DrawText(hDC, std::wstring(TEXT(" Билет №: ") + ticket->ID + TEXT(" ")).c_str(), -1, &rect, DT_CENTER | DT_SINGLELINE | DT_VCENTER);
		::SelectObject(hDC, hpenOld);
		::SelectObject(hDC, hbrushOld);
	}
	return 0L;
}

double Ticket::CalculateInsurance()
{
	transdep::RawTariff* tariff = Transdep.RawTariffBy(
		Transdep.passage.dispatcher.direction_id,
		Transdep.passage.dispatcher.direction_start_stop_id,
		Transdep.times.stopID);
	bool IsAdult = Adult.IsChecked();
	if (checkInsurance.IsChecked())
	{
		switch (static_cast<CarSize>(Transdep.passage.dispatcher.car_type_id))
		{
		case csLarge: return (IsAdult) ? tariff->big_insurance : tariff->big_childinsurance;
		case csMid: return (IsAdult) ? tariff->mid_insurance : tariff->mid_childinsurance;
		case csSmall: return (IsAdult) ? tariff->lit_insurance : tariff->lit_childinsurance;
		}
		return (IsAdult) ? tariff->sit_insurance : tariff->sit_childinsurance;
	}
	else
		return 0;
}

double Ticket::CalculatePrice()
{
	transdep::RawTariff* tariff = Transdep.RawTariffBy(
		Transdep.passage.dispatcher.direction_id,
		Transdep.passage.dispatcher.direction_start_stop_id,
		Transdep.times.stopID);
	bool IsAdult = Adult.IsChecked();
	switch (static_cast<CarSize>(Transdep.passage.dispatcher.car_type_id))
	{
	case csLarge: return (IsAdult) ? tariff->big_price : tariff->big_childprice;
	case csMid: return (IsAdult) ? tariff->mid_price : tariff->mid_childprice;
	case csSmall: return (IsAdult) ? tariff->lit_price : tariff->lit_childprice;
	}
	return (IsAdult) ? tariff->sit_price : tariff->sit_childprice;
}

double Ticket::TotalPrice()
{
	return CalculateInsurance() + CalculatePrice();
}

void Ticket::GetPrices(RawTariff tariff, int *Price100, int *Price92, int *Price8, int *PriceInsurance)
{
	float price, insurance;
	UINT PricePercent = tariff.big_price_percent;
	bool IsAdult = Adult.IsChecked();

	price = (IsAdult) ? tariff.big_price : tariff.big_childprice;
	insurance = (IsAdult) ? tariff.big_insurance : tariff.big_childinsurance;
	switch (static_cast<CarSize>(Transdep.passage.dispatcher.car_type_id))
	{
	case csCab:
	{
		price = (IsAdult) ? tariff.sit_price : tariff.sit_childprice;
		insurance = (IsAdult) ? tariff.sit_insurance : tariff.sit_childinsurance;
		PricePercent = tariff.sit_price_percent;
	} break;
	case csMid:
	{
		price = (IsAdult) ? tariff.mid_price : tariff.mid_childprice;
		insurance = (IsAdult) ? tariff.mid_insurance : tariff.mid_childinsurance;
		PricePercent = tariff.mid_price_percent;
	} break;
	case csSmall:
	{
		price = (IsAdult) ? tariff.lit_price : tariff.lit_childprice;
		insurance = (IsAdult) ? tariff.lit_insurance : tariff.lit_childinsurance;
		PricePercent = tariff.lit_price_percent;
	} break;
	}

	*PriceInsurance = (int)insurance;
	*Price100 = (int)price;
	int percent = *Price100 / 100;
	*Price8 = percent * PricePercent;
	*Price92 = *Price100 - (*Price8);
}


LPVOID Ticket::OnCommand(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	if (HIWORD(wParam) == BN_CLICKED)
	{
		::EnumChildWindows(hWnd, CRadio::EnumSiblings, (LPARAM)(UINT)LOWORD(wParam));
	}

	if (HIWORD(wParam) == EN_CHANGE)
	{
		for (UINT i = 0; i < Transdep.Bus.Reserved(); i++)
		{
			if ((UINT)LOWORD(wParam) == Transdep.ticket[i].txtRegister)
			{
				/*
				if (Transdep.ticket[i].txtRegister.GetTextLength() == 10)
				{
				OnSendRegister(Transdep.ticket[i].txtRegister);
				}
				else
				{
				Transdep.ticket[i].txtFullName.SetText(TEXT(""));
				Transdep.ticket[i].txtFullName.Enable();
				Transdep.ticket[i].Adult.Enable();
				Transdep.ticket[i].Child.Enable();
				}
				*/
			}

		}
		if (transdep::Ticket::CheckCompleting())
		{
			if (!Client.footer.right.IsVisible())
			{
				Client.footer.right.AssignBitmap(Bmp::LoadGDI(cfg.DataPath(TEXT("footer\\next-arrow.png")).c_str()));
				Client.footer.right.SetColor(BRUSH_TERTIARY);
				Client.footer.right.ShowText(TEXT("ТӨЛБӨР ТӨЛӨХ"));
			}
		}
		else
			Client.footer.right.Hide();
	}

	return 0L;
}

OrderSeat::OrderSeat()
{
	Empty();
}

void OrderSeat::Empty()
{
	raw.id = 0;
	raw.zahialagch_id = 0;
	raw.zahialagch_branch_name = TEXT("");
	raw.zahialagch_name = TEXT("");
	raw.zahialagch_at = 0;
	raw.expired = 0;

	raw.ErrorMsg = TEXT("");
}

void OrderSeat::ParseXML(const pugi::xml_document *doc)
{
	if (!doc)
		return;

	pugi::xml_node element =
		doc->child(TEXT("DataTable"))
		.child(TEXT("diffgr:diffgram"))
		.child(TEXT("DocumentElement"))
		.first_child();

	raw.id = 1;
	RawData::SetInt(raw.zahialagch_id, element.child_value(TEXT("zahialagch_id")));
	raw.zahialagch_branch_name = element.child_value(TEXT("zahialagch_branch_name"));
	raw.zahialagch_name = element.child_value(TEXT("zahialagch_name"));
	RawData::SetTimeTransdep(raw.zahialagch_at, element.child_value(TEXT("zahialagch_at")));
	//	RawData::SetTimeTransdep(raw.expired, element.child_value(TEXT("expired")));

	raw.ErrorMsg = element.child_value(TEXT("ErrorMsg"));
}

TicketConfirm::TicketConfirm()
{
	Empty();
}

void TicketConfirm::Empty()
{
	raw.id = 0;
	raw.dispatcher_id = 0;
	raw.start_stop_id = 0;
	raw.start_stop_name = TEXT("");
	raw.end_stop_id = 0;
	raw.end_stop_name = TEXT("");
	raw.is_child = false;
	raw.ticket_num = 0;
	raw.register_number = TEXT("");
	raw.passenger_name = TEXT("");
	raw.price92 = 0;
	raw.price8 = 0;
	raw.total = 0;
	raw.canceled = false;
	raw.printed = false;
	raw.seat_no = 0;
	raw.withchild_register = TEXT("");
	raw.withchild_name = TEXT("");
	raw.created_by = 0;
	raw.created_firstname = TEXT("");
	raw.created_at = 0;
	raw.ticket_type_id = 0;
	raw.out_date = 0;
	raw.branch_id = 0;
	raw.number = TEXT("");
	raw.insurance = 0;
	raw.company_id = 0;
	raw.priceOS = 0;
	raw.priceBankFees = 0;
	raw.billId = TEXT("");
	raw.edou_duty = 0;
	raw.settlement_date = 0;
	raw.billDate = 0;

	raw.ErrorMsg = TEXT("");
}

void TicketConfirm::ParseXML(const pugi::xml_document *doc)
{
	if (!doc)
		return;

	pugi::xml_node element =
		doc->child(TEXT("DataTable"))
		.child(TEXT("diffgr:diffgram"))
		.child(TEXT("DocumentElement"))
		.first_child();

	raw.ErrorMsg = element.child_value(TEXT("ErrorMsg"));

	return;
	RawData::SetInt(raw.id, element.child_value(TEXT("id")));
	RawData::SetInt(raw.dispatcher_id, element.child_value(TEXT("dispatcher_id")));
	RawData::SetInt(raw.start_stop_id, element.child_value(TEXT("start_stop_id")));
	raw.start_stop_name = element.child_value(TEXT("start_stop_name"));
	RawData::SetInt(raw.end_stop_id, element.child_value(TEXT("end_stop_id")));
	raw.end_stop_name = element.child_value(TEXT("end_stop_name"));
	RawData::SetBool(raw.is_child, element.child_value(TEXT("is_child")));
	RawData::SetInt(raw.ticket_num, element.child_value(TEXT("ticket_num")));
	raw.register_number = element.child_value(TEXT("register"));
	raw.passenger_name = element.child_value(TEXT("passenger_name"));
	RawData::SetInt(raw.price92, element.child_value(TEXT("price92")));
	RawData::SetInt(raw.price8, element.child_value(TEXT("price8")));
	RawData::SetInt(raw.total, element.child_value(TEXT("total")));
	RawData::SetBool(raw.canceled, element.child_value(TEXT("canceled")));
	RawData::SetBool(raw.printed, element.child_value(TEXT("printed")));
	RawData::SetInt(raw.seat_no, element.child_value(TEXT("seat_no")));
	raw.withchild_register = element.child_value(TEXT("withchild_register"));
	raw.withchild_name = element.child_value(TEXT("withchild_name"));
	RawData::SetInt(raw.created_by, element.child_value(TEXT("created_by")));
	raw.created_firstname = element.child_value(TEXT("created_firstname"));
	RawData::SetTimeTransdep(raw.created_at, element.child_value(TEXT("created_at")));
	RawData::SetInt(raw.ticket_type_id, element.child_value(TEXT("ticket_type_id")));
	RawData::SetTimeTransdep(raw.out_date, element.child_value(TEXT("out_date")));
	RawData::SetInt(raw.branch_id, element.child_value(TEXT("branch_id")));
	raw.number = element.child_value(TEXT("number"));
	RawData::SetInt(raw.company_id, element.child_value(TEXT("company_id")));
	RawData::SetInt(raw.priceOS, element.child_value(TEXT("priceOS")));
	RawData::SetInt(raw.priceBankFees, element.child_value(TEXT("priceBankFees")));
	raw.billId = element.child_value(TEXT("billId"));
	RawData::SetInt(raw.edou_duty, element.child_value(TEXT("edou_duty")));
	RawData::SetTimeTransdep(raw.settlement_date, element.child_value(TEXT("settlement_date")));
	RawData::SetTimeTransdep(raw.billDate, element.child_value(TEXT("billDate")));
}

bool Ticket::SetOrderSeat()
{
	time_t    rawtime;
	struct tm timeinfo;
	char      buffer[80];
	int       m = 0, s = 0;

	::time(&rawtime);

	//Client.header.timer.GetRemaining(&m, &s);
	m += 32;
	rawtime = rawtime + (m * 60) + s;
	::localtime_s(&timeinfo, &rawtime);
	::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:00", &timeinfo);
	std::string expired(buffer);

	std::wstring hdr = TEXT("Content-Type: application/x-www-form-urlencoded; charset=utf-8\r\n");
	std::string data = "machine_id=" + std::to_string(cfg.GetID());
	data += "&action=2";
	data += "&dispatcher_id=" + std::to_string(Transdep.passage.dispatcher.id);
	data += "&seat_no=" + std::to_string(passenger.Seat);
	data += "&start_sequence=" + string::utf8encode(Transdep.passage.StartSequence);
	data += "&end_sequence=" + string::utf8encode(Transdep.passage.EndSequence);
	data += "&zahialagch_id=875";
	data += "&expired=" + expired;
	data += "&addminut=2";


	std::wstring response = Server::Post(TEXT(""), data.c_str());
/*
	std::wstring response = CInternet::PostData(
		METAKIOSK_SERVER,
		TEXT("service/set_order_seat"),
		hdr.c_str(),
		hdr.size(),
		(LPVOID)data.c_str(),
		data.length());
*/
	pugi::xml_document set_Order_Seat;
	XML::LoadFromBuffer(&set_Order_Seat, response);

	ordering.Empty();
	if (set_Order_Seat)
		ordering.ParseXML(&set_Order_Seat);

	if (ordering.raw.ErrorMsg == TEXT(""))
		return true;
	else
		return false;
}

void Ticket::SetOrderSeats(LPVOID arg)
{
	for (UINT i = 0; i < Transdep.Bus.Reserved(); i++)
	{
		if (!Transdep.ticket[i].SetOrderSeat())
		{
			std::wstring hdr = TEXT("Content-Type: application/x-www-form-urlencoded; charset=utf-8\r\n");
			std::string  data_nuller;
			for (UINT j = 0; j < i; j++)
			{
				data_nuller = "machine_id=" + std::to_string(cfg.GetID());
				data_nuller += "&action=2";
				data_nuller += "&dispatcher_id=" + std::to_string(Transdep.passage.dispatcher.id);
				data_nuller += "&seat_no=" + std::to_string(Transdep.ticket[j].passenger.Seat);

				Server::Post(TEXT("service/set_cancel_seat"), data_nuller.c_str());
				//CInternet::PostData(METAKIOSK_SERVER, TEXT("service/set_cancel_seat"), hdr.c_str(), hdr.size(), (LPVOID)data_nuller.c_str(), data_nuller.length());
			}

			Client.loading.Hide();
			Client.message.SetText((TEXT("Уучлаарай. Захиалга хийгдэх боломжгүй. \n[#") + std::to_wstring(Transdep.ticket[i].passenger.Seat) + TEXT(" дугаартай суудал] ") + Transdep.ticket[i].ordering.raw.ErrorMsg).c_str());
			Client.message.Redraw();
			Client.message.Show();
			Client.footer.left.ShowText(TEXT("СУУДАЛ СОЛИХ"));

			return;
		}
	}
	Client.State(ST_TRANSDEP_PAYDATABANK);
}

bool Ticket::CheckCompleting()
{
	for (UINT i = 0; i < Transdep.Bus.Reserved(); i++)
	{
		if (Transdep.ticket[i].txtRegister.GetTextLength() < 3 || Transdep.ticket[i].txtFullName.GetTextLength() < 3)
			return false;
	}
	return true;
}

void Ticket::Print(HDC hDC, UINT hDCwidth, UINT fontSize)
{
	Point p;
	p.X = 30;
	p.Y = 0;

	std::wstring logoname;
	if (hDCwidth < 400)
		logoname = TEXT("transdep/slogo.jpg");
	else
		logoname = TEXT("transdep/hlogo.jpg");

	HBITMAP logo = Bmp::LoadGDI(cfg.DataPath(logoname).c_str());
	HDC dcImage1;
	dcImage1 = ::CreateCompatibleDC(hDC);
	::SelectObject(dcImage1, logo);
	::StretchBlt(hDC, 0, 0, hDCwidth, hDCwidth * Bmp::GetSize(logo).cy / Bmp::GetSize(logo).cx, dcImage1, 0, 0, Bmp::GetSize(logo).cx, Bmp::GetSize(logo).cy, SRCCOPY);
	::SetBkMode(hDC, TRANSPARENT);
	p.Y += hDCwidth * Bmp::GetSize(logo).cy / Bmp::GetSize(logo).cx + hDCwidth / 100;
	::SelectObject(hDC, cfg.Font(fontSize, 0, 0, 0, BOLD_FONTTYPE, TRUE));
	std::wstring TicketID = TEXT("№: ") + ID;
	::TextOut(hDC, hDCwidth / 4, p.Y, TicketID.c_str(), TicketID.length());

	p.Y += fontSize;
	::SelectObject(hDC, CreatePen(0, hDCwidth / 100, RGB(0, 0, 0)));
	::MoveToEx(hDC, 0, p.Y, NULL);
	::LineTo(hDC, hDCwidth, p.Y);

	::SelectObject(hDC, cfg.Font(fontSize, 0, 0, 0, BOLD_FONTTYPE, TRUE));
	p.Y += fontSize / 2;
	::TextOut(hDC, p.X + 50, p.Y, TEXT("Зорчигч"), std::wstring(TEXT("Зорчигч")).length());

	::SelectObject(hDC, cfg.Font(fontSize, 0, 0, 0, BOLD_FONTTYPE));
	p.Y += fontSize;
	std::wstring Name = TEXT("Нэр: ") + passenger.FullName;
	::TextOut(hDC, p.X, p.Y, Name.c_str(), Name.length());

	p.Y += fontSize;
	std::wstring sRegister = TEXT("Регистр: ") + passenger.Register;
	::TextOut(hDC, p.X, p.Y, sRegister.c_str(), sRegister.length());

	p.Y += fontSize;
	std::wstring Age = TEXT("Насны ангилал: ") + std::wstring(passenger.Age > 17 ? TEXT("Том хүн ") : TEXT("Хүүхэд"));
	TextOut(hDC, p.X, p.Y, Age.c_str(), Age.length());

	::SelectObject(hDC, cfg.Font(fontSize, 0, 0, 0, BOLD_FONTTYPE, TRUE));
	p.Y += fontSize;
	::TextOut(hDC, p.X + 50, p.Y, TEXT("Тээвэрлэгч"), std::wstring(TEXT("Тээвэрлэгч")).length());

	::SelectObject(hDC, cfg.Font(fontSize, 0, 0, 0, BOLD_FONTTYPE));

	p.Y += fontSize;
	::TextOut(hDC, p.X, p.Y, Transdep.passage.Company.c_str(), Transdep.passage.Company.length());

	p.Y += fontSize;
	std::wstring CarNo = TEXT("Улсын дугаар: ") + Transdep.passage.Register;
	::TextOut(hDC, p.X, p.Y, CarNo.c_str(), CarNo.length());

	::SelectObject(hDC, cfg.Font(fontSize, 0, 0, 0, BOLD_FONTTYPE, TRUE));
	p.Y += fontSize;
	::TextOut(hDC, p.X + 50, p.Y, TEXT("Аялал"), std::wstring(TEXT("Аялал")).length());

	::SelectObject(hDC, cfg.Font(fontSize, 0, 0, 0, BOLD_FONTTYPE));
	p.Y += fontSize;
	std::wstring TravelNo = TEXT("Аялалын дугаар: ") + std::to_wstring(Transdep.passage.dispatcher.direction_id);
	::TextOut(hDC, p.X, p.Y, TravelNo.c_str(), TravelNo.length());

	p.Y += fontSize;
	std::wstring SeatNo = TEXT("Суудлын дугаар: ") + std::to_wstring(passenger.Seat);
	::TextOut(hDC, p.X, p.Y, SeatNo.c_str(), SeatNo.length());

	p.Y += fontSize;
	std::wstring Marshrut = TEXT("Маршрут: ") + Transdep.passage.dispatcher.direction_name;
	::TextOut(hDC, p.X, p.Y, Marshrut.c_str(), Marshrut.length());

	p.Y += fontSize;
	std::wstring GoDate = TEXT("Хөдлөх цаг: ") + Transdep.passage.LeaveDate + TEXT(" ") + Transdep.passage.LeaveTime;
	::TextOut(hDC, p.X, p.Y, GoDate.c_str(), GoDate.length());

	p.Y += fontSize;
	std::wstring StartStop = TEXT("Хөдлөх зогсоол: ") + Transdep.passage.dispatcher.direction_start_stop_name;
	::TextOut(hDC, p.X, p.Y, StartStop.c_str(), StartStop.length());

	p.Y += fontSize;
	std::wstring EndStop = TEXT("Буух зогсоол: ") + Transdep.passage.EndAimag + TEXT(" ") + Transdep.passage.EndStop;
	::TextOut(hDC, p.X, p.Y, EndStop.c_str(), EndStop.length());

	p.Y += fontSize;
	std::wstring TicketPay = TEXT("Тийзний үнэ: ") + std::to_wstring((UINT)CalculatePrice()) + TEXT("₮");
	::TextOut(hDC, p.X, p.Y, TicketPay.c_str(), TicketPay.length());

	p.Y += fontSize;
	std::wstring Daatgal = TEXT("Амь даатгал: ");
	if ((UINT)CalculateInsurance() == 0)
		Daatgal += TEXT("даатгуулаагүй");
	else
		Daatgal += std::to_wstring((UINT)CalculateInsurance()) + TEXT("₮");
	::TextOut(hDC, p.X, p.Y, Daatgal.c_str(), Daatgal.length());

	p.Y += fontSize;
	std::wstring Payment = TEXT("Нийт төлбөр: ") + std::to_wstring((UINT)(CalculateInsurance() + CalculatePrice())) + TEXT("₮");
	::TextOut(hDC, p.X, p.Y, Payment.c_str(), Payment.length());

	p.Y += fontSize + 10;
	Barcode128 barcode;
	barcode.Encode128A(string::utf8encode(ID).c_str());
	barcode.DrawBarcode(hDC, hDCwidth / 20, p.Y, p.Y + 50, p.Y + 80, RGB(0, 0, 0), RGB(255, 255, 255), hDCwidth / 300);

	p.Y += fontSize + 50;
	::MoveToEx(hDC, 0, p.Y, NULL);
	::LineTo(hDC, hDCwidth, p.Y);

	::SelectObject(hDC, cfg.Font(fontSize - 3, 0, 0, 0, BOLD_FONTTYPE, TRUE));
	if (passenger.Lottery != TEXT(""))
	{
		p.Y += fontSize - 3;
		std::wstring Lottery = TEXT("Сугалаа: ");
		Lottery += passenger.Lottery;
		::TextOut(hDC, p.X, p.Y, Lottery.c_str(), Lottery.length());
	}

	if (passenger.Amount != TEXT(""))
	{
		p.Y += fontSize - 3;
		std::wstring Amount = TEXT("Бүртгүүлэх үнэ: ");
		Amount += passenger.Amount + TEXT("₮");
		::TextOut(hDC, p.X, p.Y, Amount.c_str(), Amount.length());
	}

	if (passenger.TTD != TEXT(""))
	{
		p.Y += fontSize - 3;
		std::wstring TTD = TEXT("ТТД: ");
		TTD += passenger.TTD;
		::TextOut(hDC, p.X, p.Y, TTD.c_str(), TTD.length());
	}

	if (passenger.DDTD != TEXT(""))
	{
		p.Y += fontSize - 3;
		std::wstring DDTD = TEXT("ДДТД: ");
		DDTD += passenger.DDTD;
		::TextOut(hDC, p.X, p.Y, DDTD.c_str(), DDTD.length());
	}

//	::MessageBoxW(NULL, passenger.QR.c_str(), NULL, 0);
	if (passenger.QR != TEXT(""))
	{
		p.Y += fontSize;
		CQREncoder QR;
		QR.InitializeData(string::utf8encode(passenger.QR).c_str());
		QR.PaintData(::WindowFromDC(hDC), hDC, coord::to_rect(hDCwidth / 2 - hDCwidth / 6, p.Y, hDCwidth / 3, hDCwidth / 3));
	}

	::SelectObject(hDC, cfg.Font(fontSize, 0, 0, 0, BOLD_FONTTYPE, TRUE));
	p.Y += hDCwidth / 3 + 5;
	::TextOut(hDC, p.X, p.Y, TEXT("Авто тээврийн үндэсний төв ТӨҮГ\n\n"), std::wstring(TEXT("Авто тээврийн үндэсний төв ТӨҮГ\n\n")).length());
}