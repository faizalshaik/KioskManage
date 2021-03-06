#pragma once

#include "station.h"

using namespace globalpay;

namespace transdep
{
	class Map
	{
	public:
		Station *stations;
		POINT    center;
		RECT     display;

		Map();
		~Map();
		
		void AttachTo(const HWND hWnd);
		HWND GetContainer();
		void ReserveStations(UINT);
		void ClearStations();
		UINT Width();
		UINT Height();
		UINT GetStationCount();

	private:
		HWND  container;
		UINT  nStations;
	};
}