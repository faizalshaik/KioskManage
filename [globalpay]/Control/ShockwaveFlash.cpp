/**
* ShockwaveFlash.cpp
*
* @program		Global-Pay
* @author		АНКУ ХХК
* @copyright	Copyright (c) 2017 он, АНКУ ХХК.
* @version		0.1.0.0
* @date			2017.09.19
*/

#include "shockwaveflash.h"

using namespace globalpay;

CFlashNotify::CFlashNotify()
{
	ref = 1;
	FlashInvoke = NULL;
}

CFlashControl::CFlashControl(void)
	: CActiveX(), CCoObject(NULL, COINIT_APARTMENTTHREADED)
{
	SetClass(CFLASHCONTROLCLASS);
	
	axCookie = 0;
	cpc = NULL;
	cp = NULL;
}

BOOL CFlashControl::Init(RECT rc, HWND hParent, HINSTANCE hInst, LPCTSTR szText, DWORD dwStyle, UINT classStyle)
{
	if (!Register(classStyle, hInst))
		return FALSE;

	if (!Create(szText, dwStyle, rc, hParent, NULL, hInst))
		return FALSE;

	return TRUE;
}

void CFlashControl::ActivateControl()
{
	SendMessage(WM_AX_INPLACE, 1);
}

BOOL CFlashControl::SetMovie(std::wstring lpMoviePath)
{
	return SetMovie((TCHAR*)lpMoviePath.c_str());
}

BOOL CFlashControl::SetMovie(LPTSTR lpMoviePath)
{
	IShockwaveFlash *p = 0;
	CLSID iidx = __uuidof(IShockwaveFlash);
	SendMessage(WM_AX_QUERYINTERFACE, (WPARAM)&iidx, (LPARAM)&p);
	if (p)
	{
		p->put_Movie(lpMoviePath);

		CActiveX* iax = (CActiveX*)SendMessage(WM_AX_GETAXINTERFACE);
		if (iax)
			axCookie = ConnectObject(iax->OleObject, __uuidof(_IShockwaveFlashEvents), (IUnknown*)&fn, &cpc, &cp);

		p->Release();
		return TRUE;
	}
	return FALSE;
}

BOOL CFlashControl::CallFunction(LPTSTR lpFlashXml)
{
	IShockwaveFlash* p = 0;
	CLSID iidx = __uuidof(IShockwaveFlash);
	HRESULT hr = (HRESULT)SendMessage(WM_AX_QUERYINTERFACE, (WPARAM)&iidx, (LPARAM)&p);
	if (p && SUCCEEDED(hr))
	{
		try
		{
			p->CallFunction(lpFlashXml);
		}
		catch (...)
		{
		}
		p->Release();
		return TRUE;
	}
	return FALSE;
}

ULONG __stdcall CFlashNotify::AddRef()
{
	return ++ref;
}

ULONG __stdcall CFlashNotify::Release()
{
	return --ref;
}

HRESULT __stdcall CFlashNotify::QueryInterface(const IID &id, void **p)
{
	if (id == IID_IUnknown || id == __uuidof(_IShockwaveFlashEvents) || id == IID_IDispatch)
	{
		*p = this;
		
		AddRef();

		return S_OK;
	}

	return E_NOINTERFACE;
}

HRESULT __stdcall CFlashNotify::GetTypeInfoCount(UINT *c)
{
	if (!c)
		return E_POINTER;

	*c = 0;
	return S_OK;
}

HRESULT __stdcall CFlashNotify::GetTypeInfo(UINT, LCID, ITypeInfo FAR* FAR*)
{
	return E_NOTIMPL;
}

HRESULT __stdcall CFlashNotify::GetIDsOfNames(REFIID, LPOLESTR*, UINT, LCID, DISPID FAR*)
{
	return E_NOTIMPL;
}

HRESULT __stdcall CFlashNotify::Invoke(DISPID d, REFIID, LCID, WORD, DISPPARAMS* p, VARIANT FAR*, EXCEPINFO FAR*, UINT FAR*)
{
	if (d != 0xC5)
		return S_OK;

	if (!p)
		return E_POINTER;

	if (p->cArgs != 1)
		return E_INVALIDARG;

	if (p->rgvarg[0].vt != VT_BSTR)
		return E_INVALIDARG;

	if (FlashInvoke != NULL)
		(*FlashInvoke)(d, p);

	return S_OK;
}

std::wstring CFlashControl::GetExternalXML(std::wstring szFunction, std::wstring szArguments)
{
	return TEXT("<invoke name=\"") + szFunction + TEXT("\" returntype=\"xml\"><arguments>") + szArguments + TEXT("</arguments></invoke>");
}

std::wstring CFlashControl::GetArgumentXML(std::wstring value, FlashExternalInterfaceArgTypes type)
{
	switch (type)
	{
	case feiaString: return TEXT("<string>") + value + TEXT("</string>");
	case feiaNumber: return TEXT("<number>") + value + TEXT("</number>");
	case feiaBoolean: break;
	}
	return value;
}