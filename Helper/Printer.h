#include <windows.h>  
#include <stdlib.h>  
#include <string.h>  
#include <tchar.h> 
#include <stdio.h>

namespace helper {
	class PrinterStatus
	{
	public:
		// ************************************************************************** //
		// Parameters:                                                                //
		//  int prnNum   --  Printer id (1 or 2)                                      //
		// ************************************************************************** //
		// Return values:                                                             //
		// -1   --  Error                                                             //
		//  0   --  Working, no error.                                                //
		//  1   --  Paper found, but not fed to printer.                              //
		//  4   --  Paper ending soon.                                                //
		//  5   --  No paper found.                                                   //
		// ************************************************************************** //
		int getPrinterStatus(int prnNum = 1);

	protected:
		typedef DWORD(*LPFNDLLFUNC_012)(HANDLE, LPDWORD, LPDWORD);
		typedef DWORD(*LPFNDLLFUNC_020)(LPHANDLE, LPDWORD);
		typedef DWORD(*LPFNDLLFUNC_021)(HANDLE);
		typedef DWORD(*LPFNDLLFUNC_022)(HANDLE, LPCTSTR, LPDWORD);

		LPFNDLLFUNC_012 m_lpfnCeGetSts;
		LPFNDLLFUNC_020 m_lpfnCeInitUSBLayer;
		LPFNDLLFUNC_021 m_lpfnCeDeInitUSBLayer;
		LPFNDLLFUNC_022 m_lpfnCeOpenUSBDev;
	};
}