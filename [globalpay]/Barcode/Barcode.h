/**
* 1D Barcode classes for Win32
*
* @program		Global-Pay
* @author		АНКУ ХХК
* @copyright	Copyright (c) 2017 он, АНКУ ХХК.
* @version		0.1.0.0
* @date			2017.09.19
*/

#pragma once

#include <stdio.h>
#include <windows.h>

#include "globalpay.hpp"

namespace globalpay {
	typedef struct
	{
		int   ch;
		char *psz;
	} BarcodeIntString;

	typedef struct
	{
		enum
		{
			SETA = 0,
			SETB = 1,
			SETC = 2,
		};
	} SUB128;

	typedef struct
	{
		int iV, iFirst, iSecond;
	} I3forBarcode93;

	class BarcodeBase
	{
	public:
		BarcodeBase();

		void operator = (const BarcodeBase &);

		void        Clear();
		int         GetEncodeLength() const;
		int         GetBufferLength() const;
		const BYTE& GetAt(int) const;
		int         GetRatio() const;
		int         SetRatio(int);
		void        DrawBarcode(HDC, int, int, int, int, const COLORREF, const COLORREF, const int);

	protected:
		BYTE	    ia_Buf[4096];
		int		    i_LenBuf;
		int		    i_Ratio;
	};

	class Barcode39 : public BarcodeBase
	{
	public:
		Barcode39();
		~Barcode39();

		BOOL  Encode39(const char *);
		void  Draw39(HDC, int, int, int, const COLORREF, const COLORREF, const int);

	private:
		BYTE *P_GetNarrowWideBarSpace39(char, BYTE *);
	};

	class BarcodeI2of5 : public BarcodeBase
	{
	public:
		BarcodeI2of5();
		~BarcodeI2of5();

		BOOL  EncodeI2of5(const char *);
		void  DrawI2of5(HDC, int, int, int, const COLORREF, const COLORREF, const int);

	private:
		BYTE *P_GetNarrorWideBarSpaceI2of5(BYTE *, int);
	};

	class Barcode93 : public BarcodeBase
	{
	public:
		Barcode93();
		~Barcode93();

		BOOL  Encode93(const char *);
		void  Draw93(HDC, int, int, int, const COLORREF, const COLORREF, const int);

	private:
		BYTE *P_GetBarSpace93(BYTE *, int);

	private:
		BYTE *P_GetCheckDigits(BYTE *, const char *&);
		BOOL  P_AscIItoCode93Sequence(int, int &, int &);
	};

	class Barcode128 : public BarcodeBase
	{
	public:
		Barcode128();
		~Barcode128();
		
		BOOL  Encode128A(const char *);
		BOOL  Encode128B(const char *);
		BOOL  Encode128C(const char *);
		void  Draw128(HDC, int, int, int, const COLORREF, const COLORREF, const int);

	private:
		BOOL  P_Encode128(char *, const int);
		BYTE *P_GetBarSpace128(BYTE *, int);

	private:
		int   GetCheckDigit(const int, char *);
	};

	class BarcodeEan13 : public BarcodeBase
	{
	public:
		BarcodeEan13();
		~BarcodeEan13();
		
		BOOL  EncodeEan13(const char *);
		void  DrawEan13(HDC, int, int, int, int, const COLORREF, const COLORREF, const int);

	private:
		BOOL  P_GetCountryCode(char, BYTE *);
		BYTE *P_GetLeftOddParity(BYTE *, char);
		BYTE *P_GetLeftEvenParity(BYTE *, char);
		BYTE *P_GetRightPattern(BYTE *, char);
		char  P_GetCheckSumDigit(const char *);
	};

	const int ga2_Code128[2][207] =
	{
		{
			64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79,
			80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95,
			0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
			16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31,
			32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47,
			48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63,
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
			-1, -1, -1, 95, 96, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106,
		},
		{
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
			0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
			16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31,
			32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47,
			48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63,
			64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79,
			80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, -1,
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
			-1, -1, -1, 95, 96, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106,
		},
	};
}