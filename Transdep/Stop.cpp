#include "stop.h"

using namespace transdep;

Stop::Stop()
	: BaseName()
{
}

Stop::Stop(const UINT id, const std::wstring name, const double stop_lat, const double stop_lon, const UINT aimag_id)
	: BaseName(id, name)
{
	Latitude  = stop_lat;
	Longitude = stop_lon;
	AimagID   = aimag_id;
}

const std::wstring Stop::getClean()
{
	std::size_t  found = Name.find(TEXT("."));
	std::wstring Clean;

	if (found == std::string::npos)
		Clean = Name;
	else
	{
		Clean = Name.substr(found + (std::size_t) 1);
		Clean.erase(0, Clean.find_first_not_of(TEXT(" ")));
	}

	if (Clean.length() > 0)
	{
		std::locale loc;
//		for (std::string::size_type i = 0; i < Clean.length(); ++i)
//			Clean[i] = std::toupper(Clean[i], loc);
	}

	if (AimagID == 22)
	{
		found = Clean.find(TEXT("УБ "));
		if (found != std::string::npos)
			Clean = Clean.substr(found + (std::size_t) 3);
	}

	return Clean;
}

EndStop::EndStop()
	: Base()
{
	nType = 1;
}

EndStop::EndStop(const UINT id, const UINT direction_id)
	: Base(id)
{
	AttachTariff(direction_id);
}

void EndStop::AttachTariff(const UINT direction_id)
{
	if (std::find(tariff_ids.begin(), tariff_ids.end(), direction_id) == tariff_ids.end())
		tariff_ids.push_back(direction_id);
}

bool EndStop::IsBig()
{
	return nType > 1;
}