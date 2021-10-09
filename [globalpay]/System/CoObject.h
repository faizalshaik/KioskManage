/**
* CCoObject class - CoObject.h
  Component Object Model
*
* @program		Global-Pay
* @author		АНКУ ХХК
* @copyright	Copyright (c) 2017 он, АНКУ ХХК.
* @version		0.1.0.0
* @date			2017.09.19
*/

#pragma once

#include <windows.h>
#include <wincodec.h>
#include <d2d1.h>

#pragma comment(lib, "d2d1.lib")

namespace globalpay {
	class CCoObject
	{
	public:
		static BOOL IsBooted;
		static UINT Instance;
		template <typename T>
		inline void ReleaseInstance(T *&p)
		{
			if (p != NULL)
			{
				p->Release();
				p = NULL;

				if (CCoObject::Instance > 0)
					CCoObject::Instance--;
			}
		}

		CCoObject(LPVOID pvReserved = NULL, DWORD dwCoInit = COINIT_MULTITHREADED);
		~CCoObject();

		HRESULT Initialize(LPVOID pvReserved, DWORD dwCoInit);
		void    Uninitialize();

		HRESULT CreateInstance(REFCLSID, LPUNKNOWN, DWORD, REFIID, LPVOID FAR *);		
	};
}