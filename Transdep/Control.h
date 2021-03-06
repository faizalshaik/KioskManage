#pragma once

#include "internet/internet.h"

#include "../config.hpp"

#include "tariffs.h"
#include "directions.h"
#include "insurancies.h"
#include "selection.h"
#include "route.h"
#include "station.h"
#include "places.hpp"
#include "router.h"
#include "timetable.h"
#include "seatme.h"
#include "ticket.h"
#include "payment.h"

namespace transdep {
	typedef struct
	{
		RawDispatcher dispatcher;

		std::wstring  StartAimag;
		std::wstring  StartStop;

		std::wstring  DirEndAimag;
		std::wstring  DirEndStop;

		std::wstring  EndAimag;
		std::wstring  EndStop;

		std::wstring  StartSequence;
		std::wstring  EndSequence;

		std::wstring  LeaveDate;
		std::wstring  LeaveTime;

		std::wstring  Company;
		std::wstring  Car;
		std::wstring  Register;
		std::wstring  Type;
		std::wstring  Seats;
		std::wstring  Insurance;
	} Passage;

	class TransdepControl
	{
	public:
		Router    router;
		TimeTable times;
		Autobus   Bus;
		Ticket    ticket[TRANSDEP_MAX_RESERV];
		Payment   pay;
		CThread   service;
		Passage   passage;
		Menu      menu;

		UINT      prvnc;
		UINT      stp;
		RawTariff trff;

		TransdepControl();

		bool LoadRaws(bool bReload = false);
		bool RawsLoaded();

		bool SetMaster();
		bool CookIt();
		bool GenerateRoutes();
		bool GenerateStations();

		void OnSelectionScreen();

		void OnRouter();
		void OnDispatcher();
		void OnSeat();
		void OnTicket();
		void OnPayPreview();
		void OnPayProcess();
		void OnPayDatabank();
		
	private:
		// UNTOUCHED RAW DATAS
		Directions             whole; 

	public:
		// MASTER DATAS
		Tariffs                tocountry;
		std::vector<Direction> directions;
		std::vector<Stop>      stops;

		// COOKED DATAS
		std::vector<Tariff>    tariff;
		std::vector<Base>      start;
		std::vector<EndStop>   end;
		std::vector<Insurance> insurance;
		std::vector<Route>     routes;
		std::vector<Aimag>     aimags;

		Direction*             DirectionByID(UINT);
		Tariff*                TariffByID(UINT);
		RawTariff*             RawTariffBy(UINT, UINT, UINT);
		Stop*				   StopByID(UINT);
		EndStop*               EndStopByID(UINT);
		Insurance*             InsuranceByID(UINT);
		Station*               StationByID(UINT);
		Aimag*                 AimagByID(UINT);

		BOOL                   IsStartStop(UINT);
	};
}

extern transdep::TransdepControl Transdep;