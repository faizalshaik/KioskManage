/**
* Coordinate functions - Coordinate.h
*
* @program		Global-Pay
* @author		АНКУ ХХК
* @copyright	Copyright (c) 2017 он, АНКУ ХХК.
* @version		0.1.0.0
* @date			2017.09.19
*/

#pragma once

#include <wtypes.h>

namespace globalpay {
	typedef struct
	{
		static const POINT point(LONG x, LONG y);
		static const SIZE  size(LONG Width, LONG Height);
		static const RECT  rect(int left, int top, int right, int bottom);
		static const RECT  rect(POINT lefttop, POINT rightbottom);
		static const RECT  rect(int x, int y, SIZE size);
		static const RECT  rect(POINT p, SIZE size);
		static const RECT  to_rect(int x, int y, int cx, int cy);
		static const LONG  height(RECT rect);
		static const LONG  width(RECT rect);
		static       BOOL  is_in_rect(POINT p, RECT rcRect);
	} coord;
}