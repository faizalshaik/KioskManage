#include "../clientwnd.h"
#include "control.h"
#include "timetable.h"

using namespace transdep;

bool RawDispatcher::operator < (const RawDispatcher dispatcher) const
{
	const auto diff2 = std::difftime(leave_date, dispatcher.leave_date);

	if (diff2 >= 0.0)
		return false;
	else
		return true;
}

Dispatchers::Dispatchers()
{
	raw = new std::vector<RawDispatcher>();
}

Dispatchers::~Dispatchers()
{
	if (raw != NULL)
		delete raw;
}

bool Dispatchers::IsEmpty()
{
	return raw->empty();
}

void Dispatchers::Reset()
{
	raw->clear();
}

void Dispatchers::Sort()
{
	std::sort(raw->begin(), raw->end());
}

void Dispatchers::ParseXML(const pugi::xml_document* doc)
{
	if (!doc)
		return;

	pugi::xml_node element =
		doc->child(TEXT("DataTable"))
		.child(TEXT("diffgr:diffgram"))
		.child(TEXT("DocumentElement"))
		.first_child();
	while (element != NULL) {
		UINT car_type_id;
		RawData::SetInt(car_type_id, element.child_value(TEXT("car_type_id")));

		UINT direction_start_stop_id;
		UINT direction_end_stop_id;
		std::wstring direction_start_stop_name;
		std::wstring direction_end_stop_name;

		RawData::SetInt(direction_start_stop_id, element.child_value(TEXT("direction_start_stop_id")));
		RawData::SetInt(direction_end_stop_id, element.child_value(TEXT("direction_end_stop_id")));
		direction_start_stop_name = element.child_value(TEXT("direction_start_stop_name"));
		direction_end_stop_name = element.child_value(TEXT("direction_end_stop_name"));

		if (car_type_id != 4) // Суудлын автомашиныг алгасая!
		{
			RawDispatcher dispatcher;
			BOOL valid_dispatcher = TRUE;

			if (Transdep.IsStartStop(direction_start_stop_id))
			{
				dispatcher.direction_start_stop_id = direction_start_stop_id;
				dispatcher.direction_end_stop_id = direction_end_stop_id;
				dispatcher.direction_start_stop_name = direction_start_stop_name;
				dispatcher.direction_end_stop_name = direction_end_stop_name;
			}
			else if (Transdep.IsStartStop(direction_end_stop_id))
			{
				dispatcher.direction_start_stop_id = direction_end_stop_id;
				dispatcher.direction_end_stop_id = direction_start_stop_id;
				dispatcher.direction_start_stop_name = direction_end_stop_name;
				dispatcher.direction_end_stop_name = direction_start_stop_name;
			}
			else
			{
				valid_dispatcher = FALSE;
			}

			RawData::SetInt(dispatcher.id, element.child_value(TEXT("id")));

			RawData::SetInt(dispatcher.distribution_id, element.child_value(TEXT("distribution_id")));

			RawData::SetInt(dispatcher.direction_id, element.child_value(TEXT("direction_id")));
			dispatcher.direction_name = element.child_value(TEXT("direction_name"));

			RawData::SetInt(dispatcher.direction_start_stop_id, element.child_value(TEXT("direction_start_stop_id")));
			RawData::SetInt(dispatcher.direction_end_stop_id, element.child_value(TEXT("direction_end_stop_id")));

			RawData::SetInt(dispatcher.company_id, element.child_value(TEXT("company_id")));
			dispatcher.company_name = element.child_value(TEXT("company_name"));

			RawData::SetInt(dispatcher.car_id, element.child_value(TEXT("car_id")));
			dispatcher.car_number = element.child_value(TEXT("car_number"));
			dispatcher.car_type_id = car_type_id;
			dispatcher.car_type_name = element.child_value(TEXT("car_type_name"));

			RawData::SetInt(dispatcher.driver_id, element.child_value(TEXT("driver_id")));

			RawData::SetTimeTransdep(dispatcher.leave_date, element.child_value(TEXT("leave_date")));

			RawData::SetBool(dispatcher.is_rtrn, element.child_value(TEXT("is_rtrn")));

			RawData::SetBool(dispatcher.from_country, element.child_value(TEXT("from_country")));

			RawData::SetInt(dispatcher.link_id, element.child_value(TEXT("link_id")));
			RawData::SetBool(dispatcher.is_closed, element.child_value(TEXT("is_closed")));
			RawData::SetBool(dispatcher.is_actived, element.child_value(TEXT("is_actived")));
			RawData::SetInt(dispatcher.created_by, element.child_value(TEXT("created_by")));
			RawData::SetInt(dispatcher.created_branch_id, element.child_value(TEXT("created_branch_id")));
			dispatcher.created_firstname = element.child_value(TEXT("created_firstname"));
			RawData::SetTimeTransdep(dispatcher.created_at, element.child_value(TEXT("created_at")));

			RawData::SetBool(dispatcher.company_is_country, element.child_value(TEXT("company_is_country")));

			RawData::SetBool(dispatcher.is_added, element.child_value(TEXT("is_added")));

			RawData::SetInt(dispatcher.sitcount, element.child_value(TEXT("sitcount")));

			dispatcher.driver_name = element.child_value(TEXT("driver_name"));

			dispatcher.model_name = element.child_value(TEXT("model_name"));

			dispatcher.register_number = element.child_value(TEXT("register_number"));

			RawData::SetInt(dispatcher.aimag_id, element.child_value(TEXT("aimag_id")));

			dispatcher.phone = element.child_value(TEXT("phone"));

			std::wstring emp = element.child_value(TEXT("empty_seat_count"));

			if (valid_dispatcher)
				raw->push_back(dispatcher);
		}

		element = element.next_sibling();
	}
}

DateDispatcher::DateDispatcher()
	: CHoverBitmapWindow()
{

	SetClass(TEXT("KioskMan::Transdep::TimeTable::DateDispatcher"));
	onPaint = OnPaint;
	onMouseDown = OnMouseDown;
}

DatePagination::DatePagination()
	: CPanel()
{
	SetClass(TEXT("KioskMan::Transdep::TimeTable::Pagination"));
	nPageNumber = 0;
	onCreate = OnCreate;
}

LPVOID DatePagination::OnCreate(HWND hWnd, LPARAM lParam)
{
	DatePagination *me = reinterpret_cast<DatePagination*>(GetUserDataLong(hWnd));
	if (me)
	{
		me->InitArrows();
		me->SetBkBrush(BRUSH_PAGINATION);
	}
	return 0L;
}

LPVOID DatePagination::OnPaintRight(HWND OwnerWnd, HDC hDC, PAINTSTRUCT ps)
{
	::SelectObject(hDC, ::CreatePen(PS_SOLID, 7, RGB_BTN_FRAME));
	::MoveToEx(hDC, 0, 0, 0);
	::LineTo(hDC, 60, 40);
	::MoveToEx(hDC, 0, 0, 0);
	::LineTo(hDC, 0, 80);
	::MoveToEx(hDC, 60, 40, 0);
	::LineTo(hDC, 0, 80);

	return 0L;
}

LPVOID DatePagination::OnPaintLeft(HWND OwnerWnd, HDC hDC, PAINTSTRUCT ps)
{
	::SelectObject(hDC, ::CreatePen(PS_SOLID, 7, RGB_BTN_FRAME));
	::MoveToEx(hDC, 60, 0, 0);
	::LineTo(hDC, 0, 40);
	::MoveToEx(hDC, 60, 0, 0);
	::LineTo(hDC, 60, 80);
	::MoveToEx(hDC, 0, 40, 0);
	::LineTo(hDC, 60, 80);

	return 0L;
}

bool DatePagination::InitArrows()
{
	left.SetClass(TEXT("KioskMan::TimeTable::Arrow"));
	right.SetClass(TEXT("KioskMan::TimeTable::Arrow"));

	if (!left.Register(CS_DBLCLKS | CS_OWNDC | CS_SAVEBITS, GetInstance(), NULL, ::LoadCursor(NULL, IDC_HAND)))
		return FALSE;

	left.Create(TEXT("left"), WS_CHILD, coord::to_rect(15, 10, 60, 80), *this, NULL, GetInstance());
	left.SetRegion(Rgn::Triangle(coord::point(0, 40), coord::point(60, 0), coord::point(60, 80)));
	left.SetBkBrush(BRUSH_TERTIARY);
	left.onMouseDown = OnClickLeft;
	left.onPaint = OnPaintLeft;

	right.Create(TEXT("right"), WS_CHILD, coord::to_rect(coord::width(GetRect()) - 90, 10, 60, 80), *this, NULL, GetInstance());
	right.SetRegion(Rgn::Triangle(coord::point(0, 0), coord::point(60, 40), coord::point(0, 80)));
	right.onMouseDown = OnClickRight;
	right.onPaint = OnPaintRight;

	status.Create(GetInstance(), *this, TEXT("Чиглэл хуваарьгүй байна"), coord::to_rect(110, 10, coord::width(GetRect()) - 240, 80), cfg.Font(36, 0, 0, 0, FW_BOLD),
		WS_CHILD | WS_VISIBLE | WS_GROUP | SS_CENTER | SS_NOTIFY | SS_CENTERIMAGE);

	status.Show();

	return true;
}

LPVOID DatePagination::OnClickLeft(HWND hWnd, UINT mButton, WPARAM sState, int X, int Y)
{
	if (Transdep.times.pagination.nPageNumber > 0)
		Transdep.times.pagination.nPageNumber--;

	Transdep.times.DatePagination(Transdep.times.pagination.nPageNumber);

	return 0L;
}

LPVOID DatePagination::OnClickRight(HWND hWnd, UINT mButton, WPARAM sState, int X, int Y)
{
	Transdep.times.pagination.nPageNumber++;
	Transdep.times.DatePagination(Transdep.times.pagination.nPageNumber);

	return 0L;
}

TimeTable::TimeTable()
	: CPanel()
{
	SetClass(TEXT("KioskMan::Transdep::TimeTable"));

	ddates = NULL;
}

TimeTable::~TimeTable()
{
	SafeDelete(ddates);
}

void TimeTable::CreateChilds()
{
	RECT rc = GetClientRect();
	rc.left += 20;
	rc.bottom = rc.top + 115;
	rc.right -= 25;
	rc.top += 10;
	pagination.Initialize(GetInstance(), *this, rc);
	pagination.Show();

	SafeDelete(ddates);

	ddates = new DateDispatcher[MAX_DATES_IN_PAGE];
	for (int i = 0; i < MAX_DATES_IN_PAGE; i++)
	{
		ddates[i].Instant(GetInstance(), *this, coord::to_rect(68, 130, 915, 101), NULL, NULL, FALSE);
		ddates[i].SetRegion(Rgn::Rounded(coord::size(915, 101)));
		if (i > 0)
			ddates[i].FitAfterSibling();
	}
}

void TimeTable::HideDates()
{
	for (int i = 0; i < MAX_DATES_IN_PAGE; i++)
	{
		ddates[i].Hide();
	}
}

void TimeTable::Do(UINT SelectedStop)
{
	EndStop *stop = Transdep.EndStopByID(SelectedStop);
	if (stop)
	{
		stopID = SelectedStop;
		Stop GetOff = *Transdep.StopByID(Transdep.times.stopID);
		if (Aimag::IsMglAimag(GetOff.AimagID))
		{
			Transdep.passage.EndAimag = Transdep.AimagByID(GetOff.AimagID)->getClean() + TEXT(" аймаг");
		}
		else
		{
			if (GetOff.AimagID == 25)
			{
				Transdep.passage.EndAimag = TEXT("ОХУ");
			}
			else
			{
				Transdep.passage.EndAimag = TEXT("БНХАУ");
			}
		}

		Transdep.passage.EndStop = GetOff.getClean();

		Client.State(ST_TRANSDEP_TIMETABLE);
	}
}

void TimeTable::GetDispatchers(LPVOID arg)
{
	if (Transdep.times)
	{
		EndStop *stop = Transdep.EndStopByID(Transdep.times.stopID);
		if (stop)
		{
			Transdep.times.dispatcher.Reset();
			pugi::xml_document get_Date_by_Dispatchers;
			std::wstring hdr = TEXT("Content-Type: application/x-www-form-urlencoded; charset=utf-8\r\n");
			for (auto tariff = stop->tariff_ids.begin(); tariff != stop->tariff_ids.end(); ++tariff)
			{
				if (CInternet::CheckConnection())
				{
					std::string data = "machine_id=" + std::to_string(cfg.GetID());
					data += "&id_direction=" + std::to_string(*tariff);
					data += "&start=" + string::utf8encode(Transdep.times.GetByDays());
					data += "&end=" + string::utf8encode(Transdep.times.GetByDays(31));

//					XML::LoadFromFile(&get_Date_by_Dispatchers, cfg.DataPath(TEXT("transdep\\")) + TEXT("tariff - 2017 - 05.xml"));

					XML::LoadFromBuffer(&get_Date_by_Dispatchers,
						Server::Post(TEXT("service/get_Date_by_Dispatchers"), data.c_str()));

/*
					XML::LoadFromBuffer(&get_Date_by_Dispatchers,
						CInternet::PostData(
						METAKIOSK_SERVER,					
						TEXT("service/get_Date_by_Dispatchers"),
						hdr.c_str(),
						hdr.size(),
						(LPVOID)data.c_str(),
						data.length()));
*/

					if (get_Date_by_Dispatchers)
						Transdep.times.dispatcher.ParseXML(&get_Date_by_Dispatchers);
				}
			}
			Transdep.times.dispatcher.Sort();
			Transdep.times.DatePagination(0);
			Transdep.times.Show();

			Client.loading.Hide();
			Client.footer.left.ShowText(TEXT("БУЦАХ"));
		}
	}
}

void TimeTable::DatePagination(UINT PageNumber)
{
	if (!dispatcher.IsEmpty())
	{
		HideDates();

		pagination.nPageNumber = PageNumber;

		if (pagination.nPageNumber == 0)
			pagination.left.Hide();
		else
			pagination.left.Show();

		if ((MAX_DATES_IN_PAGE * (pagination.nPageNumber + 1)) >= dispatcher.raw->size())
			pagination.right.Hide();
		else
			pagination.right.Show();

		int index = 0;
		for (auto disp = (dispatcher.raw->begin() + MAX_DATES_IN_PAGE * pagination.nPageNumber); disp != dispatcher.raw->end(); ++disp)
		{
			ddates[index].inf = *disp;
			ddates[index].Show();
			ddates[index].Redraw();

			if (index < (MAX_DATES_IN_PAGE - 1))
				index++;
			else
				break;
		}

		pagination.status.SetText(std::wstring(string::formatdatetime(&ddates[0].inf.leave_date, "%Y-%m-%d %H:%M") + TEXT(" -> ") + string::formatdatetime(&ddates[--index].inf.leave_date, "%Y-%m-%d %H:%M")).c_str());
	}
}

LPVOID DateDispatcher::OnMouseDown(HWND hWnd, UINT mButton, WPARAM sState, int X, int Y)
{
	DateDispatcher* dispatcher = reinterpret_cast<DateDispatcher*>(GetUserDataLong(hWnd));

	if (dispatcher)
	{
		transdep::RawTariff* tariff = Transdep.RawTariffBy(
			dispatcher->inf.direction_id,
			dispatcher->inf.direction_start_stop_id,
			Transdep.times.stopID);
		if (tariff)
		{
			Transdep.passage.dispatcher = dispatcher->inf;
			Transdep.passage.StartStop = Transdep.StopByID(dispatcher->inf.direction_start_stop_id)->getClean();

			if (NULL == Transdep.StopByID(dispatcher->inf.direction_end_stop_id))
			{
				// TODO: Автотээврийн серверээс get_Date_by_Dispatchers сервисээс авсан тарифийн
				// зарим буудлын дугаарууд get_Tariff_to_Country сервист ирсэн тариф дах буудлын дугаараас зөрөх
				// алдаа гарснаас болж программ crash үүсэж байв. Жишээ нь: Улаанбаатар -> Эрээн чиглэлд
				Client.JumpStart();
				return 0L;
			}

			Stop stop = *Transdep.StopByID(dispatcher->inf.direction_end_stop_id);

			if (Aimag::IsMglAimag(stop.AimagID))
			{
				Transdep.passage.DirEndAimag = Transdep.AimagByID(stop.AimagID)->getClean() + TEXT(" аймаг");
			}
			else
			{
				if (stop.AimagID == 25)
				{
					Transdep.passage.DirEndAimag = TEXT("ОХУ");
				}
				else
				{
					Transdep.passage.DirEndAimag = TEXT("БНХАУ");
				}
			}

			Transdep.passage.DirEndStop = stop.getClean();

			Transdep.passage.StartSequence = std::to_wstring(tariff->start_stop_sequence);
			Transdep.passage.EndSequence = std::to_wstring(tariff->end_stop_sequence);

			Transdep.passage.LeaveDate = string::formatdatetime(&dispatcher->inf.leave_date, "%Y-%m-%d");
			Transdep.passage.LeaveTime = string::formatdatetime(&dispatcher->inf.leave_date, "%H:%M");
			Transdep.passage.Company = dispatcher->inf.company_name;
			Transdep.passage.Car = dispatcher->inf.model_name;
			Transdep.passage.Register = dispatcher->inf.car_number;
			Transdep.passage.Type = dispatcher->inf.car_type_name;
			Transdep.passage.Seats = std::to_wstring(dispatcher->inf.sitcount);

			Transdep.passage.Insurance = tariff->end_i_com_name;
			Transdep.Bus.Set(dispatcher->inf.id, dispatcher->inf.car_type_id, dispatcher->inf.sitcount);

			Client.State(ST_TRANSDEP_SEATME);
		}
		else
		{
			Client.JumpStart(); // Диспечерийн мэдээлэл тарифын мэдээлэлтэй тохирохгүй байгаа учир шууд 
			// үргэлжлүүлэх боломжгүй
		}
	}

	return 0L;
}

LPVOID DateDispatcher::OnPaint(HWND OwnerWnd, HDC hDC, PAINTSTRUCT pStruct)
{
	DateDispatcher     *dispatcher = reinterpret_cast<DateDispatcher*>(GetUserDataLong(OwnerWnd));
	ULONG_PTR           gdiplusToken;
	GdiplusStartupInput gdiplusStartupInput;
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

	Gdiplus::Graphics graphics(hDC);
	// Drawing Rectangle 
	SolidBrush whiteBrush(Color(236, 236, 236));
	SolidBrush blueBrush(Color(17, 89, 139));
	graphics.FillRectangle(&blueBrush, 0, 0, 915, 35);
	SolidBrush blueBrush1(Color(191, 192, 197));
	graphics.FillRectangle(&blueBrush1, 0, 40, 915, 10);
	SolidBrush blueBrush2(Color(233, 233, 233));
	graphics.FillRectangle(&blueBrush2, 0, 40, 915, 150);
	SolidBrush blueBrush3(Color(187, 187, 187));
	graphics.FillRectangle(&blueBrush3, 130, 45, 1, 50);
	graphics.FillRectangle(&blueBrush3, 260, 45, 1, 50);
	graphics.FillRectangle(&blueBrush3, 390, 45, 1, 50);
	graphics.FillRectangle(&blueBrush3, 520, 45, 1, 50);
	graphics.FillRectangle(&blueBrush3, 650, 45, 1, 50);
	graphics.FillRectangle(&blueBrush3, 780, 45, 1, 50);
	SolidBrush blackBrush1(Color(28, 28, 28));
	SolidBrush blackBrush2(Color(32, 32, 32));
	SolidBrush blackBrush3(Color(25, 25, 25));
	// text info
	Font fontAlt(&FontFamily(DEF_FONT_FACE), 10);
	StringFormat format;
	format.SetAlignment(StringAlignmentFar);
	graphics.DrawString(TEXT("ОН САР"),		 -1, &fontAlt, Gdiplus::RectF(0, 13, 100, 30), &format, &whiteBrush);
	graphics.DrawString(TEXT("ЦАГ"),	     -1, &fontAlt, Gdiplus::RectF(0, 13, 230, 30), &format, &whiteBrush);
	graphics.DrawString(TEXT("ХААНААС"),     -1, &fontAlt, Gdiplus::RectF(0, 13, 360, 30), &format, &whiteBrush);
	graphics.DrawString(TEXT("ХААШАА"),      -1, &fontAlt, Gdiplus::RectF(0, 13, 490, 30), &format, &whiteBrush);
	graphics.DrawString(TEXT("ЗОГСООЛ"),     -1, &fontAlt, Gdiplus::RectF(0, 13, 620, 30), &format, &whiteBrush);
	graphics.DrawString(TEXT("ТӨРӨЛ"),       -1, &fontAlt, Gdiplus::RectF(0, 13, 750, 30), &format, &whiteBrush);
	graphics.DrawString(TEXT("СУУДЛЫН ТОО"), -1, &fontAlt, Gdiplus::RectF(0, 13, 880, 30), &format, &whiteBrush);
	// text info
	if (dispatcher)
	{
		Font fontAlt1(&FontFamily(DEF_FONT_FACE), 18);
		Font fontAlt2(&FontFamily(DEF_FONT_FACE), 12);
		StringFormat format1;
		format.SetAlignment(StringAlignmentNear);
		std::wstring Where;
		if (Transdep.StopByID(dispatcher->inf.direction_end_stop_id))
			Where = Transdep.StopByID(dispatcher->inf.direction_end_stop_id)->getClean();
		else
			Where = dispatcher->inf.direction_end_stop_name;

		std::wstring Car = dispatcher->inf.model_name + TEXT(" :") + std::to_wstring(dispatcher->inf.car_type_id) + TEXT(" :") + std::to_wstring(dispatcher->inf.sitcount);
		std::wstring Stop = Transdep.StopByID(Transdep.times.stopID) ? Transdep.StopByID(Transdep.times.stopID)->getClean() : TEXT("-");

		graphics.DrawString(string::formatdatetime(&dispatcher->inf.leave_date, "%Y").c_str(), -1, &fontAlt1, Gdiplus::RectF(35, 45, 147, 30), &format, &blackBrush1);
		graphics.DrawString(string::formatdatetime(&dispatcher->inf.leave_date, "%m-%d").c_str(), -1, &fontAlt2, Gdiplus::RectF(40, 70, 147, 30), &format, &blackBrush2);
		graphics.DrawString(string::formatdatetime(&dispatcher->inf.leave_date, "%H:%M").c_str(), -1, &fontAlt2, Gdiplus::RectF(175, 60, 250, 30), &format, &blackBrush2);
		graphics.DrawString(dispatcher->inf.direction_start_stop_name.c_str(), -1, &fontAlt1, Gdiplus::RectF(260, 55, 147, 30), &format, &blackBrush1);
		graphics.DrawString(dispatcher->inf.direction_end_stop_name.c_str(), -1, &fontAlt2, Gdiplus::RectF(410, 60, 147, 30), &format, &blackBrush1);
		graphics.DrawString(Stop.c_str(), -1, &fontAlt2, Gdiplus::RectF(560, 60, 147, 30), &format, &blackBrush1);
		graphics.DrawString(dispatcher->inf.car_type_name.c_str(), -1, &fontAlt2, Gdiplus::RectF(670, 60, 147, 30), &format, &blackBrush2);
		Font fontAlt3(&FontFamily(DEF_FONT_FACE), 16);
		graphics.DrawString(std::to_wstring(dispatcher->inf.sitcount).c_str(), -1, &fontAlt3, Gdiplus::RectF(750, 192, 147, 25), &format, &blackBrush2);
		graphics.DrawString(std::to_wstring(dispatcher->inf.sitcount).c_str(), -1, &fontAlt1, Gdiplus::RectF(830, 55, 147, 30), &format, &blackBrush1);
		
	}

	return 0L;
}

std::wstring TimeTable::GetByDays(UINT days)
{
	const time_t ONE_DAY = 24 * 60 * 60;
	time_t time;
	struct tm timeinfo;
	std::wstring date;

	std::time(&time);
	::localtime_s(&timeinfo, &time);
	time = ::mktime(&timeinfo) + (days * ONE_DAY);
	::localtime_s(&timeinfo, &time);
	timeinfo.tm_year = timeinfo.tm_year + 1900;
	timeinfo.tm_mon = timeinfo.tm_mon + 1;

	return std::to_wstring(timeinfo.tm_year) + TEXT("-") + std::to_wstring(timeinfo.tm_mon) + TEXT("-") + std::to_wstring(timeinfo.tm_mday);
}
