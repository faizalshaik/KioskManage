/**
* QRcode.h - QR Barcode encoding class for Win32
*

--- License
https://fukuchi.org/works/qrencode/
Libqrencode is a C library for encoding data in a QR Code symbol,
a kind of 2D symbology that can be scanned by handy terminals such as a mobile phone with CCD.
The capacity of QR Code is up to 7000 digits or 4000 characters, and is highly robust.
This library is characterized by:
* Does not require any additional files at run time.
* Fast symbol encoding.
* Automatic optimization of input data.

libqrencode library is Copyright (C) 2006-2012 Kentaro Fukuchi.
This library is free software; you can redistribute it and/or modify it under the terms of the GNU Lesser General Public License as published by the Free Software Foundation; either version 2.1 of the License, or any later version.
This library is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.
You should have received a copy of the GNU Lesser General Public License along with this library; if not, write to the Free Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA.
QR Code is registered trademarks of DENSO WAVE INCORPORATED in JAPAN and other countries.
Reed-Solomon code encoder is written by Phil Karn, KA9Q.
Copyright (C) 2002, 2003, 2004, 2006 Phil Karn, KA9Q
RPM packages are done by Katsumi Saito.
-----------

--- Credits
Henri Hein      - https://code.msdn.microsoft.com/vstudio/QR-Code-Painter-wFukuchi-13c3bd68
Fulvio Dominici - https://www.codeproject.com/Articles/593591/Simple-C-Cplusplus-QRCode-Library

*
* @program		Global-Pay
* @author		АНКУ ХХК
* @copyright	Copyright (c) 2017 он, АНКУ ХХК.
* @version		0.1.0.0
* @date			2017.09.19
*/

#pragma once

#include <assert.h>

#include "[vendor]/qrencode-3.4.4/qrencode.h"

#include "globalpay.hpp"

#include "types/bitmap.h"
#include "types/string.h"

namespace globalpay {
	class CQREncoder
	{
	public:
		CQREncoder();
		CQREncoder(LPCSTR);
		CQREncoder(const std::string &);
		~CQREncoder();
		
		bool InitializeData(LPCSTR, int version = 0, QRecLevel level = QR_ECLEVEL_L, QRencodeMode hint = QR_MODE_8, int casesensitive = 0);
		void PaintData(HWND, HDC, const RECT &);
		bool SaveData(const char*, COLORREF Color = RGB(0, 0, 0xff), int nPixelPrescaler = 8);

	protected:
		bool InitializeQR(int, QRecLevel, QRencodeMode, int);
		bool InitializeBitmap(HDC);
		void SetBitmapBits(HDC);
		void Cleanup();

	private:
		QRcode     *m_qrData;
		std::string m_data;
		HBITMAP     m_hbm;
	};
}