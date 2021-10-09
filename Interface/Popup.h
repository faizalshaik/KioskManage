#pragma once

#include "control/panel.h"

#include "../config.hpp"

using namespace globalpay;

class PopupBackground : public CWnd
{
public:
	PopupBackground();
};

class Menu1 : public CPanel
{
public:
	Menu1();

private:
	static LPVOID CALLBACK OnPopupShow(HWND, UINT);
};

class Popup : public CPanel
{
public:
	Popup();

	Menu1 menu1;

	bool Init(HINSTANCE, HWND, RECT);
};