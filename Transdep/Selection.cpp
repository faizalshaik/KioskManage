#include "../interface/worker.h"

#include "selection.h"

using namespace globalpay;

Selection::Selection()
{
}

LPVOID Selection::Painter(HWND hWnd, HDC hDC, PAINTSTRUCT pStruct)
{
	CWorker* worker = reinterpret_cast<CWorker*>(GetUserDataLong(hWnd));
	if (worker) {

		//::SendMessage(OwnerWnd, WM_ERASEBKGND, NULL, NULL);

		//RECT rect = selectionScreen->GetClientRect();
		HPEN hpenOld = static_cast<HPEN>(::SelectObject(hDC, ::GetStockObject(DC_PEN)));
		HBRUSH hbrushOld = static_cast<HBRUSH>(::SelectObject(hDC, ::GetStockObject(NULL_BRUSH)));

		::SetTextColor(hDC, RGB(31, 42, 74));

		HFONT fontStatic = CreateFont(35, 0, 0, 0, 800, false, false, false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH, DEF_FONT_FACE);

		// select the font to restore it after we are done.
		HFONT hFontOld = (HFONT)SelectObject(hDC, fontStatic);
		::SetBkMode(hDC, TRANSPARENT);

		// Origin label
		RECT textRect = coord::to_rect(175, 275, 175, 50);

		//from label
		::DrawText(hDC, std::wstring( L"FROM"/*TEXT(" ХААНААС ")*/).c_str(), -1, &textRect, DT_SINGLELINE | DT_VCENTER | DT_LEFT);

		// Destination label
		textRect = coord::to_rect(175, 400, 175, 50);
		::DrawText(hDC, std::wstring(L"TO" /*TEXT(" ХААШАА ")*/).c_str(), -1, &textRect, DT_SINGLELINE | DT_VCENTER | DT_LEFT);

		// Date label
		textRect = coord::to_rect(175, 550, 175, 100);
		::DrawText(hDC, std::wstring( L"When"/*TEXT(" ЗОРЧИХ \n\r ОГНОО ")*/ ).c_str(), -1, &textRect, DT_VCENTER | DT_LEFT);

		// Number of passengers label
		textRect = coord::to_rect(175, 712, 200, 100);
		::DrawText(hDC, std::wstring(L"passenger number" /*TEXT(" Зорчигчдын \n\r тоо")*/).c_str(), -1, &textRect, DT_VCENTER | DT_LEFT);

		// Draw the boxes around the choices
		drawBox(hWnd, hDC, pStruct, 360, 255, 400, 80);
		drawBox(hWnd, hDC, pStruct, 360, 380, 400, 80);

		// OLD NUMBER OF PASSENGER CHOOSER CODE
		/*drawBox(OwnerWnd, hDC, pStruct, 370, 700, 80, 80);
		drawBox(OwnerWnd, hDC, pStruct, 595, 700, 80, 80);*/
		// put back the old font

		fontStatic = CreateFont(75, 0, 0, 0, 450, false, false, false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH, DEF_FONT_FACE);
		SelectObject(hDC, fontStatic);

		// Get the date from the worker and display it here
		// Also use the same font for the number of passengers
		HFONT fontDynamic = CreateFont(65, 0, 0, 0, 600, false, false, false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH, DEF_FONT_FACE);
		::SetTextColor(hDC, BLACK_PEN);
		SelectObject(hDC, fontDynamic);
		textRect = coord::to_rect(360, 550, 400, 80);
		::DrawText(hDC, L"ORIGIN", -1, &textRect, DT_VCENTER | DT_CENTER);

		// OLD NUMBER OF PASSENGER CHOOSER CODE
		//// Number of Adult passengers 
		//textRect = coord::to_rect(370, 700, 80, 80);
		//::DrawText(hDC, std::to_wstring(selectionScreen->itinerary.adult).c_str(), -1, &textRect, DT_VCENTER|DT_CENTER|DT_SINGLELINE);
		//// Number of Child passengers
		//textRect = coord::to_rect(595, 700, 80, 80);
		//::DrawText(hDC, std::to_wstring(selectionScreen->itinerary.child).c_str(), -1, &textRect, DT_VCENTER|DT_CENTER|DT_SINGLELINE);


		// Change the font for a slightly smaller one for origin and destination
		fontDynamic = CreateFont(42, 0, 0, 0, 600, false, false, false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH, DEF_FONT_FACE);
		SelectObject(hDC, fontDynamic);

		// Get the origin from worker and display it here
		textRect = coord::to_rect(360, 255, 400, 80);
		::DrawText(hDC, L"DATE", -1, &textRect, DT_VCENTER | DT_CENTER | DT_SINGLELINE);

		// Get the destination from worker and display it here
		textRect = coord::to_rect(360, 380, 400, 80);
		::DrawText(hDC, L"DATE", -1, &textRect, DT_VCENTER | DT_CENTER | DT_SINGLELINE);

		// Draw the description text
		// Use a much smaller font with a different color.
		fontStatic = CreateFont(22, 0, 0, 0, 200, false, false, false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH, DEF_FONT_FACE);
		::SelectObject(hDC, fontStatic);
		::SetTextColor(hDC, RGB(83, 83, 83));
		textRect = coord::to_rect(360, 470, 400, 40);
		::DrawText(hDC, std::wstring(TEXT(" ЗОРЧИХ ЧИГЛЭЛЭЭ СОНГОН УУ")).c_str(), -1, &textRect, DT_VCENTER | DT_CENTER);
		fontStatic = CreateFont(30, 0, 0, 0, 200, false, false, false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH, DEF_FONT_FACE);

		// OLD NUMBER OF PASSENGER CHOOSER CODE
		::SelectObject(hDC, fontStatic);
		textRect = coord::to_rect(300, 710, 400, 80);
		::DrawText(hDC, std::wstring(TEXT(" ТОМ \n\rХҮН")).c_str(), -1, &textRect, DT_VCENTER | DT_CENTER);
		textRect = coord::to_rect(550, 730, 400, 40);
		::DrawText(hDC, std::wstring(TEXT(" ХҮҮХЭД")).c_str(), -1, &textRect, DT_VCENTER | DT_CENTER);

		// put back the old font 
		SelectObject(hDC, hFontOld);

	}
	return 0L;
}

void Selection::drawBox(HWND OwnerWnd, HDC hDC, PAINTSTRUCT pStruct, int xStart, int yStart, int width, int height) {
	HPEN hPen = CreatePen(PS_SOLID, 3, BLACK_PEN);

	::MoveToEx(hDC, xStart, yStart, NULL);
	::LineTo(hDC, xStart + width, yStart);
	::LineTo(hDC, xStart + width, yStart + height);
	::LineTo(hDC, xStart, yStart + height);
	::LineTo(hDC, xStart, yStart);

}