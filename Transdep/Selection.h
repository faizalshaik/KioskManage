#pragma once

#include "../config.hpp"

using namespace globalpay;

class Selection
{
public:
	Selection();

	static LPVOID CALLBACK Painter(HWND hWnd, HDC hDC, PAINTSTRUCT pStruct);
	static void drawBox(HWND OwnerWnd, HDC hDC, PAINTSTRUCT pStruct, int xStart, int yStart, int width, int height);
};