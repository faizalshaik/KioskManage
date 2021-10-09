// original name : glWnd.cpp
// more info @   : glWnd.h

#include "glwnd.h"

using namespace codesaur;

glWnd::glWnd()
: CWnd()
{
	SetClass(CGLWNDCLASS);
	
	onCreate = OnCreate;
	onDestroy = OnDestroy;
	onSize = OnSize;

	SetPixelFormatDesc(&pfd);
}

glWnd::~glWnd()
{
}

LPVOID glWnd::OnCreate(HWND hWnd, LPARAM lParam)
{
	glWnd *glWindow = reinterpret_cast<glWnd*>(GetUserDataLongPtr(hWnd));
	if (glWindow)
	{
		glWindow->SetGLDC();
		glWindow->SetupPixelFormat();
		glWindow->CreateRenderingContext();
		glWindow->MakeCurrentContext();
	}
	return 0;
}

LPVOID glWnd::OnDestroy(HWND hWnd)
{
	glWnd *glWindow = reinterpret_cast<glWnd*>(GetUserDataLongPtr(hWnd));
	if (glWindow)
	{
		glWindow->DeselectContext();
		glWindow->DeleteContext();
	}
	return 0;
}

LPVOID glWnd::OnSize(HWND hWnd, UINT nType, int cx, int cy)
{
	::glViewport(0, 0, cx, cy);
	return 0;
}

bool glWnd::InitWindow(HINSTANCE hInst, HWND hParent, LPCTSTR szText, RECT rcRect, DWORD WINSTYLE)
{
	if (!Register(CS_HREDRAW|CS_VREDRAW, hInst))
		return false;

	if (!Create(szText, WINSTYLE, rcRect, hParent, NULL, hInst, this))
		return false;

	return true;
}

void glWnd::SetGLDC()
{
	glDC = ::GetDC(GetHandle());
}

void glWnd::SetupPixelFormat()
{
    int nPixelFormat = ::ChoosePixelFormat(glDC, &pfd);
	::SetPixelFormat(glDC, nPixelFormat, &pfd);
}

void glWnd::SetPixelFormatDesc(PIXELFORMATDESCRIPTOR* pfdToSet,
	DWORD dwFlags, BYTE iPixelType, BYTE cColorBits, BYTE cRedBits, BYTE cRedShift, BYTE cGreenBits, BYTE cGreenShift, BYTE cBlueBits, BYTE cBlueShift,
	BYTE cAlphaBits, BYTE cAlphaShift, BYTE cAccumBits, BYTE cAccumRedBits, BYTE cAccumGreenBits, BYTE cAccumBlueBits, BYTE cAccumAlphaBits,
	BYTE cDepthBits, BYTE cStencilBits, BYTE cAuxBuffers, BYTE iLayerType, BYTE bReserved, DWORD dwLayerMask, DWORD dwVisibleMask, DWORD dwDamageMask)
{
	pfdToSet->nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfdToSet->nVersion = 1;
	pfdToSet->dwFlags = dwFlags;
	pfdToSet->iPixelType = iPixelType;
	pfdToSet->cColorBits = cColorBits;
	pfdToSet->cRedBits = cRedBits;
	pfdToSet->cRedShift = cRedShift;
	pfdToSet->cGreenBits = cGreenBits;
	pfdToSet->cGreenShift = cGreenShift;
	pfdToSet->cBlueBits = cBlueBits;
	pfdToSet->cBlueShift = cBlueShift;
	pfdToSet->cAlphaBits = cAlphaBits;
	pfdToSet->cAlphaShift = cAlphaShift;
	pfdToSet->cAccumBits = cAccumBits;
	pfdToSet->cAccumRedBits = cAccumRedBits;
	pfdToSet->cAccumGreenBits = cAccumGreenBits;
	pfdToSet->cAccumBlueBits = cAccumBlueBits;
	pfdToSet->cAccumAlphaBits = cAccumAlphaBits;
	pfdToSet->cDepthBits = cDepthBits;
	pfdToSet->cStencilBits = cStencilBits;
	pfdToSet->cAuxBuffers = cAuxBuffers;
	pfdToSet->iLayerType = iLayerType;
	pfdToSet->bReserved = bReserved;
	pfdToSet->dwLayerMask = dwLayerMask;
	pfdToSet->dwVisibleMask = dwVisibleMask;
	pfdToSet->dwDamageMask = dwDamageMask;
}

void glWnd::CreateRenderingContext()
{
	glRC = ::wglCreateContext(glDC);
}

void glWnd::MakeCurrentContext()
{
	::wglMakeCurrent(glDC, glRC);
}

void glWnd::DeselectContext()
{
	::wglMakeCurrent(glDC, NULL);
}

void glWnd::DeleteContext()
{
	::wglDeleteContext(glRC);
}

void glWnd::SwapBuffers()
{
	::SwapBuffers(glDC);
}