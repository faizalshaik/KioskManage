#pragma once

#define  _USE_MATH_DEFINES

#include <math.h>
#include <time.h>
#include <iomanip>
#include <locale>
#include <vector>

#include "xml/xml.h"

#include "../config.hpp"

#include "../helper/raw.hpp"

#include "compass.hpp"

using namespace globalpay;
using namespace helper;

namespace transdep
{
	struct Base
	{
		UINT ID;

		Base() { }

		Base(const UINT id)
		{
			ID = id;
		}

		bool operator == (const UINT value) const
		{
			return (ID == value);
		}
	};

	struct BaseName : public Base
	{
		std::wstring Name;

		BaseName() : Base() { }

		BaseName(const UINT id, const std::wstring name)
		{
			ID = id;
			Name = name;
		}

		bool operator < (const BaseName bs) const
		{
			return (ID < bs.ID);
		}

		std::wstring getClean()
		{
			return Name;
		}
	};

	class GIS
	{
	public:
		static const double GetX(double lon, UINT nScreenWidth)
		{
			return (lon + 180) * (nScreenWidth / 360);
		}

		static const double GetY(double lat, UINT nScreenWidth, UINT nScreenHeight)
		{
			return (nScreenHeight / 2) - ((nScreenWidth * log(tan((M_PI / 4) + (((lat * M_PI) / 180) / 2)))) / (2 * M_PI));
		}

		static const POINT GetAsPoint(double lat, double lon, UINT nScreenWidth, UINT nScreenHeight)
		{
			POINT p;
			p.x = (LONG)GetX(lon, nScreenWidth);
			p.y = (LONG)GetY(lat, nScreenWidth, nScreenHeight);
			return p;
		}

		static const double Deg2Rad(const double degree)
		{
			return degree * (M_PI / 180);
		}

		static const double Rad2Deg(const double radian)
		{
			return radian * (180 / M_PI);
		}

		static const double GetBearing(
			double lat1,
			double lon1,
			double lat2,
			double lon2)
		{
			lat1 = Deg2Rad(lat1);
			lon1 = Deg2Rad(lon1);
			lat2 = Deg2Rad(lat2);
			lon2 = Deg2Rad(lon2);

			double diff_lon = lon2 - lon1;
			double diff_Phi = std::log(std::tan(lat2 / 2.0 + M_PI / 4.0) / std::tan(lat1 / 2.0 + M_PI / 4.0));

			if (std::abs(diff_lon) > M_PI)
			{
				if (diff_lon > 0.0)
					diff_lon = -(2.0 * M_PI - diff_lon);
				else
					diff_lon = (2.0 * M_PI + diff_lon);
			}
			return std::fmod(Rad2Deg(std::atan2(diff_lon, diff_Phi)) + 360.0, 360.0);
		}

		static const POINT CreatePoint(LONG x, LONG y)
		{
			POINT p;
			p.x = x;
			p.y = y;
			return p;
		}
	};
}