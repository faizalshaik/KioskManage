#pragma once

#include "window/hoverbitmapwnd.h"
#include "control/panel.h"
#include "control/static.h"
#include "control/edit.h"
#include "control/static.h"
#include "control/radio.h"
#include "control/checkbox.h"
#include "barcode/barcode.h"
#include "barcode/qrcode.h"

using namespace globalpay;

namespace transdep {
	typedef struct
	{
		UINT         id;
		std::wstring LastName;
		std::wstring FirstName;
		UINT         Age;
		std::wstring Gender;
		std::wstring Password;
		std::wstring Register;
		std::wstring Address;
		std::wstring Description;
		UINT         AimagKhot;
		UINT         SumDuureg;
		UINT         Heseg;
		time_t       UpdatedAt;

		std::wstring FullName;

		UINT         Seat;

		std::wstring QR;
		std::wstring Lottery;
		std::wstring Amount;
		std::wstring DDTD;
		std::wstring TTD;

		void Empty();
	} Passenger;

	struct RawOrderSeat : public RawData
	{
		UINT         zahialagch_id;
		std::wstring zahialagch_branch_name;
		std::wstring zahialagch_name;
		time_t       zahialagch_at;
		time_t       expired;

		std::wstring ErrorMsg;
	};

	class OrderSeat : public XML
	{
	public:
		OrderSeat();

		void Empty();
		void ParseXML(const pugi::xml_document *);

		RawOrderSeat raw;
	};

	struct RawTicketConfirm : public RawData
	{
		UINT         dispatcher_id;
		UINT         start_stop_id;
		std::wstring start_stop_name;
		UINT         end_stop_id;
		std::wstring end_stop_name;
		bool         is_child;
		UINT         ticket_num;
		std::wstring register_number;
		std::wstring passenger_name;
		UINT         price92;
		UINT         price8;
		UINT         total;
		bool         canceled;
		bool         printed;
		UINT         seat_no;
		std::wstring withchild_register;
		std::wstring withchild_name;
		UINT         created_by;
		std::wstring created_firstname;
		time_t       created_at;
		UINT         ticket_type_id;
		time_t       out_date;
		UINT         branch_id;
		std::wstring number;
		UINT         insurance;
		UINT         company_id;
		UINT         priceOS;
		UINT         priceBankFees;
		std::wstring billId;
		UINT         edou_duty;
		time_t       settlement_date;
		time_t       billDate;

		std::wstring ErrorMsg;
	};

	class TicketConfirm : public XML
	{
	public:
		TicketConfirm();

		void Empty();
		void ParseXML(const pugi::xml_document *);

		RawTicketConfirm raw;
	};

	class Ticket : public CPanel
	{
	public:
		Ticket();

		std::wstring ID;

		CStatic      Seat,
				     Age,
				     Register,
				     FullName,
				     Insurance,
				     Payment;

		CRadio       Adult,
   				     Child;

		CEdit        txtSeat,
				     txtRegister,
				     txtFullName,
				     txtPayment;

		CCheckBox    checkInsurance;

		CButton      SendRegister;

		Passenger    passenger;

		OrderSeat    ordering;
		TicketConfirm
					 confirming;

		Passenger    GetByRegister(LPCTSTR);
		double       CalculatePrice();
		double       CalculateInsurance();
		double       TotalPrice();
		void         GetPrices(RawTariff, int *, int *, int *, int *);

		void         Print(HDC hDC, UINT hDCwidth, UINT fontSize);

		bool         SetOrderSeat();
		static void  SetOrderSeats(LPVOID);
		static bool  CheckCompleting();

	protected:
		static LPVOID CALLBACK OnCreate(HWND hWnd, LPARAM lParam);
		static void            OnSendRegister(HWND hWnd);
		static LPVOID CALLBACK OnEditDown(HWND hWnd, UINT mButton, WPARAM sState, int X, int Y);
		static LPVOID CALLBACK OnPaint(HWND OwnerWnd, HDC hDC, PAINTSTRUCT pStruct);
		static LPVOID CALLBACK OnCommand(HWND hWnd, WPARAM wParam, LPARAM lParam);

	public:
		static void            OnChangeSomething(HWND hWnd);
	};
}