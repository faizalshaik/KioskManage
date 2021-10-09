/*
>>> OpenGL window obj

	founder : coden aka Narankhuu N.
	mailto  : codesaur@gmail.com
  
	Copyright © 2012 Metatech LLC.
	All rights reserved.

>>> 2012.01.10 - glWnd.h
*/

#pragma once

#include "../window/wnd.h"

#include <gl/gl.h>

namespace codesaur {
#define CGLWNDCLASS TEXT("cdn::glwnd")

	class glWnd : public CWnd
	{
	public:
		glWnd();
		~glWnd();

		HDC                   glDC;
		HGLRC                 glRC;
		PIXELFORMATDESCRIPTOR pfd;

		bool InitWindow(HINSTANCE, HWND, LPCTSTR, RECT, DWORD WINSTYLE = WS_CHILD);

		void SetGLDC();
		void SetupPixelFormat();
		void SetPixelFormatDesc(PIXELFORMATDESCRIPTOR*,
			DWORD dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
			BYTE iPixelType = PFD_TYPE_RGBA,
			BYTE cColorBits = 32,
			BYTE cRedBits = 0, BYTE cRedShift = 0, BYTE cGreenBits = 0, BYTE cGreenShift = 0, BYTE cBlueBits = 0, BYTE cBlueShift = 0,
			BYTE cAlphaBits = 0,
			BYTE cAlphaShift = 0,
			BYTE cAccumBits = 0,
			BYTE cAccumRedBits = 0, BYTE cAccumGreenBits = 0, BYTE cAccumBlueBits = 0, BYTE cAccumAlphaBits = 0,
			BYTE cDepthBits = 16,
			BYTE cStencilBits = 0,
			BYTE cAuxBuffers = 0,
			BYTE iLayerType = PFD_MAIN_PLANE,
			BYTE bReserved = 0,
			DWORD dwLayerMask = 0, DWORD dwVisibleMask = 0, DWORD dwDamageMask = 0);

		void CreateRenderingContext();
		void MakeCurrentContext();
		void DeselectContext();
		void DeleteContext();

		void SwapBuffers();

		static LPVOID CALLBACK OnCreate(HWND, LPARAM);
		static LPVOID CALLBACK OnDestroy(HWND);
		static LPVOID CALLBACK OnSize(HWND, UINT, int, int);
	};
}