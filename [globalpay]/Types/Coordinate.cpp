/**
* Coordinate.cpp
*
* @program		Global-Pay
* @author		АНКУ ХХК
* @copyright	Copyright (c) 2017 он, АНКУ ХХК.
* @version		0.1.0.0
* @date			2017.09.19
*/

#include "coordinate.h"

using namespace globalpay;

const POINT coord::point(LONG x, LONG y)
{
	POINT ret;
	ret.x = x;
	ret.y = y;
	return ret;
}

const SIZE coord::size(LONG Width, LONG Height)
{
	SIZE size;
	size.cx = Width;
	size.cy = Height;
	return size;
}

const RECT coord::rect(int left, int top, int right, int bottom)
{
	RECT rc;
	rc.left = left;
	rc.right = right;
	rc.top = top;
	rc.bottom = bottom;
	return rc;
}

const RECT coord::rect(POINT lefttop, POINT rightbottom)
{
	return coord::rect(lefttop.x, lefttop.y, rightbottom.x, rightbottom.y);
}

const RECT coord::rect(int x, int y, SIZE size)
{
	return coord::to_rect(x, y, size.cx, size.cy);
}

const RECT coord::rect(POINT p, SIZE size)
{
	return coord::to_rect(p.x, p.y, size.cx, size.cy);
}

const RECT coord::to_rect(int x, int y, int cx, int cy)
{
	return coord::rect(x, y, x + cx, y + cy);
}

const LONG coord::height(RECT rect)
{
	return rect.bottom - rect.top;
}

const LONG coord::width(RECT rect)
{
	return rect.right - rect.left;
}

BOOL coord::is_in_rect(POINT p, RECT rcRect)
{
	return (p.x >= rcRect.left && p.x <= rcRect.right) && (p.y >= rcRect.top && p.y <= rcRect.bottom);
}