#pragma once

#include "control/panel.h"
#include "control/static.h"
#include "control/button.h"

#include "../config.hpp"
#include "../transdep/control.h"

using namespace codesaur;

namespace transdep {

	class Province2 : public CWnd
	{
	public:
		Province2();

		UINT ID;
		bool isCity;

	protected:
		static LPVOID CALLBACK OnPaint(HWND hWnd, HDC hDC, PAINTSTRUCT pStruct);
		static LPVOID CALLBACK OnMouseDown(HWND hWnd, UINT mButton, WPARAM sState, int X, int Y);
	};

	class Selection2 : public CWnd
	{
	public:
		Selection2();

		UINT ID;

	protected:
		static LPVOID CALLBACK OnPaint(HWND hWnd, HDC hDC, PAINTSTRUCT pStruct);
		static LPVOID CALLBACK OnMouseDown(HWND hWnd, UINT mButton, WPARAM sState, int X, int Y);
	};

	class TransparentStatic2 : public CStatic
	{
	public:
		TransparentStatic2();

	protected:
		static LPVOID CALLBACK OnEraseBackground(HWND hWnd, HDC hDC);
	};

	class Menu2 : public CPanel
	{
	public:
		Menu2();
		~Menu2();

		TransparentStatic2 aimag, khot, gadaad;
		Province2 *province, *city, *foreign;

	protected:
		static LPVOID CALLBACK OnPaint(HWND hWnd, HDC hDC, PAINTSTRUCT pStruct);
		static LPVOID CALLBACK OnMenuShow(HWND hWnd, UINT);
		static LPVOID  CALLBACK OnCtlColorStatic(HWND hWnd, HDC hStaticDC, HWND hStaticCtl);
		const UINT Aimags();
	};

	class Work2 : public CPanel
	{
	public:
		Work2();
		~Work2();

		Selection2 *stops;
	};

	class Popup1 : public CPanel
	{
	public:
		Popup1();

		Menu2 menu2;
		Work2 work2;

		bool Init(HINSTANCE, HWND, RECT);
	};
}