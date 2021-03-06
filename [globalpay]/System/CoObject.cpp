/**
* CoObject.cpp
*
* @program		Global-Pay
* @author		АНКУ ХХК
* @copyright	Copyright (c) 2017 он, АНКУ ХХК.
* @version		0.1.0.0
* @date			2017.09.19
*/

#include "coobject.h"

using namespace globalpay;

BOOL CCoObject::IsBooted = FALSE;
UINT CCoObject::Instance = 0;

CCoObject::CCoObject(LPVOID pvReserved, DWORD dwCoInit)
{
	if (!CCoObject::IsBooted)
		CCoObject::IsBooted = SUCCEEDED(Initialize(pvReserved, dwCoInit));	
}

CCoObject::~CCoObject()
{
	if (CCoObject::IsBooted && CCoObject::Instance == 0)
		Uninitialize();
}

HRESULT CCoObject::Initialize(LPVOID pvReserved, DWORD dwCoInit)
{
	return ::CoInitializeEx(pvReserved, dwCoInit);
}

void CCoObject::Uninitialize()
{
	::CoUninitialize();
	CCoObject::IsBooted = FALSE;
}

HRESULT CCoObject::CreateInstance(REFCLSID rclsid, LPUNKNOWN pUnkOuter, DWORD dwClsContext, REFIID riid, LPVOID FAR *ppv)
{
//	if (!CCoObject::IsBooted)
//		return E_POINTER;

	HRESULT hr = ::CoCreateInstance(rclsid, pUnkOuter, dwClsContext, riid, ppv);
	
	if (SUCCEEDED(hr))
	{
		CCoObject::Instance++;
	}

	return hr;
}