/**
* Msg.cpp
*
* @program		Global-Pay
* @author		АНКУ ХХК
* @copyright	Copyright (c) 2017 он, АНКУ ХХК.
* @version		0.1.0.0
* @date			2017.09.19
*/

#include "xml.h"

using namespace globalpay;

void XML::LoadFromBuffer(pugi::xml_document *doc, const std::wstring szBuffer)
{
	pugi::xml_parse_result result = doc->load_string(szBuffer.c_str());

	if (!result)
	{
		//				OutputDebugStringA("LoadXMLBuffer error!");
	}
}

void XML::LoadFromFile(pugi::xml_document *doc, const std::wstring szFileName)
{
	pugi::xml_parse_result result = doc->load_file(szFileName.c_str());

	if (!result)
	{
		//				OutputDebugStringA("LoadXMLFile error!");
	}
}