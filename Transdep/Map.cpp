#include "map.h"

using namespace transdep;

Map::Map()
{
	container = NULL;
	display   = coord::to_rect(0, 0, 0, 0);
	center.x  = 0;
	center.y  = 0;

	stations  = NULL;
	nStations = 0;
}

Map::~Map()
{
	ClearStations();
}

void Map::AttachTo(const HWND hWnd)
{
	container = hWnd;
	display   = GetRectWnd(container);
}

HWND Map::GetContainer()
{
	return this->container;
}

void Map::ReserveStations(UINT nCount)
{
	ClearStations();

	stations  = new Station[nCount];
	nStations = nCount;

	if (sizeof(stations) > 0)
		stations[0].Register(CS_DBLCLKS | CS_OWNDC | CS_SAVEBITS, GetInstanceWnd(GetContainer()), NULL, ::LoadCursor(NULL, IDC_HAND));
}

void Map::ClearStations()
{
	if (stations != NULL)
		delete [] stations;
}

UINT Map::Width()
{
	return (display.right - display.left);
}

UINT Map::Height()
{
	return (display.bottom - display.top);
}

UINT Map::GetStationCount()
{
	return nStations;
}