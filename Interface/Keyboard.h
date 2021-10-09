#pragma once

#include "window/hoverbitmapwnd.h"
#include "control/panel.h"
#include "control/edit.h"

#include "../config.hpp"

using namespace globalpay;

typedef enum
{
	keyNumber    = 1,
	keyText      = 2,
	keyBackspace = 3,
	keySpace     = 4,
	keyCapslock  = 5,
	keyEnter     = 6,
	keyClose     = 7,
	keySelector  = 8  // Language
} KeyRoles;

typedef struct
{
public:
	KeyRoles     Role;

	const RECT         GetRectValue();
	const std::wstring GetTextValue();
	const std::wstring GetPicturePath();
	void               SetRectValue(RECT);
	void               SetTextValue(const std::wstring);
	void               SetPicturePath(const std::wstring);

private:
	RECT         rect;
	std::wstring text;
	std::wstring picture;
} KeyValues;

class CKey : public CHoverBitmapWindow, public KeyValues
{
public:
	CKey();

	bool Init(HINSTANCE, HWND, KeyValues);

	static void SetEditFocus(HWND);
	const bool  IsAppending();

protected:
	static LPVOID CALLBACK OnPaint(HWND OwnerWnd, HDC hDC, PAINTSTRUCT pStruct);
	static LPVOID CALLBACK OnPress(HWND hWnd, UINT mButton, WPARAM sState, int X, int Y);	

public:
	static HWND Target;
};

class CKeyboard : public CPanel
{
public:
	CKeyboard();
	~CKeyboard();

public:
	CKey       *keys;

	void        Assign(HINSTANCE, HWND, DWORD dwExStyle = 0);
	void        CreateKeys();
	UINT        GetKeys();
	void        DeleteKeys();
	bool        IsMongolian();
	void        SetLanguage(DWORD);
	void        SetTarget(HWND);
	HWND        GetTarget();

	void        Reset();
	void        Enable(KeyRoles);
	void        Disable(KeyRoles);

	static LPVOID CALLBACK OnEditPaint(HWND OwnerWnd, HDC hDC, PAINTSTRUCT pStruct);

protected:
	const KeyValues        GetDefaultValues(UINT);
	const std::wstring     KeyLanguage();
	static LPVOID CALLBACK OnCreate(HWND hWnd, LPARAM lParam);
	static LPVOID CALLBACK OnClick(HWND hWnd, UINT mButton, WPARAM sState, int X, int Y);
	static LPVOID CALLBACK OnChangeLanguage(HWND hWnd, UINT uLanguageId);

private:
	DWORD dwLang;
};

extern CKeyboard Keyboard;