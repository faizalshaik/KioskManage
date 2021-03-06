/**
* String functions - String.h
*
* @program		Global-Pay
* @author		АНКУ ХХК
* @copyright	Copyright (c) 2017 он, АНКУ ХХК.
* @version		0.1.0.0
* @date			2017.09.19
*/

#pragma once

#include <vector>
#include <sstream>
#include <algorithm>

#include <wtypes.h>
#include <string>
#include <codecvt>

namespace globalpay {
#define isEmpty(x)     !::wcscmp(x, TEXT(""))
#define isEqual(a, b)  !::wcscmp(a, b)

	typedef struct
	{
		static const int    copy(LPTSTR, LPCTSTR);
		static const int    ncopy(LPTSTR, LPCTSTR, int, int, bool bAutoEnd = true);
		static BOOL         append(LPTSTR, LPCTSTR);
		static const int    find_first(LPCTSTR, LPCTSTR);
		static const int    find_last(LPCTSTR, LPCTSTR);
		static void         wto_multi(LPTSTR, char*, UINT CodePage = CP_ACP, DWORD dwFlags = 0);
		static BOOL         to_utf16(CHAR *in_Src, WCHAR *out_Dst, INT in_MaxLen);
		static std::wstring utf8_to_utf16(const std::string);
//		static std::string  utf16_to_utf8(const std::wstring);
		static int          to_int(std::string);
		static int          wto_int(std::wstring);
		static float        to_float(std::string);
		static float        wto_float(std::wstring);
		static double       to_double(std::string);
		static double       wto_double(std::wstring);
		static bool	        to_bool(std::string);
		static bool	        wto_bool(std::wstring);
		static std::wstring split(std::wstring);
		static std::wstring split(std::wstring, std::wstring, int);
		static std::wstring s2ws(const std::string&);
		static std::string  utf8encode(const std::wstring &wstr);
		static std::wstring formatdatetime(time_t *datetime = NULL, char* dateFormat = "%Y-%m-%d %H:%M:%S");
	} string;
}

