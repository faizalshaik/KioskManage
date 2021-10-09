/**
* QRcode.cpp
*
* @program		Global-Pay
* @author		АНКУ ХХК
* @copyright	Copyright (c) 2017 он, АНКУ ХХК.
* @version		0.1.0.0
* @date			2017.09.19
*/

#include "qrcode.h"

using namespace globalpay;

CQREncoder::CQREncoder()
{
	m_qrData = NULL;
	m_hbm    = NULL;
}

CQREncoder::CQREncoder(LPCSTR data)
{
	m_data   = data;
	m_hbm    = NULL;
	m_qrData = NULL;

	InitializeQR(0, QR_ECLEVEL_L, QR_MODE_8, 0);
}

CQREncoder::CQREncoder(const std::string &data)
{
	m_data   = data;
	m_hbm    = NULL;
	m_qrData = NULL;

	InitializeQR(0, QR_ECLEVEL_L, QR_MODE_8, 0);
}

CQREncoder::~CQREncoder()
{
	Cleanup();
}

void CQREncoder::Cleanup()
{
	if (m_qrData)
	{
		QRcode_free(m_qrData);
		m_qrData = NULL;
	}
	SafeDeleteObject(m_hbm);
}

bool CQREncoder::InitializeQR(int version, QRecLevel level, QRencodeMode hint, int casesensitive)
{
	Cleanup();

	if (!m_data.empty())
		m_qrData = QRcode_encodeString(m_data.data(), version, level, hint, casesensitive);

	return NULL != m_qrData;
}

bool CQREncoder::InitializeBitmap(HDC hdc)
{
	SafeDeleteObject(m_hbm);
	if (m_qrData)
	{
		m_hbm = ::CreateCompatibleBitmap(hdc, m_qrData->width, m_qrData->width);
		SetBitmapBits(hdc);
	}
	return NULL != m_hbm;
}

void CQREncoder::SetBitmapBits(HDC hdc)
{
	if (m_hbm && m_qrData)
	{
		BITMAP bm;
		BITMAPINFO bmInfo;

		if (GetObject(m_hbm, sizeof(bm), &bm))
		{
			const int bytesPerPixel = bm.bmBitsPixel / 8;
			CBitmapBits bmBits(bm, m_qrData->width, m_qrData->width);

			if (bm.bmBitsPixel % 8)
				return;

			for (int x = 0; x < m_qrData->width; x++)
			{
				for (int scanLine = 0; scanLine < m_qrData->width; scanLine++)
				{
					const int y = bm.bmHeight - scanLine - 1;

					if (0 == (m_qrData->data[y * m_qrData->width + x] & 0x1))
					{
						for (int ixColorByte = 0; ixColorByte < bytesPerPixel; ixColorByte++)
							bmBits[scanLine * bm.bmWidthBytes + x * (bytesPerPixel)+ixColorByte] = 0xFF;
					}
				}
			}
			memset(&bmInfo, 0, sizeof(bmInfo));
			bmInfo.bmiHeader.biSize = sizeof(bmInfo);
			bmInfo.bmiHeader.biWidth = bm.bmWidth;
			bmInfo.bmiHeader.biHeight = bm.bmHeight;
			bmInfo.bmiHeader.biPlanes = bm.bmPlanes;
			bmInfo.bmiHeader.biBitCount = bm.bmBitsPixel;
			bmInfo.bmiHeader.biCompression = BI_RGB;
			::SetDIBits(hdc, m_hbm, 0, bm.bmHeight, bmBits, &bmInfo, DIB_RGB_COLORS);
		}
	}
}

bool CQREncoder::InitializeData(LPCSTR data, int version, QRecLevel level, QRencodeMode hint, int casesensitive)
{
	if (data)
		m_data = data;
	else
		m_data.erase();

	return InitializeQR(version, level, hint, casesensitive);
}

void CQREncoder::PaintData(HWND hwnd, HDC hdc, const RECT &rc)
{
	if (InitializeBitmap(hdc))
	{
		int cxWnd = rc.right - rc.left;
		int cyWnd = rc.bottom - rc.top;
		HDC hdcMem = ::CreateCompatibleDC(hdc);
		HBITMAP hbmSav = static_cast<HBITMAP>(::SelectObject(hdcMem, m_hbm));

		if (cxWnd > cyWnd)
			cxWnd = cyWnd;
		else
			cyWnd = cxWnd;
		if (hbmSav)
		{
			::StretchBlt(hdc, rc.left, rc.top, cxWnd, cyWnd, hdcMem, 0, 0, m_qrData->width, m_qrData->width, SRCCOPY);
			::SelectObject(hdc, hbmSav);
		}
		if (hdcMem)
			::DeleteDC(hdcMem);
	}
}

bool CQREncoder::SaveData(const char* FileName, COLORREF Color, int nPixelPrescaler)
{
	if (m_qrData)
	{
		unsigned int   x, y,
				       nWidth, nWidthAdjusted, nDataBytes;
		int            l, n;
		unsigned char *pRGBData,
			          *pSourceData, *pDestData;
		FILE          *f;

		nWidth             = m_qrData->width;
		nWidthAdjusted     = nWidth * nPixelPrescaler * 3;
		if (nWidthAdjusted % 4)
			nWidthAdjusted = (nWidthAdjusted / 4 + 1) * 4;
		nDataBytes         = nWidthAdjusted * nWidth * nPixelPrescaler;

		if (!(pRGBData = (unsigned char*)::malloc(nDataBytes)))
			return false;

		::memset(pRGBData, 0xff, nDataBytes);

		BITMAPFILEHEADER kFileHeader;
		kFileHeader.bfType          = 0x4d42;
		kFileHeader.bfSize          = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + nDataBytes;
		kFileHeader.bfReserved1     = 0;
		kFileHeader.bfReserved2     = 0;
		kFileHeader.bfOffBits       = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

		BITMAPINFOHEADER kInfoHeader;
		kInfoHeader.biSize          = sizeof(BITMAPINFOHEADER);
		kInfoHeader.biWidth         = nWidth * nPixelPrescaler;
		kInfoHeader.biHeight        = -((int)nWidth * nPixelPrescaler);
		kInfoHeader.biPlanes        = 1;
		kInfoHeader.biBitCount      = 24;
		kInfoHeader.biCompression   = BI_RGB;
		kInfoHeader.biSizeImage     = 0;
		kInfoHeader.biXPelsPerMeter = 0;
		kInfoHeader.biYPelsPerMeter = 0;
		kInfoHeader.biClrUsed       = 0;
		kInfoHeader.biClrImportant  = 0;

		pSourceData = m_qrData->data;
		for (y = 0; y < nWidth; y++)
		{
			pDestData = pRGBData + nWidthAdjusted * y * nPixelPrescaler;
			for (x = 0; x < nWidth; x++)
			{
				if (*pSourceData & 1)
				{
					for (l = 0; l < nPixelPrescaler; l++)
					{
						for (n = 0; n < nPixelPrescaler; n++)
						{
							*(pDestData + n * 3 + nWidthAdjusted * l)     = GetBValue(Color);
							*(pDestData + 1 + n * 3 + nWidthAdjusted * l) = GetGValue(Color);
							*(pDestData + 2 + n * 3 + nWidthAdjusted * l) = GetRValue(Color);
						}
					}
				}
				pDestData += 3 * nPixelPrescaler;
				pSourceData++;
			}
		}

		if (!(::fopen_s(&f, FileName, "wb")))
		{
			::fwrite(&kFileHeader, sizeof(BITMAPFILEHEADER), 1, f);
			::fwrite(&kInfoHeader, sizeof(BITMAPINFOHEADER), 1, f);
			::fwrite(pRGBData,     sizeof(unsigned char), nDataBytes, f);
			::fclose(f);
		}
		else
			return false;

		::free(pRGBData);
	}
	return false;
}