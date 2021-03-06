#pragma once

#include "base.hpp"

namespace transdep
{
	typedef enum
	{
		Rose4Point,
		Rose8Point,
		Rose16Point,
		Rose32Point
	} CompassRoses;

	static UINT Points(const CompassRoses Rose)
	{
		if (Rose == Rose32Point)
			return 32;

		return (Rose == 0) ? 4 : Rose * 8;
	}

	typedef enum
	{
		North,              //0
		North_by_East,      //11.25
		North_Northeast,    //22.50
		Northeast_by_North, //33.75
		Northeast,          //45
		Northeast_by_East,  //56.25
		East_Northeast,     //67.50
		East_by_North,      //78.75

		East,               //90
		East_by_South,      //101.25
		East_Southeast,     //112.50
		Southeast_by_East,  //123.75
		Southeast,          //135
		Southeast_by_South, //146.25
		South_Southeast,    //157.50
		South_by_East,      //168.75

		South,              //180
		South_by_West,      //191.25
		South_Southwest,    //202.50
		Southwest_by_South, //213.75
		Southwest,          //225
		Southwest_by_West,  //236.25
		West_Southwest,     //247.50
		West_by_South,      //258.75

		West,               //270
		West_by_North,      //281.25
		West_Northwest,     //292.50
		Northwest_by_West,  //303.75
		Northwest,          //315
		Northwest_by_North, //326.25
		North_Northwest,    //337.50
		North_by_West       //348.75
	} Compass32Points;

	static Compass32Points GetCompassBearing(double Angle, CompassRoses Rose = Rose32Point)
	{
		double between = 360 / Points(Rose);
		UINT bearing = (UINT)std::floor(Angle / between);

		if ((Angle - bearing * between) > (between / 2))
			bearing++;
		if (bearing > (Points(Rose) - 1))
			bearing = 0;

		if (Rose != Rose32Point)
			bearing = bearing * (32 / Points(Rose));

		return static_cast<Compass32Points>(bearing);
	}

	static POINT GetCompassPoint(POINT center, double Angle, UINT Radius)
	{
		POINT ret;
		ret.x = center.x + (LONG)(Radius * std::cos(Angle));
		ret.y = center.y + (LONG)(Radius * std::sin(Angle));

		return ret;
	}

	static POINT GetCompassRose(POINT center, Compass32Points point, UINT Radius = 100)
	{
		switch (point)
		{
		case North: center.y -= Radius; break;
		case North_by_East: return GetCompassPoint(center, 11.25, Radius);
		case North_Northeast: return GetCompassPoint(center, 22.50, Radius);
		case Northeast_by_North: return GetCompassPoint(center, 33.75, Radius);
		case Northeast: return GetCompassPoint(center, 45, Radius);
		case Northeast_by_East: return GetCompassPoint(center, 56.25, Radius);
		case East_Northeast: return GetCompassPoint(center, 67.50, Radius);
		case East_by_North: return GetCompassPoint(center, 78.75, Radius);
		case East: center.x += Radius; break;
		case East_by_South: return GetCompassPoint(center, 101.25, Radius);
		case East_Southeast: return GetCompassPoint(center, 112.50, Radius);
		case Southeast_by_East: return GetCompassPoint(center, 123.75, Radius);
		case Southeast: return GetCompassPoint(center, 135, Radius);
		case Southeast_by_South: return GetCompassPoint(center, 146.25, Radius);
		case South_Southeast: return GetCompassPoint(center, 157.50, Radius);
		case South_by_East: return GetCompassPoint(center, 168.75, Radius);
		case South: center.y += Radius; break;
		case South_by_West: return GetCompassPoint(center, 191.25, Radius);
		case South_Southwest: return GetCompassPoint(center, 202.50, Radius);
		case Southwest_by_South: return GetCompassPoint(center, 213.75, Radius);
		case Southwest: return GetCompassPoint(center, 225, Radius);
		case Southwest_by_West: return GetCompassPoint(center, 236.25, Radius);
		case West_Southwest: return GetCompassPoint(center, 247.50, Radius);
		case West_by_South: return GetCompassPoint(center, 258.75, Radius);
		case West: center.x -= Radius; break;
		case West_by_North: return GetCompassPoint(center, 281.25, Radius);
		case West_Northwest: return GetCompassPoint(center, 292.50, Radius);
		case Northwest_by_West: return GetCompassPoint(center, 303.75, Radius);
		case Northwest: return GetCompassPoint(center, 315, Radius);
		case Northwest_by_North: return GetCompassPoint(center, 326.25, Radius);
		case North_Northwest: return GetCompassPoint(center, 337.50, Radius);
		case North_by_West: return GetCompassPoint(center, 348.75, Radius);
		}
		return center;
	}
}