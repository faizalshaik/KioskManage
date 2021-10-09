/**
* ActiveX.cpp
*
* @program		Global-Pay
* @author		АНКУ ХХК
* @copyright	Copyright (c) 2017 он, АНКУ ХХК.
* @version		0.1.0.0
* @date			2017.09.19
*/

#include "activex.h"

#pragma warning (disable: 4800)

using namespace globalpay;

//CActiveXDispatchNotification::CActiveXDispatchNotification()
//{
	//::memset(&riid, 0, sizeof(riid));

	//ax = NULL;
	//dispIdMember = 0;
	//wFlags = 0;
	//lcid = 0;
	//pDispParams = NULL;
	//pVarResult = NULL;
	//pExcepInfo = NULL;
	//puArgErr = NULL;
//}

CActiveX::CActiveX()
	: CWnd()
{
	SetClass(CACTIVEXCLASS);

	onCreate = OnCreate;
	onDestroy = OnDestroy;
	onCommand = OnCommand;
	onSize = OnSize;
	onMouseDblClick = OnMouseDblClick;

	AllocCustomMessages(12);
	SetCustomMessage(0, WM_AX_RECREATE, OnRecreate);
	SetCustomMessage(1, WM_AX_QUERYINTERFACE, OnQueryInterface);
	SetCustomMessage(2, WM_AX_INPLACE, OnInPlace);
	SetCustomMessage(3, WM_AX_GETAXINTERFACE, OnGetAXInterface);
	SetCustomMessage(4, WM_AX_CONNECTOBJECT, OnConnectObject);
	SetCustomMessage(5, WM_AX_DISCONNECTOBJECT, OnDisconnetObject);
	SetCustomMessage(6, WM_AX_SETDATAADVISE, OnSetDataAdvice);
	SetCustomMessage(7, WM_AX_DOVERB, OnDoverb);
	SetCustomMessage(8, WM_AX_SETCOMMANDCALLBACK, OnSetCommandCallback);
	SetCustomMessage(9, WM_AX_SETDISPATCHNOTIFICATION, OnSetDispatchNotification);
	SetCustomMessage(10, WM_AX_SETSERVICEPROVIDER, OnSetServiceProvider);
	SetCustomMessage(11, WM_AX_SETDISPATCHNOTIFICATIONFUNC, OnSetDispatchNotificationFunc);
}

CActiveXClientSite::CActiveXClientSite()
	: CWnd()
{
	SetClass(CACTIVEXCLIENTSITECLASS);

	refNum = 0;
	CalledCanInPlace = 0;
	InPlace = 0;
}

CActiveXClientSite::~CActiveXClientSite()
{
}

void CActiveX::Init(LPCTSTR szCLSID)
{
	::CLSIDFromString(szCLSID, &clsid);

	::memset(DAdviseToken, 0, sizeof(DAdviseToken));

	iid = (IID*)&IID_IOleObject;
	OleObject = 0;
	Storage = 0;
	View = 0;
	Data = 0;
	Unk = 0;
	Pao = 0;
	AdviseToken = 0;
	CustomServiceProvider = 0;

	Site.activex = this;

	DispatchNotificationFunction = 0;
	DispatchNotificationWindow = 0;
	DispatchNotificationMessage = 0;
}

void CActiveX::Clean()
{
	if (Site.InPlace == true)
	{
		Site.InPlace = false;
		IOleInPlaceObject *iib = 0;

		if (OleObject)
			OleObject->QueryInterface(IID_IOleInPlaceObject, (void**)&iib);

		if (iib)
		{
			iib->UIDeactivate();
			iib->InPlaceDeactivate();
			iib->Release();
		}
	}

	if (AdviseToken && OleObject)
	{
		OleObject->Unadvise(AdviseToken);
		AdviseToken = 0;
	}

	if (Data)
	{
		for (int i = 0; i < 100; i++)
			if (DAdviseToken[i])
				Data->DUnadvise(DAdviseToken[i]);

		::memset(DAdviseToken, 0, sizeof(DAdviseToken));
	}

	SafeRelease(Pao);
	SafeRelease(Unk);
	SafeRelease(Data);
	SafeRelease(View);
	SafeRelease(Storage);
	SafeRelease(OleObject);
}

CLSID CActiveX::GetCLSID()
{
	return clsid;
}

int CActiveX::ConnectObject(IUnknown *Container, REFIID riid, IUnknown *Advisor, IConnectionPointContainer **picpc, IConnectionPoint **picp)
{
	HRESULT hr = 0;
	ULONG tid = 0;
	IConnectionPointContainer *icpc = 0;
	IConnectionPoint *icp = 0;
	*picpc = 0;
	*picp = 0;

	Container->QueryInterface(IID_IConnectionPointContainer, (void **)&icpc);
	if (icpc)
	{
		*picpc = icpc;
		icpc->FindConnectionPoint(riid, &icp);
		if (icp)
		{
			*picp = icp;
			hr = icp->Advise(Advisor, &tid);
		}
	}
	return tid;
}

void CActiveX::DisconnectObject(IConnectionPointContainer *icpc, IConnectionPoint *icp, UINT Cookie)
{
	ULONG hr = 0;
	icp->Unadvise(Cookie);
	icp->Release();
	icpc->Release();
}

LPVOID CActiveX::OnCreate(HWND hWnd, LPARAM)
{
	CActiveX *me = reinterpret_cast<CActiveX*>(GetUserDataLong(hWnd));
	if (me)
	{
		std::wstring Title = me->AsString();

		if ((char)Title.c_str()[0] == '}')
			return 0L;

		me->Init(Title.c_str());

		me->Site.SetHandleForced(*me);
		//me->Site.SetClass(me->GetClass());

		HRESULT hr = ::StgCreateDocfile(0, STGM_READWRITE | STGM_SHARE_EXCLUSIVE | STGM_DIRECT | STGM_CREATE, 0, &me->Storage);

		REFIID rid = *me->iid;

		CLSID cid = me->GetCLSID();

		hr = ::OleCreate(cid, rid, OLERENDER_DRAW, 0, &me->Site, me->Storage, (void**)&me->OleObject);
		if (FAILED(hr))
		{
			hr = ::OleCreate(cid, rid, OLERENDER_NONE, 0, &me->Site, me->Storage, (void**)&me->OleObject);
		}

		if (!me->OleObject)
		{
			::DestroyWindow(*me);
			return 0L;
		}

		me->OleObject->SetClientSite(&me->Site);

		hr = ::OleSetContainedObject(me->OleObject, TRUE);
		hr = me->OleObject->Advise(&me->Site, &me->AdviseToken);
		hr = me->OleObject->QueryInterface(IID_IViewObject, (void**)&me->View);
		hr = me->OleObject->QueryInterface(IID_IDataObject, (void**)&me->Data);

		if (me->View)
			hr = me->View->SetAdvise(DVASPECT_CONTENT, 0, &me->Site);
	}
	return 0L;
}

LPVOID CActiveX::OnDestroy(HWND hWnd)
{
	CActiveX *me = reinterpret_cast<CActiveX*>(GetUserDataLong(hWnd));
	if (me)
	{
		me->Clean();
		return voidTRUE;
	}
	return 0L;
}

LPVOID CActiveX::OnCommand(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	CActiveX *me = reinterpret_cast<CActiveX*>(GetUserDataLong(hWnd));
	if (me)
		return (LPVOID)::SendMessage(me->CommandCallbackWindow, WM_COMMAND, wParam, lParam);
	return 0L;
}

LPVOID CActiveX::OnMouseDblClick(HWND hWnd, UINT mButton, WPARAM sState, int X, int Y)
{
	if (mButton == mButtonLeft)
		::PostMessage(hWnd, WM_AX_INPLACE, 1, 0);

	return 0L;
}

LPVOID CActiveX::OnSize(HWND hWnd, UINT nType, int cx, int cy)
{
	CActiveX *me = reinterpret_cast<CActiveX*>(GetUserDataLong(hWnd));
	if (me)
	{
		if (!me->OleObject)
			return 0L;

		NMHDR nh = { 0 };
		nh.code = ACTIVEX_N_SIZE;
		nh.hwndFrom = *me;
		nh.idFrom = me->GetID();
		::SendMessage(me->Site.GetParent(), WM_NOTIFY, 0, (LPARAM)&nh);
		::DefWindowProc(*me, WM_SIZE, (WPARAM)nType, MAKELPARAM(cx, cy));

		IOleInPlaceObject* pl;
		me->OleObject->QueryInterface(IID_IOleInPlaceObject, (void**)&pl);
		if (!pl)
			return 0L;

		RECT r = me->Site.GetClientRect();
		pl->SetObjectRects(&r, &r);
		pl->Release();
	}
	return 0L;
}

LPVOID CActiveX::OnRecreate(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	CActiveX *me = reinterpret_cast<CActiveX*>(GetUserDataLong(hWnd));
	if (me)
	{
		std::wstring Title = me->AsString();

		Title[0] = '{';

		me->Init(Title.c_str());

		me->Site.SetHandleForced(*me);
		//me->Site.SetClass(me->GetClass());

		HRESULT hr = ::StgCreateDocfile(0, STGM_READWRITE | STGM_SHARE_EXCLUSIVE | STGM_DIRECT | STGM_CREATE, 0, &me->Storage);

		REFIID rid = *me->iid;

		IUnknown* u = (IUnknown*)lParam;
		if (u)
			u->QueryInterface(rid, (void**)&me->OleObject);

		if (!me->OleObject)
		{
			::DestroyWindow(*me);
			return (LPVOID)-1;
		}

		me->OleObject->SetClientSite(&me->Site);

		hr = ::OleSetContainedObject(me->OleObject, TRUE);
		hr = me->OleObject->Advise(&me->Site, &me->AdviseToken);
		hr = me->OleObject->QueryInterface(IID_IViewObject, (void**)&me->View);
		hr = me->OleObject->QueryInterface(IID_IDataObject, (void**)&me->Data);

		if (me->View)
			hr = me->View->SetAdvise(DVASPECT_CONTENT, 0, &me->Site);
	}
	return 0L;
}

LPVOID CActiveX::OnQueryInterface(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	CActiveX *me = reinterpret_cast<CActiveX*>(GetUserDataLong(hWnd));
	if (me)
	{
		char *p = (char*)wParam;
		return (LPVOID)me->OleObject->QueryInterface((REFIID)*p, (void**)lParam);
	}	
	return 0L;
}

LPVOID CActiveX::OnInPlace(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	CActiveX *me = reinterpret_cast<CActiveX*>(GetUserDataLong(hWnd));
	if (me)
	{
		if (!me->OleObject)
			return 0L;

		RECT rect = me->GetClientRect();
		HRESULT hr;

		if (me->Site.InPlace == false && wParam == 1)
		{
			me->Site.InPlace = true;
			me->Site.ExternalPlace = lParam;
			hr = me->OleObject->DoVerb(OLEIVERB_INPLACEACTIVATE, 0, &me->Site, 0, *me, &rect);
			::InvalidateRect(*me, 0, true);
			return (LPVOID)1;
		}

		if (me->Site.InPlace == true && wParam == 0)
		{
			me->Site.InPlace = false;

			IOleInPlaceObject* iib;
			me->OleObject->QueryInterface(IID_IOleInPlaceObject, (void**)&iib);
			if (iib)
			{
				iib->UIDeactivate();
				iib->InPlaceDeactivate();
				iib->Release();
				::InvalidateRect(*me, 0, true);
				return (LPVOID)1;
			}
		}
	}
	return 0L;
}

LPVOID CActiveX::OnGetAXInterface(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	CActiveX *me = reinterpret_cast<CActiveX*>(GetUserDataLong(hWnd));
	if (me)
		return (LPVOID)(LONG_PTR)me;
	return 0L;
}

LPVOID CActiveX::OnConnectObject(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	CActiveX *me = reinterpret_cast<CActiveX*>(GetUserDataLong(hWnd));
	if (me)
	{
		int *wx = (int*)wParam;
		ActiveXConnectStruct *tcs = (ActiveXConnectStruct*)lParam;
		if (tcs->Advisor == 0)
		{
			tcs->Advisor = reinterpret_cast<IUnknown*>(&me->Site);
			me->Site.SpecialIIDForceOK = tcs->SpecialIIDForceOK;
		}
		tcs->ID = me->ConnectObject(reinterpret_cast<IUnknown*>(me->OleObject), (REFIID)*wx, tcs->Advisor, &tcs->icpc, &tcs->icp);
		me->tcs = new ActiveXConnectStruct;
		::memcpy(me->tcs, tcs, sizeof(ActiveXConnectStruct));
	}
	return 0L;
}

LPVOID CActiveX::OnDisconnetObject(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	CActiveX *me = reinterpret_cast<CActiveX*>(GetUserDataLong(hWnd));
	if (me)
	{
		char *p = (char*)wParam;
		ActiveXConnectStruct *tcs = me->tcs;
		me->DisconnectObject(tcs->icpc, tcs->icp, tcs->ID);
		delete me->tcs;
		me->tcs = 0;
	}
	return 0L;
}

LPVOID CActiveX::OnSetDataAdvice(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	CActiveX *me = reinterpret_cast<CActiveX*>(GetUserDataLong(hWnd));
	if (me)
	{
		if (!me->Data)
			return 0L;

		IEnumFORMATETC* ief = 0;
		int i = 0;

		FORMATETC fe;
		me->Data->EnumFormatEtc((LPARAM)lParam, &ief);
		if (!ief)
			return 0;

		for (;;)
		{
			HRESULT hr = ief->Next(1, &fe, 0);
			if (hr != S_OK)
				break;
			if (wParam == i)
				break;
			i++;
		}
		ief->Release();

		if (wParam == -1)
			return (LPVOID)i;

		if (me->Data)
			me->Data->DAdvise(&fe, 0, &me->Site, &me->DAdviseToken[wParam]);

		return voidTRUE;
	}
	return 0L;
}

LPVOID CActiveX::OnDoverb(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	CActiveX *me = reinterpret_cast<CActiveX*>(GetUserDataLong(hWnd));
	if (me)
	{
	}
	return 0L;
}

LPVOID CActiveX::OnSetCommandCallback(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	CActiveX *me = reinterpret_cast<CActiveX*>(GetUserDataLong(hWnd));
	if (me)
	{
		me->CommandCallbackWindow = (HWND)wParam;
		return (LPVOID)1;
	}		
	return 0L;
}

LPVOID CActiveX::OnSetDispatchNotification(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	CActiveX *me = reinterpret_cast<CActiveX*>(GetUserDataLong(hWnd));
	if (me)
	{
		me->DispatchNotificationWindow = (HWND)wParam;
		me->DispatchNotificationMessage = lParam;
		return (LPVOID)1;
	}
	return 0L;
}

LPVOID CActiveX::OnSetServiceProvider(HWND hWnd, WPARAM, LPARAM lParam)
{
	CActiveX *me = reinterpret_cast<CActiveX*>(GetUserDataLong(hWnd));
	if (me)
	{
		me->CustomServiceProvider = (IServiceProvider*)lParam;
		return (LPVOID)1;
	}		
	return 0L;
}

LPVOID CActiveX::OnSetDispatchNotificationFunc(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	CActiveX *me = reinterpret_cast<CActiveX*>(GetUserDataLong(hWnd));
	if (me)
		me->DispatchNotificationFunction = (void(__stdcall*)(CActiveXDispatchNotification*))wParam;
	return 0L;
}

STDMETHODIMP CActiveXClientSite::QueryInterface(REFIID iid, void* *ppvObject)
{
	*ppvObject = 0;

	if (iid == IID_IOleClientSite)
		*ppvObject = (IOleClientSite*)this;

	if (iid == IID_IUnknown)
		*ppvObject = this;

	if (iid == IID_IAdviseSink)
		*ppvObject = (IAdviseSink*)this;

	if (iid == IID_IDispatch)
		*ppvObject = (IDispatch*)this;

	if (iid == SpecialIIDForceOK)
		*ppvObject = (IDispatch*)this;

	if (iid == IID_IServiceProvider)
		*ppvObject = (IServiceProvider*)this;

	if (ExternalPlace == false)
	{
		if (iid == IID_IOleInPlaceSite)
			*ppvObject = (IOleInPlaceSite*)this;

		if (iid == IID_IOleInPlaceFrame)
			*ppvObject = (IOleInPlaceFrame*)this;

		if (iid == IID_IOleInPlaceUIWindow)
			*ppvObject = (IOleInPlaceUIWindow*)this;
	}

	if (*ppvObject)
	{
		AddRef();
		return S_OK;
	}

	return E_NOINTERFACE;
}

STDMETHODIMP_(ULONG) CActiveXClientSite::AddRef()
{
	refNum++;
	return refNum;
}

STDMETHODIMP_(ULONG) CActiveXClientSite::Release()
{
	refNum--;
	return refNum;
}

STDMETHODIMP CActiveXClientSite::QueryService(REFGUID guid, REFIID iid, void **ppvObject)
{
	if (!activex->CustomServiceProvider)
		return E_NOINTERFACE;

	return activex->CustomServiceProvider->QueryService(guid, iid, ppvObject);
}

STDMETHODIMP CActiveXClientSite::SaveObject()
{
	return S_OK;
}

STDMETHODIMP CActiveXClientSite::GetMoniker(DWORD, DWORD, IMoniker **pm)
{
	*pm = 0;
	return E_NOTIMPL;
}

STDMETHODIMP CActiveXClientSite::GetContainer(IOleContainer **pc)
{
	*pc = 0;
	return E_FAIL;
}

STDMETHODIMP CActiveXClientSite::ShowObject()
{
	return S_OK;
}

STDMETHODIMP CActiveXClientSite::OnShowWindow(BOOL)
{
	::InvalidateRect(*this, 0, TRUE);
	::InvalidateRect(GetParent(), 0, TRUE);
	return S_OK;
}

STDMETHODIMP CActiveXClientSite::RequestNewObjectLayout()
{
	return S_OK;
}

STDMETHODIMP_(void) CActiveXClientSite::OnViewChange(DWORD, LONG)
{
}

STDMETHODIMP_(void) CActiveXClientSite::OnRename(IMoniker*)
{
}

STDMETHODIMP_(void) CActiveXClientSite::OnSave()
{
}

STDMETHODIMP_(void) CActiveXClientSite::OnClose()
{
}

STDMETHODIMP CActiveXClientSite::GetWindow(HWND *p)
{
	*p = *this;
	return S_OK;
}

STDMETHODIMP CActiveXClientSite::ContextSensitiveHelp(BOOL)
{
	return E_NOTIMPL;
}

STDMETHODIMP CActiveXClientSite::CanInPlaceActivate()
{
	if (InPlace)
	{
		CalledCanInPlace = true;
		return S_OK;
	}
	return S_FALSE;
}

STDMETHODIMP CActiveXClientSite::OnInPlaceActivate()
{
	return S_OK;
}

STDMETHODIMP CActiveXClientSite::OnUIActivate()
{
	return S_OK;
}

STDMETHODIMP CActiveXClientSite::GetWindowContext(IOleInPlaceFrame **ppFrame, IOleInPlaceUIWindow **ppDoc, LPRECT r1, LPRECT r2, LPOLEINPLACEFRAMEINFO o)
{
	*ppFrame = (IOleInPlaceFrame*)this;
	AddRef();

	*ppDoc = NULL;
	*r1 = GetClientRect();
	*r2 = GetClientRect();
	o->cb = sizeof(OLEINPLACEFRAMEINFO);
	o->fMDIApp = false;
	o->hwndFrame = GetParent();
	o->haccel = 0;
	o->cAccelEntries = 0;

	return S_OK;
}

STDMETHODIMP CActiveXClientSite::Scroll(SIZE)
{
	return E_NOTIMPL;
}

STDMETHODIMP CActiveXClientSite::OnUIDeactivate(int)
{
	return S_OK;
}

STDMETHODIMP CActiveXClientSite::OnInPlaceDeactivate()
{
	return S_OK;
}

STDMETHODIMP CActiveXClientSite::DiscardUndoState()
{
	return S_OK;
}

STDMETHODIMP CActiveXClientSite::DeactivateAndUndo()
{
	return S_OK;
}

STDMETHODIMP CActiveXClientSite::OnPosRectChange(LPCRECT)
{
	return S_OK;
}

STDMETHODIMP CActiveXClientSite::GetBorder(LPRECT l)
{
	*l = GetClientRect();
	return S_OK;
}

STDMETHODIMP CActiveXClientSite::RequestBorderSpace(LPCBORDERWIDTHS)
{
	return E_NOTIMPL;
}

STDMETHODIMP CActiveXClientSite::SetBorderSpace(LPCBORDERWIDTHS)
{
	return S_OK;
}

STDMETHODIMP CActiveXClientSite::SetActiveObject(IOleInPlaceActiveObject*pV, LPCOLESTR)
{
	activex->Pao = pV;
	return S_OK;
}

STDMETHODIMP CActiveXClientSite::SetStatusText(LPCOLESTR)
{
	return E_NOTIMPL;
}

STDMETHODIMP CActiveXClientSite::EnableModeless(BOOL)
{
	return E_NOTIMPL;
}

STDMETHODIMP CActiveXClientSite::TranslateAccelerator(LPMSG, WORD)
{
	return E_NOTIMPL;
}

HRESULT _stdcall CActiveXClientSite::GetTypeInfoCount(UINT*)
{
	return E_NOTIMPL;
}

HRESULT _stdcall CActiveXClientSite::GetTypeInfo(UINT, LCID, ITypeInfo FAR* FAR*)
{
	return E_NOTIMPL;
}

HRESULT _stdcall CActiveXClientSite::GetIDsOfNames(REFIID, OLECHAR FAR* FAR*, UINT, LCID, DISPID FAR*)
{
	return E_NOTIMPL;
}

HRESULT _stdcall CActiveXClientSite::InsertMenus(HMENU, LPOLEMENUGROUPWIDTHS)
{
	return E_NOTIMPL;
}

HRESULT _stdcall CActiveXClientSite::SetMenu(HMENU, HOLEMENU, HWND)
{
	return E_NOTIMPL;
}

HRESULT _stdcall CActiveXClientSite::RemoveMenus(HMENU h)
{
#ifdef WINCE
	return E_NOTIMPL;
#else
	CActiveX *t = (CActiveX*)activex;
	if (t->AddMenu)
	{
		if (!h)
			return S_OK;

		int c = ::GetMenuItemCount(h);
		for (int i = c; i >= 0; i--)
		{
			HMENU hh = ::GetSubMenu(h, i);
			if (hh == GetMenu(*this))
				::RemoveMenu(h, i, MF_BYPOSITION);
		}
		if (h == GetMenu(*this))
			::DestroyMenu(h);

		::DrawMenuBar(GetParent());
		return S_OK;
	}
	return E_NOTIMPL;
#endif
}

HRESULT _stdcall CActiveXClientSite::Invoke(DISPID dispIdMember, REFIID riid, LCID lcid, WORD wFlags, DISPPARAMS FAR *pDispParams, VARIANT FAR *pVarResult, EXCEPINFO FAR *pExcepInfo, UINT FAR *puArgErr)
{
	CActiveXDispatchNotification axd = { activex, dispIdMember, riid, lcid, wFlags, pDispParams, pVarResult, pExcepInfo, puArgErr };

	if (activex->DispatchNotificationWindow)
		::SendMessage(activex->DispatchNotificationWindow, activex->DispatchNotificationMessage, 0, (LPARAM)&axd);

	if (activex->DispatchNotificationFunction)
	{
		activex->DispatchNotificationFunction(&axd);
		return S_OK;
	}

	if (activex->pcmd == 0)
		return S_OK;

	char zv[1000] = { 0 };

	if (dispIdMember == 0xfa && pDispParams->cArgs == 7)
	{
		for (UINT i = 0; i < pDispParams->cArgs; i++)
		{
			if (pDispParams->rgvarg[i].vt == (VT_VARIANT | VT_BYREF))
			{
				VARIANT* x = pDispParams->rgvarg[i].pvarVal;

				if (x->vt == VT_BSTR)
					::WideCharToMultiByte(0, 0, x->bstrVal, -1, zv, 1000, 0, 0);

				if (::strncmp(zv, "app:", 4) == 0)
				{
					for (UINT x = 0; x < pDispParams->cArgs; x++)
					{
						if (pDispParams->rgvarg[x].vt == (VT_BOOL | VT_BYREF))
						{
							VARIANT_BOOL* y = pDispParams->rgvarg[x].pboolVal;
							*y = VARIANT_TRUE;
							break;
						}
					}
					return activex->pcmd(zv, (LPARAM)activex);
					}
				}

			if (pDispParams->rgvarg[i].vt == VT_BSTR)
			{
				VARIANT* x = &pDispParams->rgvarg[i];
				if (x->vt == VT_BSTR)
					::WideCharToMultiByte(0, 0, x->bstrVal, -1, zv, 1000, 0, 0);

				if (::strncmp(zv, "app:", 4) == 0)
				{
					for (UINT x = 0; x < pDispParams->cArgs; x++)
					{
						if (pDispParams->rgvarg[x].vt == (VT_BOOL | VT_BYREF))
						{
							VARIANT_BOOL* y = pDispParams->rgvarg[x].pboolVal;
							*y = VARIANT_TRUE;
							break;
						}
					}
					return activex->pcmd(zv, (LPARAM)activex);
				}
			}
			}
		return S_OK;
		}
	return S_OK;
	}

void _stdcall CActiveXClientSite::OnDataChange(FORMATETC *pFormatEtc, STGMEDIUM *pStgmed)
{
	return;
}