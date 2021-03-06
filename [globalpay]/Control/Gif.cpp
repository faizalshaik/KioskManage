/**
* Gif.cpp
*
* @program		Global-Pay
* @author		АНКУ ХХК
* @copyright	Copyright (c) 2017 он, АНКУ ХХК.
* @version		0.1.0.0
* @date			2017.09.19
*/

#include "gif.h"

using namespace globalpay;

CGifControl::CGifControl()
	: CWnd(), CCoObject(NULL, COINIT_MULTITHREADED | COINIT_DISABLE_OLE1DDE)
{
	SetClass(CGIFCONTROLCLASS);

	m_pD2DFactory     = NULL;
	m_pHwndRT         = NULL;
	m_pFrameComposeRT = NULL;
	m_pRawFrame       = NULL;
	m_pSavedFrame     = NULL;
	m_pIWICFactory    = NULL;
	m_pDecoder        = NULL;

	onPaint           = OnPaint;
	onTimer           = OnTimer;
}

CGifControl::~CGifControl()
{
	SafeRelease(m_pD2DFactory);
	SafeRelease(m_pHwndRT);
	SafeRelease(m_pFrameComposeRT);
	SafeRelease(m_pRawFrame);
	SafeRelease(m_pSavedFrame);
	SafeRelease(m_pDecoder);

	ReleaseInstance(m_pIWICFactory);
}

BOOL CGifControl::Initialize(HINSTANCE hInst, HWND hParent, RECT rcRect, LPCTSTR szText, DWORD WINSTYLE)
{
	if (!Register(CS_HREDRAW | CS_VREDRAW, hInst, NULL, ::LoadCursor(NULL, IDC_ARROW), ::CreateSolidBrush(RGB(0, 0, 0))))
		return FALSE;

	if (!SUCCEEDED(::D2D1CreateFactory(D2D1_FACTORY_TYPE_MULTI_THREADED, &m_pD2DFactory)))
		return FALSE;
	
	if (!SUCCEEDED(CreateInstance(CLSID_WICImagingFactory, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&m_pIWICFactory))))
		return FALSE;
	
	return Create(szText, WINSTYLE, rcRect, hParent, NULL, hInst, this);
}

void CGifControl::LoadFile(LPCTSTR szFileName)
{
	m_uNextFrameIndex = 0;
	m_uFrameDisposal  = DM_NONE;
	m_uLoopNumber     = 0;
	m_fHasLoop        = false;

	SafeRelease(m_pSavedFrame);
	SafeRelease(m_pDecoder);

	HRESULT hr = m_pIWICFactory->CreateDecoderFromFilename(szFileName, NULL, GENERIC_READ, WICDecodeMetadataCacheOnLoad, &m_pDecoder);
	if (SUCCEEDED(hr))
		hr = GetGlobalMetadata();

	RECT rcWindow = GetClientRect();
	if (SUCCEEDED(hr))
	{
		rcWindow.right = rcWindow.left + m_cxGifImagePixel;
		rcWindow.bottom = rcWindow.top + m_cyGifImagePixel;
//		if (!::AdjustWindowRect(&rcWindow, WS_CHILD, FALSE))
//			hr = ::HRESULT_FROM_WIN32(::GetLastError());
	}

	if (SUCCEEDED(hr))
	{
		SetRect(rcWindow);
		hr = CreateDeviceResources();
	}

	if (SUCCEEDED(hr) && m_cFrames > 0)
	{
		hr = ComposeNextFrame();
		Invalidate();	
	}
}

HRESULT CGifControl::CreateDeviceResources(FLOAT renderDPI)
{
	HRESULT hr;
	RECT rcClient = GetClientRect();
	if (m_pHwndRT == NULL)
	{
		auto renderTargetProperties     = D2D1::RenderTargetProperties();
		renderTargetProperties.dpiX     = renderDPI;
		renderTargetProperties.dpiY     = renderDPI;
		auto hwndRenderTargetproperties = D2D1::HwndRenderTargetProperties(*this, D2D1::SizeU(coord::width(rcClient), coord::height(rcClient)));
		hr                              = m_pD2DFactory->CreateHwndRenderTarget(renderTargetProperties, hwndRenderTargetproperties, &m_pHwndRT);
	}
	else
	{
		D2D1_SIZE_U size;
		size.width  = coord::width(rcClient);
		size.height = coord::height(rcClient);
		hr          = m_pHwndRT->Resize(size);
	}


	if (SUCCEEDED(hr))
	{
		SafeRelease(m_pFrameComposeRT);
		hr = m_pHwndRT->CreateCompatibleRenderTarget(D2D1::SizeF(static_cast<float>(m_cxGifImage), static_cast<float>(m_cyGifImage)), &m_pFrameComposeRT);
	}

	return hr;
}

HRESULT CGifControl::GetRawFrame(UINT uFrameIndex)
{
	IWICFormatConverter *pConverter                    = NULL;
	IWICBitmapFrameDecode *pWicFrame                   = NULL;
	IWICMetadataQueryReader *pFrameMetadataQueryReader = NULL;

	PROPVARIANT propValue;
	::PropVariantInit(&propValue);

	HRESULT hr = m_pDecoder->GetFrame(uFrameIndex, &pWicFrame);
	if (SUCCEEDED(hr))
		hr = m_pIWICFactory->CreateFormatConverter(&pConverter);

	if (SUCCEEDED(hr))
		hr = pConverter->Initialize(pWicFrame, GUID_WICPixelFormat32bppPBGRA, WICBitmapDitherTypeNone, NULL, 0.f, WICBitmapPaletteTypeCustom);

	if (SUCCEEDED(hr))
	{
		SafeRelease(m_pRawFrame);
		hr = m_pHwndRT->CreateBitmapFromWicBitmap(pConverter, NULL, &m_pRawFrame);
	}

	if (SUCCEEDED(hr))
		hr = pWicFrame->GetMetadataQueryReader(&pFrameMetadataQueryReader);

	if (SUCCEEDED(hr))
	{
		hr = pFrameMetadataQueryReader->GetMetadataByName(TEXT("/imgdesc/Left"), &propValue);
		if (SUCCEEDED(hr))
		{
			hr = (propValue.vt == VT_UI2 ? S_OK : E_FAIL);
			if (SUCCEEDED(hr))
				m_framePosition.left = static_cast<float>(propValue.uiVal);

			::PropVariantClear(&propValue);
		}
	}

	if (SUCCEEDED(hr))
	{
		hr = pFrameMetadataQueryReader->GetMetadataByName(TEXT("/imgdesc/Top"), &propValue);
		if (SUCCEEDED(hr))
		{
			hr = (propValue.vt == VT_UI2 ? S_OK : E_FAIL);
			if (SUCCEEDED(hr))
				m_framePosition.top = static_cast<float>(propValue.uiVal);

			::PropVariantClear(&propValue);
		}
	}

	if (SUCCEEDED(hr))
	{
		hr = pFrameMetadataQueryReader->GetMetadataByName(TEXT("/imgdesc/Width"), &propValue);
		if (SUCCEEDED(hr))
		{
			hr = (propValue.vt == VT_UI2 ? S_OK : E_FAIL);
			if (SUCCEEDED(hr))
				m_framePosition.right = static_cast<float>(propValue.uiVal) + m_framePosition.left;

			::PropVariantClear(&propValue);
		}
	}

	if (SUCCEEDED(hr))
	{
		hr = pFrameMetadataQueryReader->GetMetadataByName(TEXT("/imgdesc/Height"), &propValue);
		if (SUCCEEDED(hr))
		{
			hr = (propValue.vt == VT_UI2 ? S_OK : E_FAIL);
			if (SUCCEEDED(hr))
				m_framePosition.bottom = static_cast<float>(propValue.uiVal) + m_framePosition.top;

			::PropVariantClear(&propValue);
		}
	}

	if (SUCCEEDED(hr))
	{
		if (SUCCEEDED(pFrameMetadataQueryReader->GetMetadataByName(TEXT("/grctlext/Delay"), &propValue)))
		{
			hr = (propValue.vt == VT_UI2 ? S_OK : E_FAIL);
			if (SUCCEEDED(hr))
				hr = ::UIntMult(propValue.uiVal, 10, &m_uFrameDelay);

			::PropVariantClear(&propValue);
		}
		else
			m_uFrameDelay = 0;

		if (SUCCEEDED(hr) && m_uFrameDelay < 90)
			m_uFrameDelay = 90;
	}

	if (SUCCEEDED(hr))
	{
		if (SUCCEEDED(pFrameMetadataQueryReader->GetMetadataByName(TEXT("/grctlext/Disposal"), &propValue)))
		{
			hr = (propValue.vt == VT_UI1) ? S_OK : E_FAIL;
			if (SUCCEEDED(hr))
				m_uFrameDisposal = propValue.bVal;
		}
		else
			m_uFrameDisposal = DM_UNDEFINED;
	}

	::PropVariantClear(&propValue);

	SafeRelease(pConverter);
	SafeRelease(pWicFrame);
	SafeRelease(pFrameMetadataQueryReader);

	return hr;
}

HRESULT CGifControl::GetGlobalMetadata()
{
	PROPVARIANT              propValue;
	::PropVariantInit(&propValue);
	IWICMetadataQueryReader *pMetadataQueryReader = NULL;
	HRESULT                  hr                   = m_pDecoder->GetFrameCount(&m_cFrames);
	if (SUCCEEDED(hr))
		hr = m_pDecoder->GetMetadataQueryReader(&pMetadataQueryReader);

	if (SUCCEEDED(hr))
	{
		if (FAILED(GetBackgroundColor(pMetadataQueryReader)))
			m_backgroundColor = D2D1::ColorF(0, 0.f);

		hr = pMetadataQueryReader->GetMetadataByName(TEXT("/logscrdesc/Width"), &propValue);
		if (SUCCEEDED(hr))
		{
			hr = (propValue.vt == VT_UI2 ? S_OK : E_FAIL);
			if (SUCCEEDED(hr))
				m_cxGifImage = propValue.uiVal;

			::PropVariantClear(&propValue);
		}
	}

	if (SUCCEEDED(hr))
	{
		hr = pMetadataQueryReader->GetMetadataByName(TEXT("/logscrdesc/Height"), &propValue);
		if (SUCCEEDED(hr))
		{
			hr = (propValue.vt == VT_UI2 ? S_OK : E_FAIL);
			if (SUCCEEDED(hr))
				m_cyGifImage = propValue.uiVal;

			::PropVariantClear(&propValue);
		}
	}

	if (SUCCEEDED(hr))
	{
		hr = pMetadataQueryReader->GetMetadataByName(TEXT("/logscrdesc/PixelAspectRatio"), &propValue);
		if (SUCCEEDED(hr))
		{
			hr = (propValue.vt == VT_UI1 ? S_OK : E_FAIL);
			if (SUCCEEDED(hr))
			{
				UINT uPixelAspRatio = propValue.bVal;
				if (uPixelAspRatio != 0)
				{
					float pixelAspRatio = (uPixelAspRatio + 15.f) / 64.f;
					if (pixelAspRatio > 1.f)
					{
						m_cxGifImagePixel = m_cxGifImage;
						m_cyGifImagePixel = static_cast<unsigned int>(m_cyGifImage / pixelAspRatio);
					}
					else
					{
						m_cxGifImagePixel = static_cast<unsigned int>(m_cxGifImage * pixelAspRatio);
						m_cyGifImagePixel = m_cyGifImage;
					}
				}
				else
				{
					m_cxGifImagePixel = m_cxGifImage;
					m_cyGifImagePixel = m_cyGifImage;
				}
			}
			::PropVariantClear(&propValue);
		}
	}

	if (SUCCEEDED(hr))
	{
		if (SUCCEEDED(pMetadataQueryReader->GetMetadataByName(TEXT("/appext/application"), &propValue))
			&& propValue.vt == (VT_UI1 | VT_VECTOR) && propValue.caub.cElems == 11
			&& (!::memcmp(propValue.caub.pElems, "NETSCAPE2.0", propValue.caub.cElems)
			|| !::memcmp(propValue.caub.pElems, "ANIMEXTS1.0", propValue.caub.cElems)))
		{
			::PropVariantClear(&propValue);
			hr = pMetadataQueryReader->GetMetadataByName(TEXT("/appext/data"), &propValue);
			if (SUCCEEDED(hr))
			{
				if (propValue.vt == (VT_UI1 | VT_VECTOR)
					&& propValue.caub.cElems >= 4
					&& propValue.caub.pElems[0] > 0
					&& propValue.caub.pElems[1] == 1)
				{
					m_uTotalLoopCount = MAKEWORD(propValue.caub.pElems[2], propValue.caub.pElems[3]);
					if (m_uTotalLoopCount != 0)
					{
						m_fHasLoop = true;
					}
				}
			}
		}
	}
	::PropVariantClear(&propValue);
	
	SafeRelease(pMetadataQueryReader);

	return hr;
}

HRESULT CGifControl::GetBackgroundColor(IWICMetadataQueryReader *pMetadataQueryReader)
{
	DWORD        dwBGColor;
	BYTE         backgroundIndex = 0;
	WICColor     rgColors[256];
	UINT         cColorsCopied   = 0;
	PROPVARIANT  propVariant;
	::PropVariantInit(&propVariant);
	IWICPalette *pWicPalette     = NULL;

	HRESULT hr = pMetadataQueryReader->GetMetadataByName(TEXT("/logscrdesc/GlobalColorTableFlag"), &propVariant);
	if (SUCCEEDED(hr))
	{
		hr = (propVariant.vt != VT_BOOL || !propVariant.boolVal) ? E_FAIL : S_OK;
		::PropVariantClear(&propVariant);
	}

	if (SUCCEEDED(hr))
	{
		hr = pMetadataQueryReader->GetMetadataByName(TEXT("/logscrdesc/BackgroundColorIndex"), &propVariant);
		if (SUCCEEDED(hr))
		{
			hr = (propVariant.vt != VT_UI1) ? E_FAIL : S_OK;
			if (SUCCEEDED(hr))
				backgroundIndex = propVariant.bVal;

			::PropVariantClear(&propVariant);
		}
	}

	if (SUCCEEDED(hr))
		hr = m_pIWICFactory->CreatePalette(&pWicPalette);

	if (SUCCEEDED(hr))
		hr = m_pDecoder->CopyPalette(pWicPalette);

	if (SUCCEEDED(hr))
		hr = pWicPalette->GetColors(ARRAYSIZE(rgColors), rgColors, &cColorsCopied);

	if (SUCCEEDED(hr))
		hr = (backgroundIndex >= cColorsCopied) ? E_FAIL : S_OK;

	if (SUCCEEDED(hr))
	{
		dwBGColor         = rgColors[backgroundIndex];
		float alpha       = (dwBGColor >> 24) / 255.f;
		m_backgroundColor = D2D1::ColorF(dwBGColor, alpha);
	}

	SafeRelease(pWicPalette);

	return hr;
}

HRESULT CGifControl::ComposeNextFrame()
{
	HRESULT hr = S_OK;
	if (m_pHwndRT && m_pFrameComposeRT)
	{
		KillTimer(WM_DELAY_TIMER);

		hr = DisposeCurrentFrame();
		if (SUCCEEDED(hr))
			hr = OverlayNextFrame();

		while (SUCCEEDED(hr) && m_uFrameDelay == 0 && !IsLastFrame())
		{
			hr = DisposeCurrentFrame();
			if (SUCCEEDED(hr))
				hr = OverlayNextFrame();
		}

		if (!EndOfAnimation() && m_cFrames > 1)
			SetTimer(WM_DELAY_TIMER, m_uFrameDelay);
	}
	return hr;
}

HRESULT CGifControl::DisposeCurrentFrame()
{
	HRESULT hr = S_OK;
	switch (m_uFrameDisposal)
	{
	case DM_UNDEFINED:
	case DM_NONE:                                     break;
	case DM_BACKGROUND: hr = ClearCurrentFrameArea(); break;
	case DM_PREVIOUS:   hr = RestoreSavedFrame();     break;
	default:            hr = E_FAIL;
	}
	return hr;
}

HRESULT CGifControl::OverlayNextFrame()
{
	HRESULT hr = GetRawFrame(m_uNextFrameIndex);
	if (SUCCEEDED(hr) && m_uFrameDisposal == DM_PREVIOUS)
		hr = SaveComposedFrame();

	if (SUCCEEDED(hr))
	{
		m_pFrameComposeRT->BeginDraw();

		if (m_uNextFrameIndex == 0)
		{
			m_pFrameComposeRT->Clear(m_backgroundColor);
			m_uLoopNumber++;
		}

		m_pFrameComposeRT->DrawBitmap(m_pRawFrame, m_framePosition);

		hr = m_pFrameComposeRT->EndDraw();
	}

	if (SUCCEEDED(hr))
		m_uNextFrameIndex = (++m_uNextFrameIndex) % m_cFrames;

	return hr;
}

HRESULT CGifControl::SaveComposedFrame()
{
	ID2D1Bitmap *pFrameToBeSaved = NULL;
	HRESULT      hr              = m_pFrameComposeRT->GetBitmap(&pFrameToBeSaved);
	if (SUCCEEDED(hr) && m_pSavedFrame == NULL)
	{
		auto bitmapSize        = pFrameToBeSaved->GetPixelSize();
		D2D1_BITMAP_PROPERTIES bitmapProp;
		pFrameToBeSaved->GetDpi(&bitmapProp.dpiX, &bitmapProp.dpiY);
		bitmapProp.pixelFormat = pFrameToBeSaved->GetPixelFormat();
		hr                     = m_pFrameComposeRT->CreateBitmap(bitmapSize, bitmapProp, &m_pSavedFrame);
	}

	if (SUCCEEDED(hr))
		hr = m_pSavedFrame->CopyFromBitmap(NULL, pFrameToBeSaved, NULL);

	SafeRelease(pFrameToBeSaved);

	return hr;
}

HRESULT CGifControl::RestoreSavedFrame()
{
	ID2D1Bitmap *pFrameToCopyTo = NULL;
	HRESULT      hr             = m_pSavedFrame ? S_OK : E_FAIL;
	if (SUCCEEDED(hr))
		hr = m_pFrameComposeRT->GetBitmap(&pFrameToCopyTo);

	if (SUCCEEDED(hr))
		hr = pFrameToCopyTo->CopyFromBitmap(NULL, m_pSavedFrame, NULL);

	SafeRelease(pFrameToCopyTo);

	return hr;
}

HRESULT CGifControl::ClearCurrentFrameArea()
{
	m_pFrameComposeRT->BeginDraw();
	m_pFrameComposeRT->PushAxisAlignedClip(&m_framePosition, D2D1_ANTIALIAS_MODE_PER_PRIMITIVE);
	m_pFrameComposeRT->Clear(m_backgroundColor);
	m_pFrameComposeRT->PopAxisAlignedClip();

	return m_pFrameComposeRT->EndDraw();
}

bool CGifControl::IsLastFrame()
{
	return (m_uNextFrameIndex == 0);
}

bool CGifControl::EndOfAnimation()
{
	return m_fHasLoop && IsLastFrame() && m_uLoopNumber == m_uTotalLoopCount + 1;
}

HRESULT CGifControl::CalculateDrawRectangle(D2D1_RECT_F &drawRect)
{
	RECT rcClient    = GetClientRect();

	drawRect.left    = (static_cast<float>(rcClient.right) - m_cxGifImagePixel) / 2.f;
	drawRect.top     = (static_cast<float>(rcClient.bottom) - m_cyGifImagePixel) / 2.f;
	drawRect.right   = drawRect.left + m_cxGifImagePixel;
	drawRect.bottom  = drawRect.top + m_cyGifImagePixel;

	auto aspectRatio = static_cast<float>(m_cxGifImagePixel) / static_cast<float>(m_cyGifImagePixel);

	if (drawRect.left < 0)
	{
		auto newWidth   = static_cast<float>(rcClient.right);
		float newHeight = newWidth / aspectRatio;
		drawRect.left   = 0;
		drawRect.top    = (static_cast<float>(rcClient.bottom) - newHeight) / 2.f;
		drawRect.right  = newWidth;
		drawRect.bottom = drawRect.top + newHeight;
	}

	if (drawRect.top < 0)
	{
		auto newHeight  = static_cast<float>(rcClient.bottom);
		float newWidth  = newHeight * aspectRatio;
		drawRect.left   = (static_cast<float>(rcClient.right) - newWidth) / 2.f;
		drawRect.top    = 0;
		drawRect.right  = drawRect.left + newWidth;
		drawRect.bottom = newHeight;
	}

	return S_OK;
}

LPVOID CGifControl::OnPaint(HWND hWnd, HDC hDC, PAINTSTRUCT pStruct)
{
	CGifControl *gif = reinterpret_cast<CGifControl*>(GetUserDataLong(hWnd));	
	if (gif)
	{
		if (gif->m_pHwndRT && gif->m_pFrameComposeRT)
		{
			if (!(gif->m_pHwndRT->CheckWindowState() & D2D1_WINDOW_STATE_OCCLUDED))
			{
				D2D1_RECT_F  drawRect;
				HRESULT      hr             = gif->CalculateDrawRectangle(drawRect);
				ID2D1Bitmap *pFrameToRender = NULL;
				if (SUCCEEDED(hr))
					hr = gif->m_pFrameComposeRT->GetBitmap(&pFrameToRender);

				if (SUCCEEDED(hr))
				{
					gif->m_pHwndRT->BeginDraw();
					gif->m_pHwndRT->Clear(gif->m_backgroundColor);
					gif->m_pHwndRT->DrawBitmap(pFrameToRender, drawRect);
					gif->m_pHwndRT->EndDraw();
				}
				SafeRelease(pFrameToRender);
			}
		}
	}
	return 0L;
}

LPVOID CGifControl::OnTimer(HWND hWnd, UINT uTimerId)
{
	CGifControl *gif = reinterpret_cast<CGifControl*>(GetUserDataLong(hWnd));
	if (gif)
	{
		if (SUCCEEDED(gif->ComposeNextFrame()))
			gif->Invalidate();
	}
	return 0L;
}