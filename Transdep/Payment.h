#pragma once

#include "control/picturepanel.h"
#include "control/panel.h"
#include "control/button.h"
#include "control/gif.h"
#include "system/thread.h"
#include "system/tools.h"

#include "../interface/footer.h"
#include "../helper/databank.h"
#include "../config.hpp"

using namespace globalpay;

namespace transdep {
	class TicketPreview : public CPicturePanel
	{
	public:
		TicketPreview();

		UINT ID;

	protected:
		static LPVOID CALLBACK OnPaint(HWND OwnerWnd, HDC hDC, PAINTSTRUCT pStruct);
	};

	class TotalPanel : public CPanel
	{
	public:
		TotalPanel();

		std::wstring desc;
		UINT         Price;
		UINT         Adults, Childs;

	protected:
		static LPVOID CALLBACK OnPaint(HWND OwnerWnd, HDC hDC, PAINTSTRUCT pStruct);
	};

	class Payment : public CPanel
	{
	public:
		Payment();
		~Payment();

		TicketPreview *preview;
		TotalPanel     summary;
		ClientAnimate  swiping;

		BOOL TRANSACTIONED;
		std::wstring LAST_ERRIOR;

		CThread        Transaction;

		bool           ConfirmTickets();
		void           SendTickets();
		void           PrintTickets();

		static void    Databank(LPVOID);
		std::wstring   DatabankDoSaleTransaction();
		std::wstring   DatabankConfirmSaleCompleted(std::wstring, std::wstring, std::wstring, std::wstring);
		std::wstring   DatabankVoidTransaction(std::wstring db_ref_no, std::wstring trace_no);

	protected:
		static LPVOID CALLBACK OnCreate(HWND hWnd, LPARAM lParam);
	};
}