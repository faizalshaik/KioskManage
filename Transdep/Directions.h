#pragma once

#include "stop.h"

namespace transdep
{
	typedef struct : public RawData
	{
		UINT         direction_id;
		bool         direction_UX_eseh;
		std::wstring direction_name;
		
		UINT         stop_id;
		std::wstring stop_name;
		UINT         stop_sequence;
		
		UINT         distance;
		
		time_t       big_move_time;
		time_t       big_stop_time;
		time_t       mid_move_time;
		time_t       mid_stop_time;
		time_t       lit_move_time;
		time_t       lit_stop_time;
		time_t       sit_move_time;
		time_t       sit_stop_time;

		bool         is_motion;
		bool         is_rtrn;

		UINT         aimag_id;
		double       lat;
		double       lon;
	} RawDirection;

	class Direction : public BaseName
	{
	public:
		Direction();
		Direction(const UINT, const std::wstring, const bool);

	public:
		bool isUX;
	};

	class Directions : public XML
	{
	public:
		std::vector<RawDirection> *raw;

	public:
		Directions();
		~Directions();

		void ParseXML(const pugi::xml_document *);
		bool IsEmpty();
		void Reset();
	};
}