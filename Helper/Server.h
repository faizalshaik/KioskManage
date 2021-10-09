#pragma once

#include "globalpay.hpp"

#include "internet/internet.h"

using namespace globalpay;

#define HTTP_FORM_HEADER TEXT("Content-Type: application/x-www-form-urlencoded; charset=utf-8\r\n")

namespace helper {
	class Server
	{
	public:
		static std::wstring Post(LPCTSTR, std::string);
	};
}