#pragma once

#include "[vendor]/pugixml/pugixml.hpp"

namespace helper {
	class XML
	{
	public:
		static void LoadFromBuffer(pugi::xml_document *doc, const std::wstring szBuffer)
		{
			pugi::xml_parse_result result = doc->load_string(szBuffer.c_str());

			if (!result)
			{
				::OutputDebugStringA("LoadXMLBuffer error!");
			}
		}

		static void LoadFromFile(pugi::xml_document *doc, const std::wstring szFileName)
		{
			pugi::xml_parse_result result = doc->load_file(szFileName.c_str());

			if (!result)
			{
				::OutputDebugStringA("LoadXMLFile error!");
			}
		}
	};
}