/**
* Region helper functions - Region.h
*
* @program		Global-Pay
* @author		АНКУ ХХК
* @copyright	Copyright (c) 2017 он, АНКУ ХХК.
* @version		0.1.0.0
* @date			2017.09.19
*/

#pragma once

#include <windows.h>

namespace globalpay
{
	typedef struct
	{
		static HRGN Rounded(SIZE size, UINT eWidth = 16, UINT eHeight = 0);
		static HRGN Circle(POINT center, UINT Radius = 16);
		static HRGN Triangle(POINT p1, POINT p2, POINT p3);
		static HRGN Bitmap(HBITMAP hBmp, COLORREF cTransparentColor = NULL, COLORREF cTolerance = 0x101010);
	} Rgn;
}