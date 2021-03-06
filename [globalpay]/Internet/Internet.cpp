/**
* Internet.cpp
*
* @program		Global-Pay
* @author		АНКУ ХХК
* @copyright	Copyright (c) 2017 он, АНКУ ХХК.
* @version		0.1.0.0
* @date			2017.09.19
*/

#include "internet.h"

using namespace globalpay;

const BOOL CInternet::CheckConnection(LPCTSTR testUrl)
{
	return ::InternetCheckConnection(testUrl, FLAG_ICC_FORCE_CONNECTION, 0);
}

HINTERNET CInternet::Open(LPCTSTR szAgent, DWORD dwAccessType, LPCTSTR lpszProxyName, LPCTSTR lpszProxyBypass, DWORD dwFlags)
{
	return ::InternetOpen(szAgent, dwAccessType, lpszProxyName, lpszProxyBypass, dwFlags);
}

void CInternet::Close(HINTERNET hInternet)
{
	if (hInternet)
		::InternetCloseHandle(hInternet);
}

HINTERNET CInternet::Connect(HINTERNET hIntSession, LPCTSTR szDomain, INTERNET_PORT nServerPort,
	LPCTSTR lpszUsername, LPCTSTR lpszPassword, DWORD dwService, DWORD dwFlags, DWORD_PTR dwContext)
{
	if (!hIntSession)
		return NULL;

	return ::InternetConnect(hIntSession, szDomain, nServerPort, lpszUsername, lpszPassword, dwService, dwFlags, dwContext);
}

HINTERNET CInternet::OpenRequest(HINTERNET hHttpSession, LPCTSTR szMethod, LPCTSTR szObject,
	LPCTSTR lpszVersion, LPCTSTR lpszReferer, LPCTSTR *lplpszAcceptTypes, DWORD dwFlags, DWORD_PTR dwContext)
{
	if (!hHttpSession)
		return NULL;

	return ::HttpOpenRequest(hHttpSession, szMethod, szObject, lpszVersion, lpszReferer, lplpszAcceptTypes, dwFlags, dwContext);
}

BOOL CInternet::SendRequest(HINTERNET hHttpRequest, LPCTSTR szHeaders, DWORD dwHeadersLength, LPVOID lpOptional, DWORD dwOptionalLength)
{
	if (!hHttpRequest)
		return FALSE;

	return ::HttpSendRequest(hHttpRequest, szHeaders, (dwHeadersLength == 0) ? ::wcslen(szHeaders) : dwHeadersLength, (LPVOID)lpOptional, dwOptionalLength);
}

const std::wstring CInternet::RequestXML(LPCTSTR szDomain, LPCTSTR szObject)
{
	return Request(szDomain, szObject, TEXT("GET"), TEXT("Content-Type: text/xml, application/xml"));
}

const std::wstring CInternet::PostData(
	LPCTSTR       szDomain,
	LPCTSTR       szObject,
	LPCTSTR       szHeaders,
	DWORD         dwHeadersLength,
	LPVOID        szData,
	DWORD         dwDataLength,
	INTERNET_PORT nServerPort,
	LPCTSTR      *lplpszAcceptTypes,
	DWORD         dwRequestFlags)
{
	if (lplpszAcceptTypes == NULL)
	{
		lplpszAcceptTypes    = new LPCTSTR[2];
		lplpszAcceptTypes[0] = TEXT("*/*");
		lplpszAcceptTypes[1] = NULL;
	}

	HINTERNET hIntSession  = CInternet::Open(TEXT("codesaur::CInternet"), INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
	HINTERNET hHttpSession = CInternet::Connect(hIntSession, szDomain, nServerPort, NULL, NULL, INTERNET_SERVICE_HTTP, 0, 1);
	HINTERNET hHttpRequest = CInternet::OpenRequest(hHttpSession, TEXT("POST"), szObject, NULL, NULL, lplpszAcceptTypes, dwRequestFlags, 1);

	if (!CInternet::SendRequest(hHttpRequest, szHeaders, (dwHeadersLength == 0) ? ::wcslen(szHeaders) : dwHeadersLength, szData, dwDataLength))
		return TEXT(""); 

	std::wstring Response = CInternet::GetResponse(hHttpRequest);

	CInternet::Close(hHttpRequest);
	CInternet::Close(hHttpSession);
	CInternet::Close(hIntSession);

	return Response;
}

const std::wstring CInternet::Request(
	LPCTSTR       szDomain,
	LPCTSTR       szObject,
	LPCTSTR       szMethod,
	LPCTSTR       szHeaders,
	DWORD         dwHeadersLength,
	LPVOID        lpOptional,
	DWORD         dwOptionalLength,
	LPCTSTR      *lplpszAcceptTypes,
	LPCTSTR       szAgent,
	DWORD         dwAccessType,
	LPCTSTR       lpszProxyName,
	LPCTSTR       lpszProxyBypass,
	DWORD         dwFlags,
	INTERNET_PORT nServerPort,
	LPCTSTR       lpszUsername,
	LPCTSTR       lpszPassword,
	DWORD         dwService,
	DWORD         dwConnectFlags,
	DWORD_PTR     dwConnectContext,
	LPCTSTR       lpszVersion,
	LPCTSTR       lpszReferer,
	DWORD         dwRequestFlags,
	DWORD_PTR     dwRequestContext)
{
	HINTERNET hIntSession  = CInternet::Open(szAgent, dwAccessType, lpszProxyName, lpszProxyBypass, dwFlags);
	HINTERNET hHttpSession = CInternet::Connect(hIntSession, szDomain, nServerPort, lpszUsername, lpszPassword, dwService, dwConnectFlags, dwConnectContext);
	HINTERNET hHttpRequest = CInternet::OpenRequest(hHttpSession, szMethod, szObject, lpszVersion, lpszReferer, lplpszAcceptTypes, dwRequestFlags, dwRequestContext);

	if (!CInternet::SendRequest(hHttpRequest, szHeaders, dwHeadersLength, lpOptional, dwOptionalLength))
		return TEXT("");

	std::wstring Response = CInternet::GetResponse(hHttpRequest);

	CInternet::Close(hHttpRequest);
	CInternet::Close(hHttpSession);
	CInternet::Close(hIntSession);

	return Response;
}

const std::wstring CInternet::GetResponse(HINTERNET hHttpRequest)
{
	std::wstring szData = TEXT("");
	DWORD        dwRead = 0;
	DWORD        dwBytesCount = 0;
	std::string  szBytes = "";
	char         szBuffer[1024];
	while (::InternetReadFile(hHttpRequest, szBuffer, sizeof(szBuffer) - 1, &dwRead) && dwRead)
	{
		szBuffer[dwRead] = 0;
		dwBytesCount    += dwRead;
		szBytes         += szBuffer;
		dwRead           = 0;
	}

	int nSize = ::MultiByteToWideChar(CP_UTF8, 0, szBytes.c_str(), dwBytesCount + 1, NULL, 0);
	if (nSize)
	{
		LPWSTR lpszText = new WCHAR[nSize];
		::MultiByteToWideChar(CP_UTF8, 0, szBytes.c_str(), dwBytesCount, lpszText, nSize);
		lpszText[nSize - 1] = TEXT('\0');
		szData = std::wstring(lpszText);
		delete [] lpszText;
	}

	return szData;
}