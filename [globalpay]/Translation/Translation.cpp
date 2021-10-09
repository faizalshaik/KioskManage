#include "translation.h"

using namespace globalpay;

Translation::Translation()
{
}

Translation::~Translation()
{
}

Content Translation::CreateContent(DWORD dwLanguage, LPCTSTR szTitle)
{
	Content content;
	content.code  = dwLanguage;
	content.title = szTitle;
	return content;
}