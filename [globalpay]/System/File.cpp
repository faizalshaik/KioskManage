/**
* File.cpp
*
* @program		Global-Pay
* @author		АНКУ ХХК
* @copyright	Copyright (c) 2017 он, АНКУ ХХК.
* @version		0.1.0.0
* @date			2017.09.19
*/

#include "file.h"

using namespace globalpay;

BOOL GetModuleDirectory(LPTSTR lpPath, HMODULE hModule)
{
	TCHAR szTemp[MAX_PATH];
	if (!::GetModuleFileName(hModule, szTemp, MAX_PATH))
		return FALSE;
	
	int i = string::find_last(szTemp, TEXT("\\"));
	if (i != -1)
	{
		string::ncopy(lpPath, szTemp, 0, i);
		return TRUE;
	}
	return FALSE;
}

BOOL GetCurrentDirectory(LPTSTR lpDir, DWORD dwBUFFERSIZE)
{
	if (int i = ::GetCurrentDirectory(dwBUFFERSIZE, lpDir))
	{
		lpDir[i] = '\\';
		lpDir[i+1] = '\0';
		return TRUE;
	}
	return FALSE;
}

BOOL ResExtractToFile(TCHAR* fName, TCHAR* Name, HINSTANCE hy, bool IsMemoryBuffer, TCHAR* Type)
{
	BOOL bRet = FALSE;

	HINSTANCE hI = hy;
	HRSRC R;
	if (Type)
		R = FindResource(hI, Name, Type);
	else
		R = FindResource(hI, Name, _T("DATA"));
	
	if (!R)
		return bRet;
	
	HGLOBAL hG = LoadResource(hI, R);
	if (!hG)
		return bRet;
	
	int S = SizeofResource(hI, R);
	char *p = (char *)LockResource(hG);
	if (!p)
	{
		FreeResource(R);
		return bRet;
	}

	if (IsMemoryBuffer)
	{
		memcpy(fName, p, S);
		bRet = TRUE;
	}
	else
	{
		FILE *fp = 0;
		_wfopen_s(&fp, fName, _T("wb"));
		if (fp)
		{
			fwrite(p, 1, S, fp);
			fclose(fp);
			bRet = TRUE;
		}
	}
	FreeResource(R);
	return bRet;
}