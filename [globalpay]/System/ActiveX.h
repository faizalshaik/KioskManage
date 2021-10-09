/**
* ActiveX in Win32 - ActiveX.h
*
* @program		Global-Pay
* @author		АНКУ ХХК
* @copyright	Copyright (c) 2017 он, АНКУ ХХК.
* @version		0.1.0.0
* @date			2017.09.19
*/

#pragma once

#ifndef WINCE
	#include <exdisp.h>
#endif

#include "../window/wnd.h"

#define CACTIVEXCLASS                     TEXT("codesaur::CActiveX")

#define ACTIVEX_N_SIZE                    1

#define WM_AX_RECREATE                    (WM_USER + 4000)
#define WM_AX_QUERYINTERFACE              (WM_USER + 4001)
#define WM_AX_INPLACE                     (WM_USER + 4002)
#define WM_AX_GETAXINTERFACE              (WM_USER + 4003)
#define WM_AX_CONNECTOBJECT               (WM_USER + 4004)
#define WM_AX_DISCONNECTOBJECT            (WM_USER + 4005)
#define WM_AX_SETDATAADVISE               (WM_USER + 4006)
#define WM_AX_DOVERB                      (WM_USER + 4007)
#define WM_AX_SETCOMMANDCALLBACK          (WM_USER + 4008)
#define WM_AX_SETDISPATCHNOTIFICATION     (WM_USER + 4009)
#define WM_AX_SETSERVICEPROVIDER          (WM_USER + 4010)
#define WM_AX_SETDISPATCHNOTIFICATIONFUNC (WM_USER + 4011)

namespace globalpay {
	typedef struct
	{
		IUnknown                  *Advisor;
		IConnectionPointContainer *icpc;
		IConnectionPoint          *icp;
		DWORD                      ID;
		CLSID                      SpecialIIDForceOK;
	} ActiveXConnectStruct;

	class CActiveXDispatchNotification
	{
	public:
		class CActiveX *ax;

		DISPID			dispIdMember;
		CLSID			riid;
		LCID			lcid;
		WORD			wFlags;
		DISPPARAMS FAR *pDispParams;
		VARIANT FAR    *pVarResult;
		EXCEPINFO FAR  *pExcepInfo;
		UINT FAR       *puArgErr;

		//CActiveXDispatchNotification();
	};

	class CActiveXClientSite :
		public CWnd,
		public IOleClientSite,
		public IServiceProvider,
		public IDispatch,
		public IAdviseSink,
		public IOleInPlaceSite,
		public IOleInPlaceFrame
	{
#define CACTIVEXCLIENTSITECLASS TEXT("codesaur::CActiveXClientSite")
	public:
		bool InPlace;
		bool ExternalPlace;
		bool CalledCanInPlace;
		
		class CActiveX *activex;
		CLSID SpecialIIDForceOK;

		CActiveXClientSite();
		virtual ~CActiveXClientSite();
		STDMETHODIMP_(void) OnDataChange2(FORMATETC*);

		STDMETHODIMP QueryInterface(REFIID, void**);
		STDMETHODIMP_(ULONG) AddRef();
		STDMETHODIMP_(ULONG) Release();

		STDMETHODIMP QueryService(REFGUID, REFIID, void**);

		STDMETHODIMP SaveObject();
		STDMETHODIMP GetMoniker(DWORD, DWORD, IMoniker**);
		STDMETHODIMP GetContainer(IOleContainer**);
		STDMETHODIMP ShowObject();
		STDMETHODIMP OnShowWindow(BOOL);
		STDMETHODIMP RequestNewObjectLayout();

		STDMETHODIMP_(void) OnDataChange(FORMATETC*, STGMEDIUM*);
		STDMETHODIMP_(void) OnViewChange(DWORD, LONG);
		STDMETHODIMP_(void) OnRename(IMoniker*);
		STDMETHODIMP_(void) OnSave();
		STDMETHODIMP_(void) OnClose();

		STDMETHODIMP GetWindow(HWND*);
		STDMETHODIMP ContextSensitiveHelp(BOOL);
		STDMETHODIMP CanInPlaceActivate();
		STDMETHODIMP OnInPlaceActivate();
		STDMETHODIMP OnUIActivate();
		STDMETHODIMP GetWindowContext(IOleInPlaceFrame**, IOleInPlaceUIWindow**, LPRECT, LPRECT, LPOLEINPLACEFRAMEINFO);
		STDMETHODIMP Scroll(SIZE);
		STDMETHODIMP OnUIDeactivate(int);
		STDMETHODIMP OnInPlaceDeactivate();
		STDMETHODIMP DiscardUndoState();
		STDMETHODIMP DeactivateAndUndo();
		STDMETHODIMP OnPosRectChange(LPCRECT);

		STDMETHODIMP GetBorder(LPRECT);
		STDMETHODIMP RequestBorderSpace(LPCBORDERWIDTHS);
		STDMETHODIMP SetBorderSpace(LPCBORDERWIDTHS);
		STDMETHODIMP SetActiveObject(IOleInPlaceActiveObject*, LPCOLESTR);
		STDMETHODIMP InsertMenus(HMENU, LPOLEMENUGROUPWIDTHS);
		STDMETHODIMP SetMenu(HMENU, HOLEMENU, HWND);
		STDMETHODIMP RemoveMenus(HMENU);
		STDMETHODIMP SetStatusText(LPCOLESTR);
		STDMETHODIMP EnableModeless(BOOL);
		STDMETHODIMP TranslateAccelerator(LPMSG, WORD);

		HRESULT _stdcall GetTypeInfoCount(UINT*);
		HRESULT _stdcall GetTypeInfo(UINT, LCID, ITypeInfo FAR* FAR*);
		HRESULT _stdcall GetIDsOfNames(REFIID, OLECHAR FAR* FAR*, UINT, LCID, DISPID FAR*);
		HRESULT _stdcall Invoke(DISPID, REFIID, LCID, WORD, DISPPARAMS FAR*, VARIANT FAR*, EXCEPINFO FAR*, UINT FAR*);

	protected:
		int refNum;
	};

	class CActiveX : public CWnd
	{
	private:
		CLSID     clsid;
		HRESULT (*pcmd)(char*, LPARAM);

	public:
		friend class CActiveXClientSite;

		CLSID GetCLSID();

		CActiveX();

		void Init(LPCTSTR);
		void Clean();

		int  ConnectObject(IUnknown*, REFIID, IUnknown*, IConnectionPointContainer**, IConnectionPoint**);
		void DisconnectObject(IConnectionPointContainer*, IConnectionPoint*, UINT);

		CActiveXClientSite       Site;
		IID                     *iid;
		IOleObject              *OleObject;
		IStorage                *Storage;
		IViewObject             *View;
		IDataObject             *Data;
		IUnknown                *Unk;
		IOleInPlaceActiveObject *Pao;
		IServiceProvider        *CustomServiceProvider;
		ActiveXConnectStruct    *tcs;

		bool                     AddMenu;
		DWORD                    AdviseToken;
		DWORD                    DAdviseToken[100];
		HWND                     CommandCallbackWindow;
		HWND                     DispatchNotificationWindow;

		void (__stdcall *DispatchNotificationFunction)(class CActiveXDispatchNotification*);
		UINT DispatchNotificationMessage;
		void SetProcessCmd(HRESULT(*y)(char*, LPARAM));

	protected:
		static LPVOID CALLBACK OnCreate(HWND, LPARAM);
		static LPVOID CALLBACK OnDestroy(HWND);
		static LPVOID CALLBACK OnCommand(HWND, WPARAM, LPARAM);
		static LPVOID CALLBACK OnMouseDblClick(HWND, UINT, WPARAM, int, int);
		static LPVOID CALLBACK OnSize(HWND, UINT, int, int);

		static LPVOID CALLBACK OnRecreate(HWND, WPARAM, LPARAM);
		static LPVOID CALLBACK OnQueryInterface(HWND, WPARAM, LPARAM);
		static LPVOID CALLBACK OnInPlace(HWND, WPARAM, LPARAM);
		static LPVOID CALLBACK OnGetAXInterface(HWND, WPARAM, LPARAM);
		static LPVOID CALLBACK OnConnectObject(HWND, WPARAM, LPARAM);
		static LPVOID CALLBACK OnDisconnetObject(HWND, WPARAM, LPARAM);
		static LPVOID CALLBACK OnSetDataAdvice(HWND, WPARAM, LPARAM);
		static LPVOID CALLBACK OnDoverb(HWND, WPARAM, LPARAM);
		static LPVOID CALLBACK OnSetCommandCallback(HWND, WPARAM, LPARAM);
		static LPVOID CALLBACK OnSetDispatchNotification(HWND, WPARAM, LPARAM);
		static LPVOID CALLBACK OnSetServiceProvider(HWND, WPARAM, LPARAM);
		static LPVOID CALLBACK OnSetDispatchNotificationFunc(HWND, WPARAM, LPARAM);
	};
}