#pragma once

#include "control/panel.h"
#include "internet/internet.h"
#include "types/string.h"
#include "system/thread.h"

#include "../config.hpp"

#include "seatme.h"

using namespace globalpay;

namespace transdep {
	struct RawDispatcher : public RawData
	{
		UINT         distribution_id;
		UINT         direction_id;
		std::wstring direction_name;
		UINT         direction_start_stop_id;
		UINT         direction_end_stop_id;
		UINT         company_id;
		std::wstring company_name;
		UINT         car_id;
		std::wstring car_number;
		UINT         car_type_id;
		std::wstring car_type_name;
		UINT         driver_id;
		time_t       leave_date;
		bool         is_rtrn;
		bool         from_country;
		UINT         link_id;
		bool         is_closed;
		bool         is_actived;
		UINT         created_by;
		UINT         created_branch_id;
		std::wstring created_firstname;
		time_t       created_at;
		bool         company_is_country;
		bool         is_added;
		std::wstring direction_start_stop_name;
		std::wstring direction_end_stop_name;
		UINT         sitcount;
		std::wstring driver_name;
		std::wstring model_name;
		std::wstring register_number;
		UINT         aimag_id;
		std::wstring phone;
		
		bool operator < (const RawDispatcher dispatcher) const;
	};

	class Dispatchers : public XML
	{
	public:
		Dispatchers();
		~Dispatchers();

		void ParseXML(const pugi::xml_document *);
		bool IsEmpty();
		void Reset();
		void Sort();

		std::vector<RawDispatcher> *raw;
	};
	
	class DateDispatcher : public CHoverBitmapWindow
	{
	public:
		DateDispatcher();

		RawDispatcher inf;
		Autobus       bus;

	protected:
		static LPVOID CALLBACK OnMouseDown(HWND hWnd, UINT mButton, WPARAM sState, int X, int Y);
		static LPVOID CALLBACK OnPaint(HWND, HDC, PAINTSTRUCT);
	};

	class DatePagination : public CPanel
	{
#define BRUSH_PAGINATION ::CreateSolidBrush(RGB(247, 255, 158))
#define RGB_BTN_FRAME      RGB(72, 140, 175)
	public:
		DatePagination();

		UINT    nPageNumber;
		CStatic status;
		CWnd    right, left;

		bool    InitArrows();

	protected:
		static LPVOID CALLBACK OnCreate(HWND hWnd, LPARAM lParam);
		static LPVOID CALLBACK OnPaintLeft(HWND, HDC, PAINTSTRUCT);
		static LPVOID CALLBACK OnPaintRight(HWND, HDC, PAINTSTRUCT);
		static LPVOID CALLBACK OnClickLeft(HWND hWnd, UINT mButton, WPARAM sState, int X, int Y);
		static LPVOID CALLBACK OnClickRight(HWND hWnd, UINT mButton, WPARAM sState, int X, int Y);
	};

	class TimeTable : public CPanel
	{
#define MAX_DATES_IN_PAGE 6
	public:
		TimeTable();
		~TimeTable();

		DateDispatcher *ddates;
		UINT            stopID;
		DatePagination  pagination;
		Dispatchers     dispatcher;
		
		void            CreateChilds();
		void            HideDates();
		void            Do(UINT);
		void            DatePagination(UINT);
		std::wstring    GetByDays(UINT days = 0);
		static void     GetDispatchers(LPVOID);
	};
}
