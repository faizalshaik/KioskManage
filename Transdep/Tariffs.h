#pragma once

#include "base.hpp"

namespace transdep
{
	typedef struct : public RawData
	{
		UINT         direction_id;
		bool         direction_UX_eseh;
		std::wstring direction_name;
		UINT         start_stop_id;
		std::wstring start_stop_name;
		UINT         end_stop_id;
		std::wstring end_stop_name;
		bool         is_rtrn;
		UINT         start_stop_sequence;
		UINT         end_stop_sequence;
		UINT         ticket_type_id;
		double       end_stop_lat;
		double       end_stop_lon;
		double       start_stop_lat;
		double       start_stop_lon;
		UINT         aimag_id;
		UINT         end_stop_aimag_id;

		float        big_price;
		float        big_childprice;
		float        big_insurance;
		float        big_childinsurance;
		float        mid_price;
		float        mid_childprice;
		float        mid_insurance;
		float        mid_childinsurance;
		float        lit_price;
		float        lit_childprice;
		float        lit_insurance;
		float        lit_childinsurance;
		float        sit_price;
		float        sit_childprice;
		float        sit_insurance;
		float        sit_childinsurance;

		UINT         start_i_com_id;
		std::wstring start_i_com_name;
		std::wstring start_i_com_reg;
		UINT         end_i_com_id;
		std::wstring end_i_com_name;
		std::wstring end_i_com_reg;

		UINT         big_price_percent;
		UINT         mid_price_percent;
		UINT         lit_price_percent;
		UINT         sit_price_percent;
	} RawTariff;

	class Tariffs : public XML
	{
	public:
		std::vector<RawTariff> *raw;

	public:
		Tariffs();
		~Tariffs();

		void ParseXML(const pugi::xml_document *);
		bool IsEmpty();
		void Reset();
	};

	class Tariff : public Base
	{
	public:
		Tariff();
		Tariff(const UINT);
	};
}