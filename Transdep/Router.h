#pragma once

#include "types/bitmap.h"
#include "window/hoverbitmapwnd.h"
#include "control/button.h"
#include "control/panel.h"
#include "control/static.h"

#include "../config.hpp"
#include "../transdep/control.h"

using namespace globalpay;

namespace transdep {
#define MENU_WITH 400

	typedef enum
	{
		Enabled,
		Disabled
	} SelectionState;

	class Province : public CWnd
	{
	public:
		Province();

		UINT ID;
		bool isCity;
		SelectionState sState;

	protected:
		static LPVOID CALLBACK OnPaint(HWND hWnd, HDC hDC, PAINTSTRUCT pStruct);
		static LPVOID CALLBACK OnMouseDown(HWND hWnd, UINT mButton, WPARAM sState, int X, int Y);
	};

	class Destination : public CWnd
	{
	public:
		Destination();

		UINT ID;

	protected:
		static LPVOID CALLBACK OnPaint(HWND hWnd, HDC hDC, PAINTSTRUCT pStruct);
		static LPVOID CALLBACK OnMouseDown(HWND hWnd, UINT mButton, WPARAM sState, int X, int Y);
	};

	class TransparentStatic : public CStatic
	{
	public:
		TransparentStatic();

	protected:
		static LPVOID CALLBACK OnEraseBackground(HWND hWnd, HDC hDC);
	};

	class Menu : public CPanel
	{
	public:
		Menu();
		~Menu();

		TransparentStatic aimag, khot, gadaad;
		Province *province, *city, *foreign;

	protected:
		static LPVOID CALLBACK OnPaint(HWND hWnd, HDC hDC, PAINTSTRUCT pStruct);
		static LPVOID CALLBACK OnMenuShow(HWND hWnd, UINT);
		static LPVOID  CALLBACK OnCtlColorStatic(HWND hWnd, HDC hStaticDC, HWND hStaticCtl);
		const UINT Aimags();
	};

	class Work : public CPanel
	{
	public:
		Work();
		~Work();

		Destination *stops;
	};

	class Router : public CBitmapWindow
	{
	public:
		Router();
		void setVisible(bool bVisible);

		Menu menu;
		Work work;

		bool Init(HINSTANCE, HWND, RECT);
	};
}