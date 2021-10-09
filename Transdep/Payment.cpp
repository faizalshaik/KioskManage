#include "[vendor]/simplejson/json.h"

#include "../clientwnd.h"

#include "control.h"
#include "payment.h"

using namespace transdep;

TicketPreview::TicketPreview()
	: CPicturePanel()
{
	SetClass(TEXT("KioskMan::Payment::TicketPreview"));

	onPaint = OnPaint;
}

Payment::Payment()
	: CPanel()
{
	SetClass(TEXT("KioskMan::Payment"));

	preview  = NULL;
	onCreate = OnCreate;
}

Payment::~Payment()
{
	SafeDelete(preview);
}

LPVOID Payment::OnCreate(HWND hWnd, LPARAM lParam)
{
	Payment *payment = reinterpret_cast<Payment*>(GetUserDataLong(hWnd));
	if (payment)
	{
		payment->SetBkBrush(::CreateSolidBrush(RGB(100, 100, 240)));

		RECT rc = payment->GetClientRect();
		rc.left += 16;
		rc.bottom -= 5;
		rc.right -= 15;
		rc.top = rc.bottom - 85;
		payment->summary.Initialize(payment->GetInstance(), *payment, rc);
		payment->summary.SetBkBrush(BRUSH_WHITE);
		payment->summary.Show();

		payment->swiping.Create(payment->GetInstance(), Worker, Worker.GetClientRect());
		payment->swiping.SetBkBrush(BRUSH_WHITE);
		if (payment->swiping.animator.Initialize(payment->GetInstance(), Client, coord::to_rect(0, 0, 200, 200)))
		{
			payment->swiping.SetAnimation(cfg.DataPath(TEXT("cardread.gif")).c_str());
			payment->swiping.animator.SetParent(payment->swiping);
			payment->swiping.animator.Center();
		}

	}
	return 0L;
}

LPVOID TicketPreview::OnPaint(HWND OwnerWnd, HDC hDC, PAINTSTRUCT pStruct)
{
	TicketPreview *ticket = reinterpret_cast<TicketPreview*>(GetUserDataLong(OwnerWnd));
	if (ticket)
	{
		RECT rc = ticket->GetClientRect();
		Transdep.ticket[ticket->ID].Print(hDC, rc.right, 18);

		Graphics graphics(hDC);

		Pen      pen(Color(255, 81, 81, 81), 5);
		graphics.DrawRectangle(&pen, rc.left, rc.top, rc.right, rc.bottom);
	}
	return 0L;
}

TotalPanel::TotalPanel()
	: CPanel()
{
	SetClass(TEXT("KioskMan::Payment::Total"));
	onPaint = OnPaint;
}

LPVOID TotalPanel::OnPaint(HWND OwnerWnd, HDC hDC, PAINTSTRUCT pStruct)
{
	TotalPanel *total = reinterpret_cast<TotalPanel*>(GetUserDataLong(OwnerWnd));
	if (total)
	{
		RECT rc = total->GetClientRect();

		Graphics graphics(hDC);
		Pen      pen(Color(255, 81, 81, 81), 5);
		graphics.DrawRectangle(&pen, rc.left, rc.top, rc.right, rc.bottom);

		::SelectObject(hDC, cfg.Font(23));

		POINT p = coord::point(50, 10);

		std::wstring Line = TEXT("Захиалсан суудлын тоо: ") + std::to_wstring(Transdep.Bus.Reserved());
		::TextOut(hDC, p.x, p.y, Line.c_str(), Line.size());

		p.y += 25;
		if (Transdep.Bus.Reserved() == 1){
			::TextOut(hDC, p.x, p.y, (TEXT("Суудлын дугаар: ") + total->desc).c_str(), (TEXT("Суудлын дугаар: ") + total->desc).size());
		}
		else
		{
			::TextOut(hDC, p.x, p.y, (TEXT("Суудлын дугаарууд: ") + total->desc).c_str(), (TEXT("Суудлын дугаарууд: ") + total->desc).size());
			p.y += 25;
			Line = TEXT("Үүнээс: ") + (total->Adults > 0 ? TEXT("том хүн ") + std::to_wstring(total->Adults) : TEXT(""))
				+ (total->Childs > 0 ? ((total->Adults > 0 ? TEXT(", хүүхэд ") : TEXT("хүүхэд ")) + std::to_wstring(total->Childs)) : TEXT(""));
			::TextOut(hDC, p.x, p.y, Line.c_str(), Line.size());
		}

		p.y += 25;
		::SelectObject(hDC, cfg.Font(28));
		p.x = rc.right / 2;
		p.y = rc.bottom / 2 - 14;
		Line = TEXT("НИЙТ ТӨЛБӨРИЙН ХЭМЖЭЭ: ") + std::to_wstring(total->Price) + TEXT("₮");
		::TextOut(hDC, p.x, p.y, Line.c_str(), Line.size());
	}
	return 0L;
}

bool Payment::ConfirmTickets()
{	
	transdep::RawTariff *tariff = Transdep.RawTariffBy(
		Transdep.passage.dispatcher.direction_id,
		Transdep.passage.dispatcher.direction_start_stop_id,
		Transdep.times.stopID);

	time_t    rawtime;
	struct tm timeinfo;
	char      buffer[80];
	::time(&rawtime);
	::localtime_s(&timeinfo, &Transdep.passage.dispatcher.leave_date);
	::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &timeinfo);

	std::wstring hdr = TEXT("Content-Type: application/x-www-form-urlencoded; charset=utf-8\r\n");
	std::string  out_date(buffer);
	std::string  data_nuller;
	std::string  data;
	std::wstring response;
	for (UINT i = 0; i < Transdep.Bus.Reserved(); i++)
	{
/*		data_nuller = "machine_id=" + std::to_string(cfg.GetID());
		data_nuller += "&action=9";
		data_nuller += "&dispatcher_id=" + std::to_string(Transdep.passage.dispatcher.id);
		data_nuller += "&seat_no=" + std::to_string(Transdep.ticket[i].passenger.Seat);
		response = CInternet::PostData(METAKIOSK_SERVER, TEXT("service/set_cancel_seat"), hdr.c_str(), hdr.size(), (LPVOID)data_nuller.c_str(), data_nuller.length());
*/
		int price100, price92, price8, price_insurance;
		Transdep.ticket[i].GetPrices(*tariff, &price100, &price92, &price8, &price_insurance);

		std::string ebarimt = "machine_id=" + std::to_string(cfg.GetID());
		ebarimt            += "&amount=" + std::to_string(price8) + ".00";
		ebarimt            += "&code=1234";
		ebarimt            += "&name=rtert";
		ebarimt            += "&ticket=" + string::utf8encode(Transdep.ticket[i].ID.substr(1));
		ebarimt            += "&register=" + string::utf8encode(Transdep.ticket[i].passenger.Register);
		ebarimt            += "&passenger=" + string::utf8encode(Transdep.ticket[i].passenger.FullName);
		pugi::xml_document     doc;

		XML::LoadFromBuffer(&doc,
			Server::Post(TEXT("service/get_ebarimt_transdep"), ebarimt.c_str()));

/*		pugi::xml_parse_result xml_result = 
			doc.load_string(
				CInternet::PostData(
					METAKIOSK_SERVER, TEXT("service/get_ebarimt_transdep"), 
					hdr.c_str(), hdr.size(), (LPVOID) ebarimt.c_str(), 
					ebarimt.length()).c_str()
			);*/

		Transdep.ticket[i].passenger.Amount  = TEXT("");
		Transdep.ticket[i].passenger.DDTD    = TEXT("");
		Transdep.ticket[i].passenger.Lottery = TEXT("");
		Transdep.ticket[i].passenger.QR      = TEXT("");
		if (/*xml_result*/doc)
		{
			pugi::xml_node element = doc.child(TEXT("ebarimt"));
			if (element != NULL)
			{
				Transdep.ticket[i].passenger.Amount  = element.child_value(TEXT("amount"));
				Transdep.ticket[i].passenger.DDTD    = element.child_value(TEXT("billId"));
				Transdep.ticket[i].passenger.Lottery = element.child_value(TEXT("lottery"));
				Transdep.ticket[i].passenger.QR      = element.child_value(TEXT("qrData"));
			}
		}

		data = "machine_id=" + std::to_string(cfg.GetID());
		data += "&action=9";

		data += "&dispatcher_id=" + std::to_string(Transdep.passage.dispatcher.id);		

		data += "&start_stop_id=" + std::to_string(Transdep.passage.dispatcher.direction_start_stop_id);
		data += "&end_stop_id=" + std::to_string(Transdep.passage.dispatcher.direction_end_stop_id);

		data += "&ticket_type_id=" + std::to_string(tariff->ticket_type_id);

		data += "&is_child=";
		if (Transdep.ticket[i].passenger.Age > 12)
			data += "false";
		else
			data += "true";

		data += "&ticket_num=" + string::utf8encode(Transdep.ticket[i].ID.substr(1));
		data += "&register=" + string::utf8encode(Transdep.ticket[i].passenger.Register);
		data += "&passenger_name=" + string::utf8encode(Transdep.ticket[i].passenger.FullName);
		
		data += "&price92=" + std::to_string(price92);
		data += "&price8=" + std::to_string(price8);
		data += "&total=" + std::to_string(price100);

		data += "&car_type_id=" + std::to_string(Transdep.passage.dispatcher.car_type_id);

		data += "&insurance=" + std::to_string(Transdep.ticket[i].checkInsurance.IsChecked() ? price_insurance : 0);

		data += "&seat_no=" + std::to_string(Transdep.ticket[i].passenger.Seat);

		data += "&out_date=" + out_date;

		data += "&withchild_register=" + std::string("");
		data += "&withchild_name=" + std::string("");

		data += "&settlement_num=" + string::utf8encode(Transdep.ticket[i].ID.substr(8));

		data += "&created_by=875";

		data += "&i_company_id=" + std::to_string(tariff->end_i_com_id);

		data += "&billId=" + string::utf8encode(Transdep.ticket[i].passenger.DDTD);

		data += "&edou_duty=0";

		data += "&foreign_passenger=false";

		data += "&start_stop_sequence=" + std::to_string(tariff->start_stop_sequence);
		data += "&end_stop_sequence=" + std::to_string(tariff->end_stop_sequence);

		response = Server::Post(
			TEXT("service/set_ticket_confirm"),
			data.c_str());
/*
		response = CInternet::PostData(
			METAKIOSK_SERVER,
			TEXT("service/set_ticket_confirm"),
			hdr.c_str(),
			hdr.size(),
			(LPVOID)data.c_str(),
			data.length());
*/
		pugi::xml_document set_Ticket_Confirm;
		XML::LoadFromBuffer(&set_Ticket_Confirm, response);

		//::MessageBox(NULL, response.c_str(), L"Oder", MB_OK);

		Transdep.ticket[i].confirming.Empty();
		if (set_Ticket_Confirm)
			Transdep.ticket[i].confirming.ParseXML(&set_Ticket_Confirm);

		if (Transdep.ticket[i].confirming.raw.ErrorMsg == TEXT(""))
		{
			// Билет Авто тээврийн серверт амжилттай бичигдлээ
			TRANSACTIONED = TRUE;
			LAST_ERRIOR = TEXT("Гүйлгээ амжилттай хийгдэж, таны захиалга баталгаажлаа.\n Та аян замдаа сайн яваарай!");
		}
		else
		{
			TRANSACTIONED = FALSE;
			LAST_ERRIOR = TEXT("Захиалга амжилтгүй болсон тул гүйлгээ буцаагдлаа.\n Уучлаарай! Та дахин оролдоно уу!");
			// Билет захиалах явцад алдаа гарлаа
			// Хүний мөнгийг буцааж өгөх хэрэгтэй
		}
	}
	return true;
}

void Payment::SendTickets()
{
	std::wstring hdr = TEXT("Content-Type: application/x-www-form-urlencoded; charset=utf-8\r\n");
	for (UINT i = 0; i < Transdep.Bus.Reserved(); i++)
	{
		std::string data = "ID=" + string::utf8encode(Transdep.ticket[i].ID);
		data += "&machine_id=0" + std::to_string(cfg.GetID());
		data += "&direction_id=" + std::to_string(Transdep.passage.dispatcher.direction_id);
		data += "&start_stop_id=" + std::to_string(Transdep.passage.dispatcher.direction_start_stop_id);
		data += "&end_stop_id=" + std::to_string(Transdep.passage.dispatcher.direction_end_stop_id);
		data += "&register=" + string::utf8encode(Transdep.ticket[i].passenger.Register);
		data += "&seat=" + std::to_string(Transdep.ticket[i].passenger.Seat);
		data += "&direction=" + string::utf8encode(Transdep.passage.dispatcher.direction_name);
		data += "&company=" + string::utf8encode(Transdep.passage.Company);
		data += "&price=" + std::to_string(Transdep.ticket[i].TotalPrice());
		data += "&card=1234";
		data += "&passenger=" + string::utf8encode(Transdep.ticket[i].passenger.FullName);

		Server::Post(TEXT("service/collect/ticket"), data.c_str());
/*
		CInternet::PostData(
			METAKIOSK_SERVER,
			TEXT("service/collect/ticket"),
			hdr.c_str(),
			hdr.size(),
			(LPVOID)data.c_str(),
			data.length());
			*/
	}
}

void Payment::PrintTickets()
{
	WCHAR szPrinterName[256];
	DWORD dwNameLen = 256;
	GetDefaultPrinter(szPrinterName, &dwNameLen);

	
	// Create the HDC for the printer
//	if ((hDC = CreateDC(TEXT("WINSPOOL"), szPrinterName, NULL, NULL)) == NULL)

	HDC K80DC = ::CreateDC(TEXT("WINSPOOL"), szPrinterName /*L"CUSTOM K80"*/, NULL, NULL);

	for (UINT i = 0; i < Transdep.Bus.Reserved(); i++)
	{
		DOCINFO doc = { sizeof(DOCINFO), TEXT("Тасалбар") };
		StartDoc(K80DC, &doc);
		StartPage(K80DC);
		Transdep.ticket[i].Print(K80DC, 630, 30);
		EndPage(K80DC);
		EndDoc(K80DC);
	}
	::DeleteDC(K80DC);
}

void Payment::Databank(LPVOID arg)
{
	bool transaction = false;
	
	std::wstring doSaleTransaction = Transdep.pay.DatabankDoSaleTransaction();
	//std::wstring doSaleTransaction = L"{ \"status\": true, \"response\" : { \"db_ref_no\": \"171008164504\", \"trace_no\" : \"000040\", \"amount\" : \"5000\", \"response_code\" : \"000\", \"response_msg\" : \"Гүйлгээ зөвшөөрөгдсөн.\", \"entry_mode\" : \"Swipe Pin Verified\", \"card_holder_name\" : \".MR\", \"pan\" : \"949610XXXXXX0790\", \"auth_code\" : \"LMQYT6\", \"terminal_id\" : \"70012409\", \"batch_no\" : \"000000000001\", \"rrn\" : \"748281331408\", \"is_vatps\" : \"0\", \"time\" : \"16:46:42\", \"date\" : \"10/08\", \"merchant_name\" : \"Erdem denver Test\", \"bank_mb_code\" : \"05\" } }";

	//::MessageBox(NULL, doSaleTransaction.c_str(), L"POS data", 0);
	std::string logdata1 = "machine_id=" + std::to_string(cfg.GetID());
	logdata1            += "&message=" + string::utf8encode(doSaleTransaction);
	logdata1            += "&execution=";
	std::string logdata2 = "";

	std::wstring response_code = TEXT("");
	std::wstring response_msg  = TEXT("");
	std::wstring ErrorCode     = TEXT("");
	std::wstring ErrorMessage  = TEXT("");

	std::wstring db_ref_no = Transdep.ticket[0].ID.substr(8);
	std::wstring trace_no = TEXT("000000");

	JSONValue *value = JSON::Parse(doSaleTransaction.c_str());
	if (value == NULL)
	{
		logdata1 += "Technical problem: Level=1!";
	}
	else
	{
		bool status;

		JSONObject root;
		if (value->IsObject() == false)
		{
			logdata1 += "Technical problem: Level=2!";
		}
		else
		{
			root = value->AsObject();

			if (root.find(TEXT("status")) != root.end() && root[TEXT("status")]->IsBool())
			{
				status = root[TEXT("status")]->AsBool();
			}
			else
			{
				logdata1 += "Technical problem: Level=3!";
			}

			if (root.find(TEXT("response")) != root.end() && root[TEXT("response")]->IsObject())
			{
				JSONObject response = root[TEXT("response")]->AsObject();

				if (status)
				{
					if (response.find(TEXT("response_code")) != response.end() && response[TEXT("response_code")]->IsString())
						response_code = response[TEXT("response_code")]->AsString();
					if (response.find(TEXT("response_msg")) != response.end() && response[TEXT("response_msg")]->IsString())
						response_msg = response[TEXT("response_msg")]->AsString();
				}
				else
				{
					if (response.find(TEXT("Exception")) != response.end() && response[TEXT("Exception")]->IsObject())
					{
						JSONObject Exception = response[TEXT("Exception")]->AsObject();
						if (Exception.find(TEXT("ErrorCode")) != Exception.end() && Exception[TEXT("ErrorCode")]->IsString())
							response_code = Exception[TEXT("ErrorCode")]->AsString();
						if (Exception.find(TEXT("ErrorMessage")) != Exception.end() && Exception[TEXT("ErrorMessage")]->IsString())
							response_msg = Exception[TEXT("ErrorMessage")]->AsString();
					}
				}

				if (status)
				{
					logdata1 += "Trancastion completed!";
					if (response_code == TEXT("000"))
					{
						// Гүйлгээ амжилттай!
						transaction = true;

						std::wstring is_vatps  = TEXT("0");

						if (response.find(TEXT("db_ref_no")) != response.end() && response[TEXT("db_ref_no")]->IsString())
							db_ref_no = response[TEXT("db_ref_no")]->AsString();
						if (response.find(TEXT("trace_no")) != response.end() && response[TEXT("trace_no")]->IsString())
							trace_no = response[TEXT("trace_no")]->AsString();
						if (response.find(TEXT("is_vatps")) != response.end() && response[TEXT("is_vatps")]->IsString())
							is_vatps = response[TEXT("is_vatps")]->AsString();

						std::wstring confirmSaleCompleted =
							Transdep.pay.DatabankConfirmSaleCompleted(Transdep.ticket[0].ID, db_ref_no, trace_no, is_vatps);

						logdata2 = "machine_id=" + std::to_string(cfg.GetID());
						logdata2 += "&message=" + string::utf8encode(confirmSaleCompleted);
					}
					else
					{
						// Гүйлгээ алдаатай!
					}
				}
				else
				{
					logdata1 += "Technical problem: Level=0!";
				}
			}
			else
			{
				logdata1 += "Technical problem: Level=4!"; // Databank connection-оос response мсж ирээгүй
			}
		}
		delete value;
	}

	std::wstring hdr = TEXT("Content-Type: application/x-www-form-urlencoded; charset=utf-8\r\n");
	Server::Post(TEXT("service/do_sale_transaction"), logdata1.c_str());
/*
	CInternet::PostData(
		METAKIOSK_SERVER,
		TEXT("service/do_sale_transaction"),
		hdr.c_str(),
		hdr.size(),
		(LPVOID)logdata1.c_str(),
		logdata1.length());
*/
	Transdep.pay.swiping.Hide();

	if (logdata2 != "")
	{
		Server::Post(TEXT("service/confirm_sale_completed"), logdata2.c_str());
//		CInternet::PostData(METAKIOSK_SERVER, TEXT("service/confirm_sale_completed"), hdr.c_str(), hdr.size(), (LPVOID)logdata2.c_str(), logdata2.length());

		Transdep.pay.ConfirmTickets();

		if (Transdep.pay.TRANSACTIONED)
		{
			Transdep.pay.PrintTickets();
			Transdep.pay.SendTickets();
		}
		else
		{
			Databank::VoidTransaction(db_ref_no.c_str(), trace_no.c_str());
		}
	}

	if (transaction)
	{
//		Client.message.AssignBitmap(Bmp::LoadPicture(cfg.DataPath(TEXT("memento-mn.jpg")).c_str()));
		Client.message.SetText(Transdep.pay.LAST_ERRIOR.c_str());
		Client.footer.left.ShowText(TEXT("БАЯРЛАЛАА"));
	}
	else
	{
		std::wstring msg = TEXT("Төлбөрийн картаар гүйлгээ хийж чадсангүй.\n [Aлдаа: ");
		if (response_code != TEXT(""))
		{
			msg += response_code + TEXT("] ") + response_msg;
		}
		else if (ErrorCode != TEXT(""))
		{
			msg += ErrorCode + TEXT("] ") + ErrorMessage;
		}
		else
		{
			msg += TEXT("000] Technical problem!");
		}
		Client.message.SetText(msg.c_str());

		Client.footer.left.ShowText(TEXT("БОЛИХ"));
		Client.footer.right.ShowText(TEXT("ДАХИН УНШУУЛАХ"));
	}
	Client.message.Show();
	Client.message.Redraw();
}

std::wstring Payment::DatabankDoSaleTransaction()
{
	std::wstring hdr = TEXT("Content-Type: application/json; charset=utf-8");

	std::wstring db_ref_no = Transdep.ticket[0].ID.substr(8);
	std::string  data = "{\"service_name\": \"doSaleTransaction\",\"service_params\":{ \"amount\": \"";
	data += std::to_string(summary.Price) + "\", \"db_ref_no\":\"";
	data += string::utf8encode(db_ref_no);
	data += "\", \"vatps_bill_type\": \"3\"}}";

//	std::wstring response = Server::Post(TEXT(""), data.c_str());

	std::wstring response = CInternet::PostData(
		TEXT("localhost"),
		TEXT(""),
		hdr.c_str(),
		hdr.size(),
		(LPVOID)data.c_str(),
		data.length(),
		27028);

	
	return response;
}

std::wstring Payment::DatabankConfirmSaleCompleted(
	std::wstring ddtd, std::wstring db_ref_no, std::wstring trace_no, std::wstring is_vatps)
{
	std::wstring hdr = TEXT("Content-Type: application/json; charset=utf-8");

	std::string  data = "{\"service_name\": \"confirmSaleCompleted\",\"service_params\":{ \"ddtd\": \"";
	data += string::utf8encode(ddtd) + "\", \"db_ref_no\":\"";
	data += string::utf8encode(db_ref_no);
	data += "\", \"trace_no\": \"" + string::utf8encode(trace_no) + "\", \"is_vatps\": \"";
	data += string::utf8encode(is_vatps) + "\"}}";

	//std::wstring response = Server::Post(TEXT(""), data.c_str());

	
	std::wstring response = CInternet::PostData(
		TEXT("localhost"),
		TEXT(""),
		hdr.c_str(),
		hdr.size(),
		(LPVOID)data.c_str(),
		data.length(),
		27028);
	
	return response;
}

std::wstring Payment::DatabankVoidTransaction(std::wstring db_ref_no, std::wstring trace_no)
{
	std::wstring hdr = TEXT("Content-Type: application/json; charset=utf-8");

	std::string  data = "{\"service_name\": \"voidTransaction\",\"service_params\":{ \"db_ref_no\":\"";
	data += string::utf8encode(db_ref_no);
	data += "\", \"trace_no\": \"" + string::utf8encode(trace_no) + "\"}}";

	
//	std::wstring response = Server::Post(TEXT(""), data.c_str());

	std::wstring response = CInternet::PostData(
		TEXT("localhost"),
		TEXT(""),
		hdr.c_str(),
		hdr.size(),
		(LPVOID) data.c_str(),
		data.length(),
		27028);
	return response;
}