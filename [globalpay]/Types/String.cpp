/**
* String.cpp
*
* @program		Global-Pay
* @author		АНКУ ХХК
* @copyright	Copyright (c) 2017 он, АНКУ ХХК.
* @version		0.1.0.0
* @date			2017.09.19
*/

#include "string.h"

using namespace globalpay;

const int string::copy(LPTSTR szBuffer, LPCTSTR szSrc)
{
	size_t count = ::wcslen(szSrc);
	if (szBuffer) ::free(szBuffer);
	szBuffer = new TCHAR[count * sizeof(TCHAR)];
	errno_t err = ::wcscpy_s(szBuffer, count * sizeof(TCHAR), szSrc);
	return (err == 0) ? count : 0;
}

const int string::ncopy(LPTSTR szBuffer, LPCTSTR szSrc, int nStart, int nEnd, bool bAutoEnd)
{
	if (nStart > nEnd) return 0;

	if (nEnd > (int) ::wcslen(szSrc) - 1)
		if (bAutoEnd)
			nEnd = (int) ::wcslen(szSrc) - 1;
		else
			return 0;

	// szBuffer hangalltai sanah oi awaagui ued aldaa uusne!
	// Ene func daraa guitseeh heregtei
	// szBuffer =  new [nEnd - nStart] geh mayagiin huwiarlalt heregtei

	int nCount = 0;
	for (int i = nStart; i <= nEnd; i++)
	{
		szBuffer[nCount] = szSrc[i];
		nCount++;
	}
	szBuffer[nCount] = '\0';
 	return nCount;
}

const int string::find_first(LPCTSTR szSrc, LPCTSTR szFnd)
{
	int nLengthSrc = (int) ::wcslen(szSrc);
	int nLengthFnd = (int) ::wcslen(szFnd);

	if (isEqual(szSrc, szFnd)) return 0;
	if (nLengthSrc < nLengthFnd) return -1;
	if (!nLengthFnd) return -1;
	
	int nPos = -1;
	int nFndx = 0;
	for (int i = 0; i < nLengthSrc; i++)
	{
		if (szSrc[i] == szFnd[nFndx])
			nFndx++;
		else
			nFndx = 0;

		if (nFndx == nLengthFnd)
		{
			nPos = (i+1) - nLengthFnd;
			break;
		}
	}
	return nPos;
}

const int string::find_last(LPCTSTR szSrc, LPCTSTR szFnd)
{
	int nLengthSrc = (int) ::wcslen(szSrc);
	int nLengthFnd = (int) ::wcslen(szFnd);

	if (isEqual(szSrc, szFnd)) return 0;
	if (nLengthSrc < nLengthFnd) return -1;
	if (!nLengthFnd) return -1;

	int nPos = -1;
	int nFndx = nLengthFnd - 1;
	for (nLengthSrc--; nLengthSrc > 0; nLengthSrc--)
	{
		if (szSrc[nLengthSrc] == szFnd[nFndx])
			nFndx--;
		else
			nFndx = nLengthFnd - 1;

		if (nFndx ==  -1)
		{
			nPos = nLengthSrc;
			break;
		}
	}
	return nPos;
}

BOOL string::append(LPTSTR szBuf, LPCTSTR szAppend)
{
	size_t size = (::wcslen(szBuf) + ::wcslen(szAppend)) * sizeof(TCHAR);
	return (::wcscat_s(szBuf, size * sizeof(TCHAR), szAppend) == 0);
}

void string::wto_multi(LPTSTR lpWideText, char* szMultiBuffer, UINT CodePage, DWORD dwFlags)
{
	int sizeNeeded = ::WideCharToMultiByte(CodePage, dwFlags, lpWideText, -1, NULL, 0, NULL, NULL);
	char* encodedStr = new char[sizeNeeded];
	::WideCharToMultiByte(CodePage, dwFlags, lpWideText, -1, szMultiBuffer, sizeNeeded, NULL, NULL);
}

BOOL string::to_utf16(CHAR *in_Src, WCHAR *out_Dst, INT in_MaxLen)
{
    /* locals */
    INT lv_Len;

  // do NOT decrease maxlen for the eos
  if (in_MaxLen <= 0)
    return FALSE;

  // let windows find out the meaning of ansi
  // - the SrcLen=-1 triggers MBTWC to add a eos to Dst and fails if MaxLen is too small.
  // - if SrcLen is specified then no eos is added
  // - if (SrcLen+1) is specified then the eos IS added
  lv_Len = MultiByteToWideChar(CP_ACP, 0, in_Src, 500, out_Dst, in_MaxLen);

  // validate
  if (lv_Len < 0)
    lv_Len = 0;

  // ensure eos, watch out for a full buffersize
  // - if the buffer is full without an eos then clear the output like MBTWC does
  //   in case of too small outputbuffer
  // - unfortunately there is no way to let MBTWC return shortened strings,
  //   if the outputbuffer is too small then it fails completely
  if (lv_Len < in_MaxLen)
    out_Dst[lv_Len] = 0;
  else if (out_Dst[in_MaxLen-1])
    out_Dst[0] = 0;

  // done
  return TRUE;
}

std::wstring string::utf8_to_utf16(const std::string str)
{
	if (str.empty())
		return std::wstring();

	size_t charsNeeded = ::MultiByteToWideChar(CP_UTF8, 0,
		str.data(), (int)str.size(), NULL, 0);

	std::vector<wchar_t> buffer(charsNeeded);
	int charsConverted = ::MultiByteToWideChar(CP_UTF8, 0,
		str.data(), (int)str.size(), &buffer[0], buffer.size());

	return std::wstring(&buffer[0], charsConverted);
}
/*
std::string string::utf16_to_utf8(const std::wstring wstr)
{
	using convert_typeX = std::codecvt_utf8<wchar_t>;
	std::wstring_convert<convert_typeX, wchar_t> converterX;

	return converterX.to_bytes(wstr);
}
*/
int string::to_int(std::string _xs)
{
	return std::atoi(_xs.c_str());
}

int string::wto_int(std::wstring _xs)
{
	return std::stoi(_xs);
}

float string::to_float(std::string _xs)
{
	return std::stof(_xs);
}

float string::wto_float(std::wstring _xs)
{
	return std::stof(_xs);
}

double string::to_double(std::string _xs)
{
	return std::stod(_xs);
}

double string::wto_double(std::wstring _xs)
{
	return std::stod(_xs);
}

bool string::to_bool(std::string str)
{
	std::transform(str.begin(), str.end(), str.begin(), ::tolower);
	std::istringstream is(str);
	bool b;
	is >> std::boolalpha >> b;
	return b;
}

bool string::wto_bool(std::wstring wstr)
{
	std::transform(wstr.begin(), wstr.end(), wstr.begin(), ::tolower);
	std::wistringstream is(wstr);
	bool b;
	is >> std::boolalpha >> b;
	return b;
}

std::wstring string::split(std::wstring src)
{
	std::wstring s = src;
	std::wstring delimiter = TEXT(".");

	size_t pos = 0;
	std::string token;

	std::wstring last_token = s.substr(s.find(delimiter) + 1);
	return last_token;
}

std::wstring string::split(std::wstring src,std::wstring delim, int index = 0)
{
	std::wstring s = src;
	std::wstring delimiter = delim;
	
	std::string token;
	
	std::wstring last_token = index != 0 ? s.substr(s.find(delimiter) + index) : s.substr(index, s.find(delimiter));
	return last_token;
}

std::wstring string::s2ws(const std::string& s)
{
	int len;
	int slength = (int) s.length() + 1;
	len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0);
	wchar_t* buf = new wchar_t[len];
	MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
	std::wstring r(buf);
	delete[] buf;
	return r;
}

std::string string::utf8encode(const std::wstring &wstr)
{
	// NOTE: C++11 has built-in support for converting between
	// UTF-8 and UTF-16.  See the std::wstring_convert class...
	/*
	wstring_convert<codecvt_utf8_utf16<wchar_t>> conv;
	return conv.to_bytes(wstr);
	*/

	std::string out;
	int len = ::WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), wstr.length(), NULL, 0, NULL, NULL);
	if (len > 0)
	{
		out.resize(len);
		::WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), wstr.length(), &out[0], len, NULL, NULL);
	}
	return out;
}

std::wstring string::formatdatetime(time_t *datetime, char* dateFormat)
{
	if (datetime == NULL)
	{
		datetime = new time_t;
		*datetime = ::time(0);
	}

	struct tm timeinfo;
	char time_buffer[86];

	::localtime_s(&timeinfo, datetime);	
	::strftime(time_buffer, sizeof(time_buffer), dateFormat, &timeinfo);

	return string::s2ws(time_buffer);
}