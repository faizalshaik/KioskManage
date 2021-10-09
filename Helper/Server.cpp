#include "server.h"

#include "../config.hpp"

using namespace helper;

std::wstring Server::Post(LPCTSTR szObject, std::string szData)
{
	return CInternet::PostData(
		METAKIOSK_SERVER, szObject,
		HTTP_FORM_HEADER, 0,
		(LPVOID)szData.c_str(), szData.length(),
		INTERNET_DEFAULT_HTTPS_PORT,
		NULL,
		INTERNET_FLAG_SECURE);
		//INTERNET_DEFAULT_HTTPS_PORT, NULL, INTERNET_FLAG_SECURE | INTERNET_FLAG_RELOAD);
}