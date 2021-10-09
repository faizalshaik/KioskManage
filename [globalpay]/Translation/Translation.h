#pragma once

#include "globalpay.hpp"

#include "internet/internet.h"
#include "types/string.h"

namespace globalpay {
	typedef struct
	{
		DWORD        code;  
		std::wstring title;
	} Content;

	typedef struct
	{
		std::wstring         index;
		std::vector<Content> content;
	} Translate;

	class Translation
	{
	public:
		Translation();
		~Translation();

		std::vector<Translate> contents;

		Content CreateContent(DWORD, LPCTSTR);
	};
}