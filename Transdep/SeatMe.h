#pragma once

#include "control/picturepanel.h"
#include "internet/internet.h"
#include "types/string.h"

#include "../config.hpp"
#include "../interface/message.h"

using namespace globalpay;

namespace transdep {
	typedef enum
	{
		csCab   = 4, // Суудлын автомашин
		csSmall = 3, // Бага оврын автобус
		csMid   = 2, // Дунд оврын автобус
		csLarge = 1  // Том автобус
	} CarSize;

	typedef enum
	{
		cUnknown,
		cCab4,    // Хувийн машин
		cSmall10, // УАЗ-10
		cSmall11, // Starex-3
		cSmall14, // Bongo-3, FordTranzit-14
		cMid24,   // County-24
		cMid25,   // County-25
		cMid26,   // ЮүТонг-26
		cMid28,   // County-28
		cMid29,   // County-29
		cMid34,   // AeroTown
		cMid36,   // ПАЗ 4234
		cLarge35, // Ютонг-35
		cLarge38, // Ютонг-38
		cLarge40, // Ютонг-40
		cLarge41, // HYUNDAI AERO-41
		cLarge43, // Granbird
		cLarge44, // Granbird 44
		cLarge45, // Aero, Granbird, Daewoo BH115, Uneverce
		cLarge48, // Granbird-48
		cLarge49, // Ютонг-49
		cLarge53, // Zhongtong
		cLarge54, // ЮүТонг-54
	} Car;
	
	typedef enum
	{
		ssDisabledUnavailable,
		ssDisabledAvailable,
		ssDisabledSelected,
		ssAvailable,
		ssUnavailable,
		ssSelected
	} SeatState;

	class Seat : public CHoverBitmapWindow
	{
	public:
		Seat();

		UINT GetFrame();		

		UINT      ID;
		SeatState State;
		
		static void OnSeat(HWND hWnd);

	protected:
		static LPVOID CALLBACK OnPaint(HWND OwnerWnd, HDC hDC, PAINTSTRUCT pStruct);
	};

	class Autobus : public CPicturePanel
	{
	public:
		Autobus();
		~Autobus();


		void  Set(UINT, UINT, UINT);
		POINT GetSeatXY(UINT, UINT*);

		UINT  GetSeats();
		void  SetSeats();
		void  EnableSeats(const std::vector<UINT>);
		Seat *SeatbyID(UINT);
		void  Reserve(UINT, bool Add = true);
		UINT  Reserved();
		void  CancelAll();
		void  SetBus(UINT);
		UINT  GetBus();

		Car   car;
		UINT  dispatcher;
		Seat *seat;
		UINT  reserve[TRANSDEP_MAX_RESERV];
		UINT  busCase;

		static void GetEmptySeats(LPVOID);

	protected:
		CarSize size;
		UINT    seat_count;

		static LPVOID CALLBACK OnPaint(HWND OwnerWnd, HDC hDC, PAINTSTRUCT pStruct);
	};
}