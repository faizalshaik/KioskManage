#pragma once

#include "window/hoverbitmapwnd.h"
#include "types/region.h"

#include "../config.hpp"
#include "control.h"

using namespace globalpay;

#define MAP_WND_CLASS TEXT("KioskMan::MapWindow")

namespace transdep {
	class MapControl : public CBitmapWindow
	{
	public:
		MapControl();
		~MapControl();

		bool Init(HINSTANCE, HWND);

		CBitmapWindow router;
		CBitmapWindow route0;
		CWnd          route[10];
		CWnd          up,
			          right,
					  down,
					  left;

		static LPVOID CALLBACK OnMapMove(HWND hWnd, UINT mButton, WPARAM sState, int X, int Y);

	protected:
		static LPVOID CALLBACK OnPaint(HWND, HDC, PAINTSTRUCT);
	};
}