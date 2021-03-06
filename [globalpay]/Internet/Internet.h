/**
* CInternet class - Internet.h
*
* @program		Global-Pay
* @author		АНКУ ХХК
* @copyright	Copyright (c) 2017 он, АНКУ ХХК.
* @version		0.1.0.0
* @date			2017.09.19
*/

#pragma once

#include <windows.h>
#include <wininet.h>
#include <string>

#pragma comment(lib, "wininet.lib")

namespace globalpay {
	class CInternet
	{
	public:
		static const BOOL CheckConnection(LPCTSTR testUrl = TEXT("http://www.gogo.mn"));
		
		static HINTERNET Open(
			LPCTSTR szAgent = TEXT("codesaur::CInternet"),
			DWORD   dwAccessType = INTERNET_OPEN_TYPE_DIRECT,
			LPCTSTR lpszProxyName = NULL,
			LPCTSTR lpszProxyBypass = NULL,
			DWORD   dwFlags = 0 /* INTERNET_FLAG_ASYNC */);
		static HINTERNET Connect(
			HINTERNET     hIntSession,
			LPCTSTR       szDomain,
			INTERNET_PORT nServerPort = INTERNET_DEFAULT_HTTP_PORT,
			LPCTSTR       lpszUsername = NULL,
			LPCTSTR       lpszPassword = NULL,
			DWORD         dwService = INTERNET_SERVICE_HTTP,
			DWORD         dwFlags = 0,
			DWORD_PTR     dwContext = NULL);
		static void Close(HINTERNET hInternet);

		static HINTERNET OpenRequest(
			HINTERNET hHttpSession,
			LPCTSTR   szMethod = TEXT("GET"),
			LPCTSTR   szObject = TEXT(""),
			LPCTSTR   lpszVersion = NULL, 
			LPCTSTR   lpszReferer = NULL,
			LPCTSTR  *lplpszAcceptTypes = 0,
			DWORD     dwFlags = INTERNET_FLAG_RELOAD,
			DWORD_PTR dwContext = 0);
		static BOOL SendRequest(
			HINTERNET HttpRequest,
			LPCTSTR   szHeaders = TEXT("Content-Type: text/html"),
			DWORD     dwHeadersLength = 0,
			LPVOID    lpOptional = NULL,
			DWORD     dwOptionalLength = 0);

		static const std::wstring RequestXML(
			LPCTSTR szDomain,
			LPCTSTR szObject);

		static const std::wstring PostData(
			LPCTSTR       szDomain,
			LPCTSTR       szObject,
			LPCTSTR       szHeaders,
			DWORD         dwHeadersLength,
			LPVOID        szData,
			DWORD         dwDataLength,
			INTERNET_PORT nServerPort = INTERNET_DEFAULT_HTTP_PORT,
			LPCTSTR      *lplpszAcceptTypes = NULL,
			DWORD         dwRequestFlags = 0);


	protected:
		static const std::wstring Request(
			LPCTSTR       szDomain,
			LPCTSTR       szObject,
			LPCTSTR       szMethod = TEXT("GET"),
			LPCTSTR       szHeaders = TEXT("Content-Type: text/html"),
			DWORD         dwHeadersLength = 0,
			LPVOID        lpOptional = NULL,
			DWORD         dwOptionalLength = 0,
			LPCTSTR      *lplpszAcceptTypes = 0,
			LPCTSTR       szAgent = TEXT("codesaur::CInternet"),
			DWORD         dwAccessType = INTERNET_OPEN_TYPE_DIRECT,
			LPCTSTR       lpszProxyName = NULL,
			LPCTSTR       lpszProxyBypass = NULL,
			DWORD         dwFlags = 0 /* INTERNET_FLAG_ASYNC */,
			INTERNET_PORT nServerPort = INTERNET_DEFAULT_HTTP_PORT,
			LPCTSTR       lpszUsername = NULL,
			LPCTSTR       lpszPassword = NULL,
			DWORD         dwService = INTERNET_SERVICE_HTTP,
			DWORD         dwConnectFlags = 0,
			DWORD_PTR     dwConnectContext = NULL,
			LPCTSTR       lpszVersion = NULL,
			LPCTSTR       lpszReferer = NULL,
			DWORD         dwRequestFlags = INTERNET_FLAG_RELOAD,
			DWORD_PTR     dwRequestContext = 0);

		static const std::wstring GetResponse(HINTERNET hInternet);
	};
}