/**
* Adobe Flash - Shockwave control in Win32 - ShockwaveFlash.h
*
* @program		Global-Pay
* @author		АНКУ ХХК
* @copyright	Copyright (c) 2017 он, АНКУ ХХК.
* @version		0.1.0.0
* @date			2017.09.19
*/

#pragma once

#include <windows.h>

#include "../system/activex.h"
#include "../system/coobject.h"

#import "progid:ShockwaveFlash.ShockwaveFlash" exclude("IServiceProvider")

using namespace ShockwaveFlashObjects;

//#pragma warning(disable:4584)

namespace globalpay {
	class CFlashNotify : public _IShockwaveFlashEvents
	{
	private:
		int ref;

	public:
		CFlashNotify();

		void(*FlashInvoke)(DISPID, DISPPARAMS*);

		ULONG   __stdcall AddRef();
		ULONG   __stdcall Release();
		HRESULT __stdcall QueryInterface(const IID&, void**);
		HRESULT __stdcall GetTypeInfoCount(UINT*);
		HRESULT __stdcall GetTypeInfo(UINT, LCID, ITypeInfo FAR* FAR*);
		HRESULT __stdcall GetIDsOfNames(REFIID, LPOLESTR*, UINT, LCID, DISPID FAR*);
		HRESULT __stdcall Invoke(DISPID, REFIID, LCID, WORD, DISPPARAMS*, VARIANT FAR*, EXCEPINFO FAR*, UINT FAR*);
	};

	typedef enum
	{
		feiaString,
		feiaNumber,
		feiaBoolean
	} FlashExternalInterfaceArgTypes;

	class CFlashControl : public CActiveX, public CCoObject
	{
#define CFLASHCONTROLCLASS TEXT("codesaur::flashcontrol")
	public:
		CFlashControl();

		CFlashNotify               fn;
		DWORD                      axCookie;
		IConnectionPointContainer *cpc;
		IConnectionPoint          *cp;

		BOOL Init(RECT, HWND, HINSTANCE, LPCTSTR szText = TEXT("ShockwaveFlash.ShockwaveFlash"), DWORD dwStyle = WS_CHILD, UINT classStyle = CS_GLOBALCLASS | CS_DBLCLKS);

		void ActivateControl();
		BOOL SetMovie(std::wstring);
		BOOL SetMovie(LPTSTR);
		BOOL CallFunction(LPTSTR);

		std::wstring GetExternalXML(std::wstring, std::wstring szArguments = TEXT(""));
		std::wstring GetArgumentXML(std::wstring value, FlashExternalInterfaceArgTypes type = feiaString);
	};
}