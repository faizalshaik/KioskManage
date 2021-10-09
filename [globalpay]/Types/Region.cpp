/**
* Region.cpp
*
* @program		Global-Pay
* @author		АНКУ ХХК
* @copyright	Copyright (c) 2017 он, АНКУ ХХК.
* @version		0.1.0.0
* @date			2017.09.19
*/

#include "Region.h"

using namespace globalpay;

HRGN Rgn::Rounded(SIZE size, UINT eWidth, UINT eHeight)
{
	return ::CreateRoundRectRgn(0, 0, size.cx, size.cy, eWidth, eHeight == 0 ? eWidth : eHeight);
}

HRGN Rgn::Circle(POINT center, UINT Radius)
{
	return ::CreateEllipticRgn(center.x - Radius, center.y - Radius, center.x + Radius, center.y + Radius);
}

HRGN Rgn::Triangle(POINT p1, POINT p2, POINT p3)
{
	POINT VertexPoint[3];

	VertexPoint[0].x = p1.x; VertexPoint[0].y = p1.y;
	VertexPoint[1].x = p2.x; VertexPoint[1].y = p2.y;
	VertexPoint[2].x = p3.x; VertexPoint[2].y = p3.y;

	return ::CreatePolygonRgn(VertexPoint, 3, ALTERNATE);
}

HRGN Rgn::Bitmap(HBITMAP hBmp, COLORREF cTransparentColor, COLORREF cTolerance)
{
	HRGN hRgn = NULL;
	if (hBmp)
	{
		HDC hMemDC = ::CreateCompatibleDC(NULL);
		if (hMemDC)
		{
			BITMAP bm;
			::GetObject(hBmp, sizeof(bm), &bm);

			BITMAPINFOHEADER RGB32BITSBITMAPINFO = { sizeof(BITMAPINFOHEADER), bm.bmWidth, bm.bmHeight, 1, 32, BI_RGB, 0, 0, 0, 0, 0 };

			VOID * pbits32;
			HBITMAP hbm32 = ::CreateDIBSection(hMemDC, (BITMAPINFO *)&RGB32BITSBITMAPINFO, DIB_RGB_COLORS, &pbits32, NULL, 0);
			if (hbm32)
			{
				HBITMAP holdBmp = (HBITMAP) ::SelectObject(hMemDC, hbm32);
				HDC hDC = ::CreateCompatibleDC(hMemDC);
				if (hDC)
				{
					BITMAP bm32;
					::GetObject(hbm32, sizeof(bm32), &bm32);
					while (bm32.bmWidthBytes % 4)
						bm32.bmWidthBytes++;

					HBITMAP holdBmp = (HBITMAP) ::SelectObject(hDC, hBmp);
					::BitBlt(hMemDC, 0, 0, bm.bmWidth, bm.bmHeight, hDC, 0, 0, SRCCOPY);

					const short ALLOC_UNIT = 100;
					DWORD maxRects = ALLOC_UNIT;
					HANDLE hData = ::GlobalAlloc(GMEM_MOVEABLE, sizeof(RGNDATAHEADER) + (sizeof(RECT) * maxRects));
					RGNDATA *pData = (RGNDATA *) ::GlobalLock(hData);
					pData->rdh.dwSize = sizeof(RGNDATAHEADER);
					pData->rdh.iType = RDH_RECTANGLES;
					pData->rdh.nCount = pData->rdh.nRgnSize = 0;
					::SetRect(&pData->rdh.rcBound, MAXLONG, MAXLONG, 0, 0);

					BYTE *p32 = (BYTE *)bm32.bmBits + (bm32.bmHeight - 1) * bm32.bmWidthBytes;
					LONG *p = (LONG *)p32;

					if (cTransparentColor == NULL)
						cTransparentColor = RGB(GetRValue(*p), GetGValue(*p), GetBValue(*p));

					BYTE lr = GetRValue(cTransparentColor);
					BYTE lg = GetGValue(cTransparentColor);
					BYTE lb = GetBValue(cTransparentColor);
					BYTE hr = min(0xff, lr + GetRValue(cTolerance));
					BYTE hg = min(0xff, lg + GetGValue(cTolerance));
					BYTE hb = min(0xff, lb + GetBValue(cTolerance));

					for (int y = 0; y < bm.bmHeight; y++)
					{
						for (int x = 0; x < bm.bmWidth; x++)
						{
							int x0 = x;
							p = (LONG *)p32 + x;
							while (x < bm.bmWidth)
							{
								BYTE b = GetRValue(*p);
								if (b >= lr && b <= hr)
								{
									b = GetGValue(*p);
									if (b >= lg && b <= hg)
									{
										b = GetBValue(*p);
										if (b >= lb && b <= hb)
											break;
									}
								}
								p++;
								x++;
							}

							if (x > x0)
							{
								if (pData->rdh.nCount >= maxRects)
								{
									::GlobalUnlock(hData);
									maxRects += ALLOC_UNIT;
									hData = ::GlobalReAlloc(hData, sizeof(RGNDATAHEADER) + (sizeof(RECT) * maxRects), GMEM_MOVEABLE);
									pData = (RGNDATA *) ::GlobalLock(hData);
								}
								RECT *pr = (RECT *)&pData->Buffer;
								::SetRect(&pr[pData->rdh.nCount], x0, y, x, y + 1);
								if (x0 < pData->rdh.rcBound.left)
									pData->rdh.rcBound.left = x0;
								if (y < pData->rdh.rcBound.top)
									pData->rdh.rcBound.top = y;
								if (x > pData->rdh.rcBound.right)
									pData->rdh.rcBound.right = x;
								if (y + 1 > pData->rdh.rcBound.bottom)
									pData->rdh.rcBound.bottom = y + 1;
								pData->rdh.nCount++;

								if (pData->rdh.nCount == 2000)
								{
									HRGN h = ::ExtCreateRegion(NULL, sizeof(RGNDATAHEADER) + (sizeof(RECT) * maxRects), pData);
									if (hRgn)
									{
										::CombineRgn(hRgn, hRgn, h, RGN_OR);
										::DeleteObject(h);
									}
									else
										hRgn = h;
									pData->rdh.nCount = 0;
									::SetRect(&pData->rdh.rcBound, MAXLONG, MAXLONG, 0, 0);
								}
							}
						}
						p32 -= bm32.bmWidthBytes;
					}

					HRGN h = ::ExtCreateRegion(NULL, sizeof(RGNDATAHEADER) + (sizeof(RECT) * maxRects), pData);
					if (hRgn)
					{
						::CombineRgn(hRgn, hRgn, h, RGN_OR);
						::DeleteObject(h);
					}
					else
						hRgn = h;

					::SelectObject(hDC, holdBmp);
					::DeleteDC(hDC);
				}
				::DeleteObject(::SelectObject(hMemDC, holdBmp));
			}
			::DeleteDC(hMemDC);
		}
	}
	return hRgn;
}