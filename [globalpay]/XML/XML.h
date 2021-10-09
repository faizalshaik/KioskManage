#pragma once

#include "[vendor]/pugixml/pugixml.hpp"

namespace globalpay {
	class XML
	{
	public:
		static void LoadFromBuffer(pugi::xml_document *, const std::wstring);
		static void LoadFromFile(pugi::xml_document *, const std::wstring);
	};
}