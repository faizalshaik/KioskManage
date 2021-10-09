#pragma once

#include <windows.h>

#pragma comment(lib, "version.lib")

#define VERSION_DELIMITER   TEXT(".")
#define VERSION_DELIMITER_A "."
#define DIRECTORY_SEPARATOR TEXT("\\")

class CVersion
{
public:
	UINT mostHi;
	UINT mostLo;
	UINT leastHi;
	UINT leastLo;

	SYSTEMTIME create;
	SYSTEMTIME access;
	SYSTEMTIME write;

	CVersion() { Set(0, 0, 0, 0); }

	bool operator < (const CVersion v) const
	{
		if (mostHi < v.mostHi)
			return true;
		else if (mostHi > v.mostHi)
			return false;

		if (mostLo < v.mostLo)
			return true;
		else if (mostLo > v.mostLo)
			return false;

		if (leastHi < v.leastHi)
			return true;
		else if (leastHi > v.leastHi)
			return false;

		return leastLo < v.leastLo;
	}

	bool ReadFrom(LPCTSTR szFileName)
	{
		DWORD dwHandle;
		DWORD dwSize = ::GetFileVersionInfoSize(szFileName, &dwHandle);
		if (dwSize)
		{
			char *buffer = new char[dwSize];
			if (::GetFileVersionInfo(szFileName, dwHandle, dwSize, &buffer[0]))
			{
				VS_FIXEDFILEINFO *pvi;
				dwSize = sizeof(VS_FIXEDFILEINFO);
				if (::VerQueryValue(&buffer[0], DIRECTORY_SEPARATOR, (LPVOID*)&pvi, (UINT*)&dwSize))
				{
					Set(
						HIWORD(pvi->dwFileVersionMS), LOWORD(pvi->dwFileVersionMS),
						HIWORD(pvi->dwFileVersionLS), LOWORD(pvi->dwFileVersionLS));

					HANDLE hFile = ::CreateFile(szFileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
					if (hFile != INVALID_HANDLE_VALUE)
					{
						FILETIME ftCreate, ftAccess, ftWrite;
						if (::GetFileTime(hFile, &ftCreate, &ftAccess, &ftWrite))
						{
							SYSTEMTIME stUTC;

							::FileTimeToSystemTime(&ftCreate, &stUTC);
							::SystemTimeToTzSpecificLocalTime(NULL, &stUTC, &create);

							::FileTimeToSystemTime(&ftAccess, &stUTC);
							::SystemTimeToTzSpecificLocalTime(NULL, &stUTC, &access);

							::FileTimeToSystemTime(&ftWrite, &stUTC);
							::SystemTimeToTzSpecificLocalTime(NULL, &stUTC, &write);
						}
					}
					return true;
				}
			}
			delete[] buffer;
		}
		return false;
	}

	std::wstring AsString()
	{
		return
			std::to_wstring(mostHi) + VERSION_DELIMITER +
			std::to_wstring(mostLo) + VERSION_DELIMITER +
			std::to_wstring(leastHi) + VERSION_DELIMITER +
			std::to_wstring(leastLo);
	}

	std::string AsStringA()
	{
		return
			std::to_string(mostHi) + VERSION_DELIMITER_A +
			std::to_string(mostLo) + VERSION_DELIMITER_A +
			std::to_string(leastHi) + VERSION_DELIMITER_A +
			std::to_string(leastLo);
	}

	void Set(UINT ma1, UINT ma2, UINT mi1, UINT mi2)
	{
		mostHi = ma1;
		mostLo = ma2;
		leastHi = mi1;
		leastLo = mi2;
	}

	bool IsEmpty()
	{
		return (mostHi == 0) && (mostLo == 0) && (leastHi == 0) && (leastLo == 0);
	}
};