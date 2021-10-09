#pragma once

#include <windows.h>

#include "types/string.h"

namespace helper {
#define RAW_EMPTY_VALUE 0
	typedef struct
	{
		UINT id;

		static void SetInt(UINT &field, std::wstring data)
		{
			if (data == TEXT(""))
			{
				field = RAW_EMPTY_VALUE;
				return;
			}

			try
			{
				field = string::wto_int(data);
			}
			catch (...)
			{
				field = RAW_EMPTY_VALUE;
			}
		}

		static void SetFloat(float &field, std::wstring data)
		{
			if (data == TEXT(""))
			{
				field = RAW_EMPTY_VALUE;
				return;
			}

			try
			{
				field = string::wto_float(data);
			}
			catch (...)
			{
				field = RAW_EMPTY_VALUE;
			}
		}

		static void SetDouble(double &field, std::wstring data)
		{
			if (data == TEXT(""))
			{
				field = RAW_EMPTY_VALUE;
				return;
			}

			try
			{
				field = string::wto_float(data);
			}
			catch (...)
			{
				field = RAW_EMPTY_VALUE;
			}
		}

		static void SetBool(bool &field, std::wstring data)
		{
			if (data == TEXT(""))
			{
				field = false;
				return;
			}

			try
			{
				field = string::wto_bool(data);
			}
			catch (...)
			{
				field = false;
			}
		}

		static void SetTime(time_t &field, std::wstring data)
		{
			if (data == TEXT(""))
			{
				field = RAW_EMPTY_VALUE;
				return;
			}

			try
			{
				field = RAW_EMPTY_VALUE;
			}
			catch (...)
			{
				field = RAW_EMPTY_VALUE;
			}
		}

		static void SetTimeTransdep(time_t &field, std::wstring data)
		{
			if (data == TEXT(""))
			{
				field = RAW_EMPTY_VALUE;
				return;
			}

			try
			{
				struct tm timeinfo;

				std::time(&field);
				::localtime_s(&timeinfo, &field);
				timeinfo.tm_year = string::wto_int(data.substr(0, 4)) - 1900;
				timeinfo.tm_mon  = string::wto_int(data.substr(5, 2)) - 1;
				timeinfo.tm_mday = string::wto_int(data.substr(8, 2));
				timeinfo.tm_hour = string::wto_int(data.substr(11, 2));
				timeinfo.tm_min  = string::wto_int(data.substr(14, 2));
				timeinfo.tm_sec  = string::wto_int(data.substr(17, 2));

				field            = std::mktime(&timeinfo);
			}
			catch (...)
			{
				field = RAW_EMPTY_VALUE;
			}
		}
	} RawData;
}