#pragma once

#include "window/hoverbitmapwnd.h"

#include "../config.hpp"

using namespace globalpay;

class CItem : public CHoverBitmapWindow
{
public:
	CItem();
	CItem* Init(LPCTSTR szText, void(*OnClickEvent)(HWND), HBITMAP hBmp = NULL, HINSTANCE hInst = NULL, HWND hParent = NULL);
};

class CBarMenu: public CBitmapWindow
{
public:
	CBarMenu();

	CHoverBitmapWindow AppVideo, KioskLocation;

	bool Initialize(HWND, RECT);
	
	bool PaintBitmap(HDC);

protected:
	static LPVOID CALLBACK OnPaint(HWND, HDC, PAINTSTRUCT);
	static void OnKioskLocation(HWND);
};

class CMainMenu
{
public:
	CMainMenu();
	~CMainMenu();

	CBarMenu bar;

	void    Create(HWND);
	void    Show(bool bLogoOnly = false);
	void    Hide();
	void    AllocItems(UINT);
	void    SetItemBitmap(LPCTSTR, HBITMAP);
	void    DeleteItems();
	CItem*  GetItemBy(LPCTSTR);
	UINT    GetItems();
	void    Position();
	void    SetLanguage();

private:
	CHoverBitmapWindow
		   logo, flag;
	CItem *items;
	UINT   count;

protected:
	static void OnLogoClick(HWND);
	static void OnFlagClick(HWND);
	static void OnTransdepClick(HWND);
	static void OnRailwayClick(HWND);
	static void OnFlightClick(HWND);
	static void OnTorguuliClick(HWND);
	static void OnUMoneyClick(HWND);
};