#pragma once

#include <windows.h>
#include <string>

#include "types/coordinate.h"
#include "system/file.h"
#include "system/tools.h"

#include "helper/server.h"

#include "version.hpp"
#include "text.h"
#include "stdafx.h"

using namespace globalpay;
using namespace helper;

class Config
{
private:
	UINT         MachineID;

	std::wstring szModuleName;
	std::wstring szCurrentDir;
	std::wstring szDataPath;
	std::wstring szAbsolutePath;
	
	DWORD        dwLanguage;
	RECT         rcPrimary, rcClient, rcWorker;

public:
	Config()
	{
		SetID(3);

		dwLanguage = LANG_MONGOLIAN;

		if (!::IsDebuggerPresent())
			::ShowCursor(true);
		
		SetPrimaryRect();

		key = TEXT("4589");

		WCHAR szBuffer[MAX_PATH];
		if (!::GetModuleFileName(NULL, szBuffer, MAX_PATH))
			szAbsolutePath = TEXT_EMPTY;
		else
		{
			szModuleName = std::wstring(szBuffer);
			szAbsolutePath = szModuleName.substr(0, szModuleName.find_last_of(TEXT("\\/")));
		}
		
		szDataPath = std::wstring(RESOURCE_FOLDER) + DIRECTORY_SEPARATOR;
		szAbsolutePath += DIRECTORY_SEPARATOR;

		if (::GetCurrentDirectory(szBuffer))
		{
			szCurrentDir = std::wstring(szBuffer);
		}
		else
		{
			szCurrentDir = szAbsolutePath;
		}

		version.ReadFrom(szModuleName.c_str());
	}

	void Identify()
	{
		std::wstring ID;
		ID = Server::Post(OBJ_REQUIRE_ID, ("name=" + ComputerName()));

		try
		{
			MachineID = std::stoi(ID);
		}
		catch (...)
		{
			::MessageBox(NULL, TEXT_FAILED_EN, TEXT_ERROR_EN, MB_OK | MB_ICONSTOP);
			::PostQuitMessage(0);
		}
	}

	void SetID(UINT nID)
	{
		MachineID = nID;
	}

	UINT GetID()
	{
		return MachineID;
	}

	void SetAs(DWORD lang)
	{
		dwLanguage = lang;
	}

	const DWORD Language()
	{
		return dwLanguage;
	}

	const std::wstring LanguageStr()
	{
		return IsMongolian() ? TEXT_MN : TEXT_EN;
	}

	void Select(DWORD dwLang)
	{
		dwLanguage = dwLang;
	}

	const std::wstring DataPath(std::wstring folder = TEXT_EMPTY, BOOL bAbsolute = ABSOLUTE_PATH)
	{
		return (bAbsolute ? (szAbsolutePath + szDataPath) : szDataPath) + folder;
	}

	const std::string DataPathAnsi(std::wstring folder = TEXT_EMPTY, BOOL bAbsolute = ABSOLUTE_PATH)
	{
		char szBuffer[MAX_PATH];
		std::wstring path = DataPath(folder, bAbsolute);
		for (size_t i = 0; i < path.length(); i++)
		{
			szBuffer[i] = (char) path[i];
		}
		szBuffer[path.length()] = '\n';
		return std::string(szBuffer);
	}

	const std::wstring CurrentDirectory()
	{
		return szCurrentDir;
	}

	const std::wstring AbsoluteDirectory()
	{
		return szAbsolutePath;
	}

	const bool IsMongolian()
	{
		return (dwLanguage == LANG_MONGOLIAN);
	}

	const bool IsEnglish()
	{
		return (dwLanguage == LANG_ENGLISH);
	}

	const LONG Width()
	{
		return coord::width(Primary());
	}

	const LONG Height()
	{
		return coord::height(Primary());
	}

	void SetClientRect(
		RECT rect,
		LONG nHeaderHeight = HEIGHT_CLIENT_HEADER,
		LONG nFooterHeight = HEIGHT_CLIENT_FOOTER)
	{
		rcClient = rect;
		rect.top += nHeaderHeight;
		rect.bottom -= nFooterHeight;
		rcWorker = rect;
	}

	const RECT Primary()
	{
		return rcPrimary;
	}

	const RECT Client()
	{
		return rcClient;
	}

	const RECT Worker()
	{
		return rcWorker;
	}

	const RECT Header()
	{
		RECT rect = Client();
		rect.bottom = Worker().top;
		return rect;
	}

	const RECT Footer()
	{
		RECT rect = Client();
		rect.top = Worker().bottom;
		return rect;
	}

	const HFONT Font(
		int     nHeight = DEF_FONT_HEIGHT,
		int     nWidth = 0,
		int     nEscapement = 0,
		int     nOrientation = 0,
		int     fnWeight = FW_DONTCARE,
		DWORD   fdwItalic = FALSE,
		DWORD   fdwUnderline = FALSE,
		DWORD   fdwStrikeOut = FALSE,
		DWORD   fdwCharSet = DEFAULT_CHARSET,
		DWORD   fdwOutputPrecision = OUT_OUTLINE_PRECIS,
		DWORD   fdwClipPrecision = CLIP_DEFAULT_PRECIS,
		DWORD   fdwQuality = CLEARTYPE_QUALITY,
		DWORD   fdwPitchAndFamily = VARIABLE_PITCH,
		LPCTSTR lpszFace = DEF_FONT_FACE)
	{
		return ::CreateFont(nHeight, nWidth, nEscapement, nOrientation, fnWeight, fdwItalic, fdwUnderline, fdwStrikeOut,
			fdwCharSet, fdwOutputPrecision, fdwClipPrecision, fdwQuality, fdwPitchAndFamily, lpszFace);
	}
	
	CVersion version;
	std::wstring key;

protected:
	void SetPrimaryRect(LONG left = MAIN_LEFT, LONG top = MAIN_TOP, LONG nWidth = MAIN_WIDTH, LONG nHeight = MAIN_HEIGHT)
	{
		rcPrimary.left = left;
		rcPrimary.top = top;
		rcPrimary.right = rcPrimary.left + (USE_SCREEN_RECT ? ::GetSystemMetrics(SM_CXSCREEN) : nWidth);
		rcPrimary.bottom = rcPrimary.top + (USE_SCREEN_RECT ? ::GetSystemMetrics(SM_CYSCREEN) : nHeight);
	}
};

extern Config cfg;