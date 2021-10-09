#include <algorithm>
#include <vector>

#include "../clientwnd.h"
#include "control.h"

using namespace globalpay;

transdep::TransdepControl Transdep;

using namespace transdep;

TransdepControl::TransdepControl() 
{
	passage.StartAimag = TEXT("Улаанбаатар хот");
}

bool TransdepControl::LoadRaws(bool bReload) 
{
	if (RawsLoaded())
	{
		if (!bReload)
			return false;
		else
		{
			tocountry.Reset();
			whole.Reset();
		}
	}

	pugi::xml_document
		get_Tariff_to_Country,
		get_all_stop;

	XML::LoadFromFile(&get_Tariff_to_Country, cfg.DataPath(TEXT("transdep\\")) + TEXT("tariff.xml"));
	XML::LoadFromFile(&get_all_stop, cfg.DataPath(TEXT("transdep\\")) + TEXT("get_all_stop.xml"));

	if (get_Tariff_to_Country)
		tocountry.ParseXML(&get_Tariff_to_Country);

	if (get_all_stop)
		whole.ParseXML(&get_all_stop);

	return RawsLoaded();
}

bool TransdepControl::SetMaster()
{
	if (!RawsLoaded())
		return false;

	directions.clear();
	stops.clear();
	aimags.clear();

	for (auto tariff = tocountry.raw->begin(); tariff != tocountry.raw->end(); ++tariff)
	{
		if (std::find(directions.begin(), directions.end(), tariff->direction_id) == directions.end())
			directions.push_back(Direction(tariff->direction_id, tariff->direction_name, tariff->direction_UX_eseh));

		if (std::find(stops.begin(), stops.end(), tariff->end_stop_id) == stops.end())
			stops.push_back(Stop(tariff->end_stop_id, tariff->end_stop_name, tariff->end_stop_lat, tariff->end_stop_lon, tariff->end_stop_aimag_id));

		if (std::find(stops.begin(), stops.end(), tariff->start_stop_id) == stops.end())
			stops.push_back(Stop(tariff->start_stop_id, tariff->start_stop_name, tariff->start_stop_lat, tariff->start_stop_lon, tariff->aimag_id));

		if (std::find(aimags.begin(), aimags.end(), tariff->aimag_id) == aimags.end())
		{
			if (Aimag::IsMglAimag(tariff->aimag_id))
				aimags.push_back(Aimag(tariff->aimag_id, tariff->start_stop_name));
		}

		if (std::find(aimags.begin(), aimags.end(), tariff->end_stop_aimag_id) == aimags.end())
		{
			if (Aimag::IsMglAimag(tariff->end_stop_aimag_id))
				aimags.push_back(Aimag(tariff->end_stop_aimag_id, tariff->end_stop_name));
		}
	}
	
	for (auto dir = whole.raw->begin(); dir != whole.raw->end(); ++dir)
	{
		//if (std::find(directions.begin(), directions.end(), dir->direction_id) == directions.end())
			//directions.push_back(Direction(dir->direction_id, dir->direction_name, dir->direction_UX_eseh));
		//if (std::find(stops.begin(), stops.end(), dir->stop_id) == stops.end())
			//stops.push_back(Stop(dir->stop_id, dir->stop_name, dir->lat, dir->lon, dir->aimag_id));
		//if (std::find(aimags.begin(), aimags.end(), dir->aimag_id) == aimags.end())
		{
			//if (Aimag::IsMglAimag(dir->aimag_id))
				//aimags.push_back(Aimag(dir->aimag_id, dir->stop_name));
		}
	}
	
	std::sort(aimags.begin(), aimags.end());

	return !directions.empty();
}

bool TransdepControl::CookIt()
{
	if (!RawsLoaded())
		return false;

	tariff.clear();
	start.clear();
	end.clear();
	insurance.clear();

	for (auto trff = tocountry.raw->begin(); trff != tocountry.raw->end(); ++trff)
	{
		if (std::find(tariff.begin(), tariff.end(), trff->id) == tariff.end())
			tariff.push_back(Tariff(trff->id));

		if (std::find(start.begin(), start.end(), trff->start_stop_id) == start.end())
			start.push_back(trff->start_stop_id);

		if (std::find(end.begin(), end.end(), trff->end_stop_id) == end.end())
			end.push_back(EndStop(trff->end_stop_id, trff->direction_id));
		else
		{
			if (EndStopByID(trff->end_stop_id))
				EndStopByID(trff->end_stop_id)->AttachTariff(trff->direction_id);
		}

		if (std::find(insurance.begin(), insurance.end(), trff->end_i_com_id) == insurance.end())
			insurance.push_back(Insurance(trff->end_i_com_id, trff->end_i_com_name, trff->end_i_com_reg));
	}

	return !tariff.empty();
}

bool TransdepControl::GenerateRoutes()
{
	if (directions.empty())
		return false;

	routes.clear();

	Route route;
	for (auto d = directions.begin(); d != directions.end(); ++d)
	{
		route.Reset();
		
		//std::for_each(whole.raw->begin(), whole.raw->end(),
			//[d, &route](const RawDirection r) ->
			//void
			//{
				//if (r.direction_id == d->ID && ! r.is_rtrn)
				//{
				//route.Extend(StopSequence(r.stop_id, r.stop_sequence));
				//}
			//}
		//);
		route.Sort();
		routes.push_back(route);
	}

	Transdep.router.Init(Client.GetInstance(), Worker, Worker.GetClientRect());

	Transdep.times.Initialize(Client.GetInstance(), Worker, Worker.GetClientRect());
	Transdep.times.SetBkBrush(BRUSH_WHITE);
	Transdep.times.CreateChilds();

	// WORKING HERE
	//Transdep.selectionScreen.InitializeHollow(Client.GetInstance(), Worker, Worker.GetClientRect());
	//scr.Create();

	Transdep.Bus.Initialize(Client.GetInstance(), Worker, Worker.GetClientRect());
	Transdep.Bus.SetBkBrush(BRUSH_WHITE);

	LONG ticketWidth = (coord::width(Worker.GetClientRect()) - 30) / 2;
	LONG ticketHeight = (coord::height(Worker.GetClientRect()) - coord::height(Keyboard.GetRect()) - 30) / 2;
	for (int i = 0; i < TRANSDEP_MAX_RESERV; i++)
	{
		ticket[i].Initialize(Client.GetInstance(), Worker, coord::to_rect(10, 10, ticketWidth, ticketHeight));
		if (i)
		{
			ticket[i].SetAfter(ticket[i - 1]);
			ticket[i].FitAfterSibling();
		}
	}

	Transdep.pay.Initialize(Client.GetInstance(), Worker, Worker.GetClientRect());

	return !routes.empty();
}

bool TransdepControl::GenerateStations()
{
	if (stops.empty())
		return false;

	return true;
}

Direction* TransdepControl::DirectionByID(UINT id)
{
	std::vector<Direction>::iterator iter = std::find_if(directions.begin(), directions.end(),
		[id](const Direction d) -> bool { return d.ID == id; });

	if (iter != directions.end())
	{
		size_t index = std::distance(directions.begin(), iter);
		return &directions[index];
	}
	return NULL;
}

Tariff* TransdepControl::TariffByID(UINT id)
{
	std::vector<Tariff>::iterator iter = std::find_if(tariff.begin(), tariff.end(),
		[id](const Tariff d) -> bool { return d.ID == id; });
	if (iter != tariff.end())
	{
		size_t index = std::distance(tariff.begin(), iter);
		return &tariff[index];
	}
	return NULL;
}

RawTariff* TransdepControl::RawTariffBy(UINT direction_id, UINT start_stop_id, UINT end_stop_id)
{
	std::vector<RawTariff>::iterator iter = std::find_if(tocountry.raw->begin(), tocountry.raw->end(),
		[direction_id, start_stop_id, end_stop_id]
	(const RawTariff tariff) -> bool
	{
		return (
			tariff.direction_id == direction_id
			//				&& tariff.start_stop_id == start_stop_id
			&& tariff.end_stop_id == end_stop_id
			);
	});

	if (iter != tocountry.raw->end())
	{
		size_t index = std::distance(tocountry.raw->begin(), iter);
		return &tocountry.raw->at(index);
	}
	return NULL;
}

Stop* TransdepControl::StopByID(UINT id)
{
	if (id == 328)
		id = 3;

	std::vector<Stop>::iterator iter = std::find_if(stops.begin(), stops.end(),
		[id](const Stop s) -> bool { return s.ID == id; });

	if (iter != stops.end())
	{
		size_t index = std::distance(stops.begin(), iter);
		return &stops[index];
	}
	return NULL;
}

EndStop* TransdepControl::EndStopByID(UINT id)
{
	if (id == 328)
		id = 3;

	std::vector<EndStop>::iterator iter = std::find_if(end.begin(), end.end(),
		[id](const EndStop s) -> bool { return s.ID == id; });

	if (iter != end.end())
	{
		size_t index = std::distance(end.begin(), iter);
		return &end[index];
	}
	return NULL;
}

Insurance* TransdepControl::InsuranceByID(UINT id)
{
	std::vector<Insurance>::iterator iter = std::find_if(insurance.begin(), insurance.end(),
		[id](const Insurance i) -> bool { return i.ID == id; });

	if (iter != insurance.end())
	{
		size_t index = std::distance(insurance.begin(), iter);
		return &insurance[index];
	}
	return NULL;
}

Station* TransdepControl::StationByID(UINT id)
{
	return NULL;
}

Aimag* TransdepControl::AimagByID(UINT id)
{
	std::vector<Aimag>::iterator iter = std::find_if(aimags.begin(), aimags.end(),
		[id](const Aimag i) -> bool { return i.ID == id; });

	if (iter != aimags.end())
	{
		size_t index = std::distance(aimags.begin(), iter);
		return &aimags[index];
	}
	return NULL;
}

BOOL TransdepControl::IsStartStop(UINT start_stop_id)
{
	if (start_stop_id == 328)
		start_stop_id = 3;

	return !(std::find(start.begin(), start.end(), start_stop_id) == start.end());
}

bool TransdepControl::RawsLoaded()
{
	return !tocountry.IsEmpty() && !whole.IsEmpty();
}


void TransdepControl::OnSelectionScreen()
{
	Worker.Color(BRUSH_PRIMARY);
	Worker.Icon(BITMAP_TRANSDEP);
	Worker.Timer(INTERVAL_TRANSDEP);
	Worker.Status(TEXT_TRANSDEP01_MN);
	Worker.Painter(Selection::Painter);

	Worker.Left(TEXT_HOME_MN);
	Worker.Right(TEXT("БАТАЛГААЖУУЛАХ"), BRUSH_RED, Bmp::LoadGDI(cfg.DataPath(TEXT("transdep\\cards.png")).c_str()));
	Worker.Show();
}

void TransdepControl::OnRouter()
{

	//Worker.cleanState();
	//scr.hideButtons(TRUE, TRUE);
	Worker.Icon(BITMAP_TRANSDEP);
	Worker.Timer(INTERVAL_TRANSDEP);
//	Resource\Images\zoom
	Worker.Status(TEXT_TRANSDEP01_MN);//TEXT("Та явах чиглэл, очих буудлаа сонгоно уу!"));
	//SafeDelete(popup1.work2.stops);
	//popup1.Init(Worker.GetInstance(), Worker.GetHandle(), Worker.GetClientRect());
	//popup1.Show();
	router.setVisible(true);
	Client.footer.left.ShowText(TEXT_BACK_MN);
}

void TransdepControl::OnDispatcher()
{
	Worker.Status((TEXT("Улаанбаатар > ") + StopByID(times.stopID)->getClean() + TEXT(" чиглэлийн цагийн хуваарь...")).c_str());

	Client.loading.SetRect(cfg.Worker());
	Client.loading.Show();

	service.SetWorker(transdep::TimeTable::GetDispatchers);
	service.Start();
}

void TransdepControl::OnSeat()
{
	//Client.header.Status((TEXT("Та суудлаа сонгоно уу! ") + std::to_wstring(TRANSDEP_MAX_RESERV) + TEXT(" хүртэлх тооны суудал захиалах боломжтой.")).c_str());
	Worker.Status(CharUpper((LPWSTR)(TEXT("УЛААНБААТАР -> ") + Transdep.passage.DirEndAimag).c_str()));

	Bus.CancelAll();
	Bus.Show();

	RECT rect = Bus.GetRect();
	rect.top += 110;
	rect.bottom = rect.top + 490;
	Client.loading.SetRect(rect);
	Client.loading.Show();

	service.SetWorker(transdep::Autobus::GetEmptySeats);
	service.Start(NULL);
}

void TransdepControl::OnTicket()
{
	Worker.Status(TEXT("Зорчигчдийн мэдээллийг оруулна уу!"));

	// 	Keyboard.enableKey(keyAll);
	Keyboard.Show();

	if (Client.GetLastState() == ST_TRANSDEP_SEATME)
	{
		for (UINT i = 0; i < Bus.Reserved(); i++) 
		{
			ticket[i].ID = TEXT("01");

			if (passage.dispatcher.direction_start_stop_id < 10)
				ticket[i].ID += TEXT("00");
			else if (passage.dispatcher.direction_start_stop_id < 100)
				ticket[i].ID += TEXT("0");
			ticket[i].ID += std::to_wstring(passage.dispatcher.direction_start_stop_id);

			if (passage.dispatcher.direction_end_stop_id < 10)
				ticket[i].ID += TEXT("00");
			else if (passage.dispatcher.direction_end_stop_id < 100)
				ticket[i].ID += TEXT("0");
			ticket[i].ID += std::to_wstring(passage.dispatcher.direction_end_stop_id);

			ticket[i].ID += string::formatdatetime(NULL, "%y%m%d%H%M");

			if (Bus.reserve[i] < 10)
				ticket[i].ID += TEXT("0");

			ticket[i].ID += std::to_wstring(Bus.reserve[i]);
			ticket[i].txtSeat.SetText((TEXT("#") + std::to_wstring(Bus.reserve[i])).c_str());

			ticket[i].Adult.Check();
			ticket[i].Child.Check(false);
			ticket[i].Adult.Enable();
			ticket[i].Child.Enable();
			ticket[i].checkInsurance.Check(false);

			ticket[i].OnChangeSomething(ticket[i].Adult);

			ticket[i].txtFullName.SetText(TEXT(""));
			ticket[i].txtFullName.Enable();
			ticket[i].txtRegister.SetText(TEXT(""));
			ticket[i].txtRegister.Enable();

			ticket[i].Show();

			if (ticket[i].CalculatePrice() == RAW_EMPTY_VALUE) {
				Client.JumpStart();
				return;
			}
		}
		Keyboard.SetTarget(ticket[0].txtRegister);
	}
	else
	{
		for (UINT i = 0; i < Bus.Reserved(); i++)
		{
			ticket[i].Show();
		}

		if (transdep::Ticket::CheckCompleting())
		{
			Client.footer.right.AssignBitmap(Bmp::LoadGDI(cfg.DataPath(TEXT("footer\\next-arrow.png")).c_str()));
			Client.footer.right.SetColor(BRUSH_TERTIARY);
			Client.footer.right.ShowText(TEXT("ТӨЛБӨР ТӨЛӨХ"));
		}

		if (::IsWindow(Keyboard.GetTarget()))
		{
			::SetFocus(Keyboard.GetTarget());
			::RedrawWindow(Keyboard.GetTarget(), NULL, NULL, RDW_INVALIDATE);
		}
	}
	Client.footer.left.ShowText(TEXT("СУУДАЛ СОЛИХ"));
}

void TransdepControl::OnPayPreview()
{
	Worker.Status(TEXT("Зорчигчдийн мэдээлэл хянах, төлбөр төлөх..."));

	SafeDelete(pay.preview);

	if (Bus.Reserved())
		pay.preview = new transdep::TicketPreview[Bus.Reserved()];

	UINT totalPrice = 0;
	UINT Adults = 0;
	UINT Childs = 0;
	std::wstring description;

	for (UINT i = 0; i < Bus.Reserved(); i++)
	{
		ticket[i].Hide();

		if (description != TEXT(""))
		{
			description += TEXT(", ");
		}
		else
		{
			description = TEXT("");
		}

		description += TEXT("#") + std::to_wstring(Bus.reserve[i]);

		pay.preview[i].Initialize(Client.GetInstance(), pay, coord::to_rect(16, 20, 300, 700));

		if (i)
			pay.preview[i].FitAfterSibling(15);

		pay.preview[i].ID = i;
		pay.preview[i].SetBkBrush(BRUSH_WHITE);

		WCHAR szBuffer[256];
		ticket[i].txtFullName.GetText(szBuffer);
		ticket[i].passenger.FullName = szBuffer;

		ticket[i].passenger.Lottery = TEXT("");
		ticket[i].passenger.Amount = TEXT("");
		ticket[i].passenger.TTD = TEXT("");
		ticket[i].passenger.DDTD = TEXT("");
		ticket[i].passenger.QR = TEXT("");

		if (ticket[i].Adult.IsChecked())
		{
			++Adults;
			ticket[i].passenger.Age = 21; // TEXT("Том хүн");
		}
		else
		{
			++Childs;
			ticket[i].passenger.Age = 11; // TEXT("Хүүхэд");
		}

		ticket[i].txtRegister.GetText(szBuffer);
		ticket[i].passenger.Register = szBuffer;
		ticket[i].passenger.Seat = Bus.reserve[i];
		totalPrice += (UINT)(ticket[i].CalculatePrice() + ticket[i].CalculateInsurance());

		pay.preview[i].Show();
	}

	pay.summary.desc = description;
	pay.summary.Price = totalPrice;
	pay.summary.Adults = Adults;
	pay.summary.Childs = Childs;

	pay.Show();
	pay.summary.Redraw();

	Client.footer.left.ShowText(TEXT("БУЦАХ"));

	Client.footer.right.AssignBitmap(Bmp::LoadGDI(cfg.DataPath(TEXT("transdep\\cards.png")).c_str()));
	Client.footer.right.SetColor(BRUSH_RED);
	Client.footer.right.ShowText(TEXT("БАТАЛГААЖУУЛАХ"));

	//::SetFocus(Client.footer.right);
}

void TransdepControl::OnPayProcess()
{
	if (Bus.Reserved() > 1)
		Worker.Status(TEXT("Таны сонгосон суудлуудын захиалгын статусыг шалгаж байна..."));
	else
		Worker.Status(TEXT("Таны сонгосон суудлын захиалгын статусыг шалгаж байна.."));

	Client.loading.SetRect(cfg.Worker());
	Client.loading.Show();

	service.SetWorker(transdep::Ticket::SetOrderSeats);
	service.Start();
}

void TransdepControl::OnPayDatabank()
{
	Worker.Status(TEXT("Төлбөр картаа уншуулcнаар гүйлгээ хийгдэж, захиалга баталгаажна!"));

	pay.swiping.Show();

	if (pay.Transaction.IsWorking())
	{
		service.SetWorker(transdep::Payment::Databank);
		service.Start();

		pay.Transaction.Stop(true);
	}
	else
	{
		pay.Transaction.Set(transdep::Payment::Databank);
		pay.Transaction.Start();

		if (service.IsWorking())
			service.Stop(true);
	}
}