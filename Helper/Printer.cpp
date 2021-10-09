#include "printer.h"

using namespace helper;

int PrinterStatus::getPrinterStatus(int prnNum)
{
	if (prnNum < 1 || prnNum > 2)
		return -1;

	HINSTANCE m_hLib;

	TCHAR *strLibPath = TEXT("resource\\CePrnLib.dll");
	// strlibpath iig ustgah heregtei, ashiglaj duusaad

	m_hLib = ::LoadLibrary((LPCTSTR)strLibPath); 
	// LoadLibrary deer instance hiisen bol Unload hiih shaardlagatai!

	if (m_hLib)
	{
		// Load DLL functions we need
		m_lpfnCeInitUSBLayer = (LPFNDLLFUNC_020)::GetProcAddress(m_hLib, "CeInitUSBLayer");
		m_lpfnCeDeInitUSBLayer = (LPFNDLLFUNC_021)::GetProcAddress(m_hLib, "CeDeInitUSBLayer");
		m_lpfnCeOpenUSBDev = (LPFNDLLFUNC_022)::GetProcAddress(m_hLib, "CeOpenUSBDev");
		m_lpfnCeGetSts = (LPFNDLLFUNC_012)::GetProcAddress(m_hLib, "CeGetSts");

		DWORD dwCodedErr = 0;
		DWORD dwSysErr = 0;
		DWORD dwPrnSts = 0;
		HANDLE m_hUSBObj = NULL;

		// Establish a connection to the USB layer
		if (m_lpfnCeInitUSBLayer)
			dwCodedErr = m_lpfnCeInitUSBLayer(&m_hUSBObj, &dwSysErr);

		if (dwCodedErr != 0 || dwSysErr != 0)
		{
			::MessageBox(NULL, TEXT("Could not connect to USB layer"), TEXT("Error!"), MB_OK | MB_ICONERROR);
			return -1;
		}

		WCHAR *m_strPrnName = (prnNum == 1) ? TEXT("CUSTOM K80-1") : TEXT("CUSTOM K80-2");

		// Get a direct connection to the printer using it's name
		if (m_lpfnCeOpenUSBDev)
			dwCodedErr = m_lpfnCeOpenUSBDev(m_hUSBObj, (LPCTSTR)m_strPrnName, &dwSysErr);

		if (dwCodedErr != 0 || dwSysErr != 0)
		{
			::MessageBox(NULL, TEXT("Could not establish USB connection to the printer via given name"), TEXT("Error!"), MB_OK | MB_ICONERROR);
			return -1;
		}

		// Get the pritner's status
		if (m_lpfnCeGetSts)
			dwCodedErr = m_lpfnCeGetSts(m_hUSBObj, &dwPrnSts, &dwSysErr);

		if (dwCodedErr != 0 || dwSysErr != 0)
		{
			::MessageBox(NULL, TEXT("Could not access printer status"), TEXT("Error!"), MB_OK | MB_ICONERROR);
			return -1;
		}

		char szBuffer[8];
		::sprintf_s(szBuffer, "%02x", (BYTE)(dwPrnSts >> 24));

		int ret = ::atoi(szBuffer);

		switch (ret)
		{
		case 0:
			::sprintf_s(szBuffer, "Error code: %d. WORKING NO ERROR! ", ret);
			break;
		case 1:
			::sprintf_s(szBuffer, "Error code: %d. PAPER PRESENT BUT NOT FED! ", ret);
			break;
		case 4:
			::sprintf_s(szBuffer, "Error code: %d. PAPER ENDING SOON! ", ret);
			break;
		case 5:
			::sprintf_s(szBuffer, "Error code: %d. NO PAPER! ", ret);
			break;
		default:
			::sprintf_s(szBuffer, "Error code: %d. OTHER ERROR! ", ret);
		}

		::MessageBox(NULL, (LPCWSTR)szBuffer, (prnNum == 1) ? TEXT("CUSTOM K80-1") : TEXT("CUSTOM K80-2"), NULL);

		m_lpfnCeDeInitUSBLayer(m_hUSBObj);

		delete m_strPrnName;

		return ret;
	}
	else
	{
		::MessageBox(NULL, TEXT("Failed to read the DLL"), TEXT("Title"), MB_OK | MB_ICONERROR);
		return -1;
	}
}