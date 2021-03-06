#pragma once

#include "window/wnd.h"

#include "compass.hpp"
#include "stop.h"

using namespace globalpay;

namespace transdep
{
#define START_STATION cfg.IsMongolian() ? TEXT("Улаанбаатар") : TEXT("Ulaanbaatar")

	typedef enum
	{
		StartStation,
		PassStation,
		EndStation
	} StationType;

	typedef enum
	{
		BigStation,
		MidStation,
		SmallStation
	} StationSize;

	class Station : public CWnd
	{
	public:
		Station();

		bool Init(HINSTANCE hInst, HWND hParent, const Stop stop);
		void SetPoint(LONG, LONG);
		bool IsVisible();
		Stop GetInfo();

	protected:
		static LPVOID CALLBACK OnPaint(HWND, HDC, PAINTSTRUCT);
		static LPVOID CALLBACK OnClick(HWND hWnd, UINT mButton, WPARAM sState, int X, int Y);

	public:
		StationType Type;
		StationSize Size;
		UINT        ID;
		POINT       Point;
	};
}