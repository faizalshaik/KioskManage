#pragma once

#include "window/hoverbitmapwnd.h"
#include "control/panel.h"

#include "../config.hpp"

typedef enum
{
	fbpLeft,
	fbpRight,
	fbpCenter,
	fbpUp,
	fbpDown
} FooterButtonPicturePosition;

class CFooterButton : public CHoverBitmapWindow
{
public:
	CFooterButton();
	~CFooterButton();

	FooterButtonPicturePosition
		    pPos;
	HBRUSH  Background;
	HBITMAP Custom;
	
	void ShowText(LPCTSTR);
	void SetColor(HBRUSH);
	CFooterButton* Reset();

protected:
	static LPVOID CALLBACK OnPaint(HWND OwnerWnd, HDC hDC, PAINTSTRUCT pStruct);
	static LPVOID CALLBACK OnEraseBackground(HWND hWnd, HDC hDC);
};

class CHiddenExit : public CWnd
{
#define CAN_UNLOCK(x) x > 3
public:
	CHiddenExit();

	UINT Unlocking;
	std::wstring Key;

protected:
	static LPVOID CALLBACK OnPaint(HWND hWnd, HDC hDC, PAINTSTRUCT pStruct);
	static LPVOID CALLBACK OnMouseDown(HWND hWnd, UINT mButton, WPARAM sState, int X, int Y);
	static LPVOID CALLBACK OnMouseLeave(HWND hWnd);
};

class CFooter : public CPanel
{
public:
	CFooter();

	CFooterButton left;
	CFooterButton right;
	CHiddenExit   middle;

	void Show();
	void InitButtons();
	void HideButtons();
	void Left(LPCTSTR);
	void Right(LPCTSTR);

protected:
	static LPVOID CALLBACK OnHide(HWND hWnd, UINT);
	static LPVOID CALLBACK OnCreate(HWND hWnd, LPARAM lParam);

	static void LeftClicked(HWND);
	static void RightClicked(HWND);
};