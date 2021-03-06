/**
* CGif control class - Gif.h - Initial
*
* @program		Global-Pay
* @author		АНКУ ХХК
* @copyright	Copyright (c) 2017 он, АНКУ ХХК.
* @version		0.1.0.0
* @date			2017.09.19
*/

#pragma once

#include "../window/wnd.h"
#include "../system/coobject.h"

namespace globalpay {
#define CGIFCONTROLCLASS TEXT("codesaur::gifcontrol")
#define GIF_DEFAULT_DPI  96.f

	class CGifControl : public CWnd, public CCoObject
	{
	public:
		CGifControl();
		~CGifControl();

		BOOL    Initialize(HINSTANCE, HWND , RECT rcRect = coord::to_rect(20, 10, 200, 100), LPCTSTR szText = TEXT("Gif"), DWORD WINSTYLE = WS_CHILD);
		void    LoadFile(LPCTSTR);
		
		HRESULT CreateDeviceResources(FLOAT renderDPI = GIF_DEFAULT_DPI);

		HRESULT GetRawFrame(UINT);
		HRESULT GetGlobalMetadata();
		HRESULT GetBackgroundColor(IWICMetadataQueryReader *);

		HRESULT ComposeNextFrame();
		HRESULT DisposeCurrentFrame();
		HRESULT OverlayNextFrame();

		HRESULT SaveComposedFrame();
		HRESULT RestoreSavedFrame();
		HRESULT ClearCurrentFrameArea();

		bool    IsLastFrame();
		bool    EndOfAnimation();

		HRESULT CalculateDrawRectangle(D2D1_RECT_F &);

	private:
		typedef enum
		{
			DM_UNDEFINED  = 0,
			DM_NONE       = 1,
			DM_BACKGROUND = 2,
			DM_PREVIOUS   = 3
		} DISPOSAL_METHODS;

		ID2D1Factory            *m_pD2DFactory;
		ID2D1HwndRenderTarget   *m_pHwndRT;
		ID2D1BitmapRenderTarget *m_pFrameComposeRT;
		ID2D1Bitmap             *m_pRawFrame;
		ID2D1Bitmap             *m_pSavedFrame;
		D2D1_COLOR_F             m_backgroundColor;

		IWICImagingFactory      *m_pIWICFactory;
		IWICBitmapDecoder       *m_pDecoder;

		unsigned int             m_uNextFrameIndex;
		unsigned int             m_uTotalLoopCount;
		unsigned int             m_uLoopNumber;
		bool                     m_fHasLoop;
		unsigned int             m_cFrames;
		unsigned int             m_uFrameDisposal;
		unsigned int             m_uFrameDelay;
		unsigned int             m_cxGifImage;
		unsigned int             m_cyGifImage;
		unsigned int             m_cxGifImagePixel;
		unsigned int             m_cyGifImagePixel;

		D2D1_RECT_F              m_framePosition;

	protected:
		static LPVOID CALLBACK OnPaint(HWND, HDC, PAINTSTRUCT);
		static LPVOID CALLBACK OnTimer(HWND hWnd, UINT uTimerId);
	};
}