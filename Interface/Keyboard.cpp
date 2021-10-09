#include "../clientwnd.h"

#include "keyboard.h"

HWND CKey::Target = NULL;
CKeyboard Keyboard;

const RECT KeyValues::GetRectValue()
{
	return rect; 
}

const std::wstring KeyValues::GetTextValue()
{
	return text;
}

const std::wstring KeyValues::GetPicturePath()
{
	return picture;
}

void KeyValues::SetRectValue(RECT rcNew)
{
	rect = rcNew;
}

void KeyValues::SetTextValue(const std::wstring szText)
{
	text = szText;
}

void KeyValues::SetPicturePath(const std::wstring szText)
{
	picture = szText;
}

CKey::CKey()
	: CHoverBitmapWindow()
{
	SetClass(KIOSKMAN_KEY);

	onPaint     = OnPaint;
	onMouseDown = OnPress;
}

CKeyboard::CKeyboard()
	: CPanel()
{
	SetClass(KIOSKMAN_KEYBOARD);

	keys             = NULL;
	onCreate         = OnCreate;
	onMouseDown      = OnClick;
	onChangeLanguage = OnChangeLanguage;
}

CKeyboard::~CKeyboard()
{
	DeleteKeys();
}

LPVOID CKeyboard::OnCreate(HWND hWnd, LPARAM lParam)
{
	CKeyboard *kb = reinterpret_cast<CKeyboard*>(GetUserDataLong(hWnd));
	if (kb)
		kb->CreateKeys();
	return 0L;
}

bool CKey::Init(HINSTANCE hInst, HWND hParent, KeyValues v)
{
	if (v.GetTextValue() == TEXT_EMPTY)
		return false;

	if (!Create(v.GetTextValue().c_str(), WS_CHILD, v.GetRectValue(), hParent, NULL, hInst, this))
		return false;

	AssignBitmap(Bmp::LoadGDI(v.GetPicturePath().c_str()));

	Role = v.Role;
	
	ShowNormal();

	return true;
}

void CKeyboard::DeleteKeys()
{
	SafeDelete(keys);
}

void CKeyboard::Assign(HINSTANCE hInst, HWND hParent, DWORD dwExStyle)
{	
	POINT p;
	p.x = 0;
	p.y = cfg.Client().bottom - coord::height(Client.footer.GetRect());
	::ClientToScreen(Client, &p);


	RECT kb = cfg.Primary();
	kb.top = p.y - HEIGHT_KEYBOARD;
	kb.bottom = p.y;

	if (CPanel::InitializeEx(dwExStyle, hInst, hParent, kb, TEXT_EMPTY, WS_POPUP))
	{
		CKey temp;
		if (!temp.Register(CS_DBLCLKS | CS_PARENTDC | CS_SAVEBITS, hInst, NULL, ::LoadCursor(NULL, IDC_HAND)))
			return;

		SetBkBrush(BRUSH_KEYBOARD);
		SetLanguage(cfg.Language());
		CreateKeys();
	}
}

void CKeyboard::SetTarget(HWND hWnd)
{
	if (!::IsWindow(hWnd))
	{
		return;
	}

	if (GetTarget() != hWnd)
	{
		if (::IsWindow(GetTarget()))
		{
			::RedrawWindow(GetTarget(), NULL, NULL, RDW_INVALIDATE);
		}

		CKey::Target = hWnd;

		::SetFocus(GetTarget());
		::RedrawWindow(GetTarget(), NULL, NULL, RDW_INVALIDATE);
	}	
}

HWND CKeyboard::GetTarget()
{
	return CKey::Target;
}

void CKeyboard::SetLanguage(DWORD dwNewValue)
{
	dwLang = dwNewValue;
}

void CKeyboard::CreateKeys()
{
	DeleteKeys();

	int nCount;
	if (dwLang == LANG_MONGOLIAN)
		nCount = MONGOLIAN_KEYS;
	else
		nCount = ENGLISH_KEYS;
	
	keys = new CKey[nCount];
	int ignoring = 0;
	for (int i = 0; i < MONGOLIAN_KEYS; i++)
	{
		if (!keys[i - ignoring].Init(GetInstance(), GetHandle(), GetDefaultValues(i)))
		{
			ignoring++;
		}
	}

	keys[0].SetBkBrush(BRUSH_KEY);
}

UINT CKeyboard::GetKeys()
{
	return (dwLang == LANG_MONGOLIAN) ? MONGOLIAN_KEYS : ENGLISH_KEYS;
}

const KeyValues CKeyboard::GetDefaultValues(UINT index)
{
	int     row = (int) ::floor(index / 11);
	int  column = index - row * 11;
	float width = 1;
	float start = 0;

	KeyValues key;

	key.Role = keyText;
	
	if (index < 10)
	{
		key.Role = keyNumber;
		index++;
		key.SetTextValue(std::to_wstring((index < 10) ? index : 0));
	}
	else
	{
		switch (index)
		{
		case 10:
		{
			key.Role = keyBackspace;
			width    = 2;
			key.SetTextValue(IsMongolian() ? TEXT("Арилгах") : TEXT("Backspace"));
		} break;
		case 11:
		{
			key.SetTextValue(IsMongolian() ? TEXT("Ф") : TEXT("Q"));
		} break;
		case 12:
		{
			key.SetTextValue(IsMongolian() ? TEXT("Ц") : TEXT("W"));
		} break;
		case 13:
		{
			key.SetTextValue(IsMongolian() ? TEXT("У") : TEXT("E"));
		} break;
		case 14:
		{
			key.SetTextValue(IsMongolian() ? TEXT("Ж") : TEXT("R"));
		} break;
		case 15:
		{
			key.SetTextValue(IsMongolian() ? TEXT("Э") : TEXT("T"));
		} break;
		case 16:
		{
			key.SetTextValue(IsMongolian() ? TEXT("Н") : TEXT("Y"));
		} break;
		case 17:
		{
			key.SetTextValue(IsMongolian() ? TEXT("Г") : TEXT("U"));
		} break;
		case 18:
		{
			key.SetTextValue(IsMongolian() ? TEXT("Ш") : TEXT("I"));
		} break;
		case 19:
		{
			key.SetTextValue(IsMongolian() ? TEXT("Ү") : TEXT("O"));
		} break;
		case 20:
		{
			key.SetTextValue(IsMongolian() ? TEXT("З") : TEXT("P"));
		} break;
		case 21:
		{
			key.SetTextValue(IsMongolian() ? TEXT("К") : TEXT(""));
		} break;
		case 22:
		{
			key.SetTextValue(IsMongolian() ? TEXT("Й") : TEXT("A"));
		} break;
		case 23:
		{
			key.SetTextValue(IsMongolian() ? TEXT("Ы") : TEXT("S"));
		} break;
		case 24:
		{
			key.SetTextValue(IsMongolian() ? TEXT("Б") : TEXT("D"));
		} break;
		case 25:
		{
			key.SetTextValue(IsMongolian() ? TEXT("Ө") : TEXT("F"));
		} break;
		case 26:
		{
			key.SetTextValue(IsMongolian() ? TEXT("А") : TEXT("G"));
		} break;
		case 27:
		{
			key.SetTextValue(IsMongolian() ? TEXT("Х") : TEXT("H"));
		} break;
		case 28:
		{
			key.SetTextValue(IsMongolian() ? TEXT("Р") : TEXT("J"));
		} break;
		case 29:
		{
			key.SetTextValue(IsMongolian() ? TEXT("О") : TEXT("K"));
		} break;
		case 30:
		{
			key.SetTextValue(IsMongolian() ? TEXT("Л") : TEXT("L"));
		} break;
		case 31:
		{
			key.SetTextValue(IsMongolian() ? TEXT("Д") : TEXT(""));
		} break;
		case 32:
		{
			key.SetTextValue(IsMongolian() ? TEXT("П") : TEXT(""));
		} break;
		case 33:
		{
			key.SetTextValue(IsMongolian() ? TEXT("Я") : TEXT("Z"));
		} break;
		case 34:
		{
			key.SetTextValue(IsMongolian() ? TEXT("Ч") : TEXT("X"));
		} break;
		case 35:
		{
			key.SetTextValue(IsMongolian() ? TEXT("Ё") : TEXT("C"));
		} break;
		case 36:
		{
			key.SetTextValue(IsMongolian() ? TEXT("С") : TEXT("V"));
		} break;
		case 37:
		{
			key.SetTextValue(IsMongolian() ? TEXT("М") : TEXT("B"));
		} break;
		case 38:
		{
			key.SetTextValue(IsMongolian() ? TEXT("И") : TEXT("N"));
		} break;
		case 39:
		{
			key.SetTextValue(IsMongolian() ? TEXT("Т") : TEXT("M"));
		} break;
		case 40:
		{
			key.SetTextValue(IsMongolian() ? TEXT("Ь") : TEXT(""));
		} break;
		case 41:
		{
			key.SetTextValue(IsMongolian() ? TEXT("В") : TEXT(""));
		} break;
		case 42:
		{
			key.SetTextValue(IsMongolian() ? TEXT("Ю") : TEXT(""));
		} break;
		case 43:
		{
			key.SetTextValue(IsMongolian() ? TEXT("Е") : TEXT(""));
		} break;
		case 44:
		{
			key.SetTextValue(IsMongolian() ? TEXT("Щ") : TEXT(""));
		} break;
		case 45:
		{
			key.SetTextValue(IsMongolian() ? TEXT("Ъ") : TEXT(""));
		} break;
		case 46:
		{
			key.SetTextValue(IsMongolian() ? TEXT(".") : TEXT("."));
		} break;
		case 47:
		{
			key.SetTextValue(IsMongolian() ? TEXT(",") : TEXT(","));
		} break;
		case 48:
		{
			key.SetTextValue(IsMongolian() ? TEXT("-") : TEXT("-"));
		} break;
		case 49:
		{
			key.Role = keySpace;
			key.SetTextValue(IsMongolian() ? TEXT("Зай авах") : TEXT("Space"));
			width    = (float)(IsMongolian() ? 5 : 3);
		} break;
		case 50:
		{
			key.Role = keySelector;
			key.SetPicturePath(KeyLanguage());
			key.SetTextValue(TEXT("Language"));
			start    = (float)(IsMongolian() ? 4 : 2);
		} break;
		}
	}
	if (!IsMongolian())
	{
		switch (row)
		{
		case 1: start = 0.5; break;
		case 2: start = 1; break;
		case 3: start = 2; break;
		}
	}

	int rwidth  = (row == 0) ? 86 : 94;
	int rheight = 50;
	int between = 3; 

	RECT rect   = coord::to_rect(
		(int) (10 + column * (rwidth + between) + start * (rwidth + between)), 
		10 + row * (rheight + between),
		(int) (rwidth * width + 3 * (width - 1)),
		rheight);

	RECT kbRect = GetClientRect();
	if (rect.right > (kbRect.right - 10))
		rect.right = kbRect.right - 10;
	key.SetRectValue(rect);

	return key;
}

bool CKeyboard::IsMongolian()
{
	return (dwLang == LANG_MONGOLIAN);
}

void CKeyboard::Reset()
{
	for (UINT i = 0; i < GetKeys(); i++)
	{
		if (!::IsWindowEnabled(keys[i]))
		{
			keys[i].Enable();
		}
	}
}

LPVOID CKey::OnPaint(HWND OwnerWnd, HDC hDC, PAINTSTRUCT pStruct)
{	
	CKey *key = reinterpret_cast<CKey*>(GetUserDataLong(OwnerWnd));
	if (key)
	{
		if (key->GetBitmap())
		{
			key->PaintBitmap(hDC);
		}
		else
		{
			HPEN   hPenOld   = static_cast<HPEN> (::SelectObject(hDC, ::GetStockObject(DC_PEN)));
			HBRUSH hBrushOld = static_cast<HBRUSH> (::SelectObject(hDC, ::GetStockObject(NULL_BRUSH)));
			
			RECT rcWindow = key->GetClientRect();
			RECT rcClient = rcWindow;
			
			if (!::IsWindowEnabled(*key))
			{
				::FillRect(hDC, &rcWindow, BRUSH_DISABLED_KEY);
			}

			::SetDCPenColor(hDC, RGB(85, 85, 85));
			::Rectangle(hDC, rcClient.left, rcClient.top, rcClient.right, rcClient.bottom);
			::InflateRect(&rcClient, -2, -2);
			::SetDCPenColor(hDC, RGB(180, 180, 180));
			::Rectangle(hDC, rcClient.left, rcClient.top, rcClient.right, rcClient.bottom);

			std::wstring szText = key->AsString();
			if (szText.length() > 5)
			{
				::SelectObject(hDC, cfg.Font(24));
			}
			else
			{
				::SelectObject(hDC, cfg.Font(28));
			}

			::SetBkMode(hDC, TRANSPARENT);
			::SetTextColor(hDC, RGB_WHITE);
			::DrawText(hDC, szText.c_str(), -1, &rcWindow, DT_CENTER | DT_SINGLELINE | DT_VCENTER);

			::SelectObject(hDC, hPenOld);
			::SelectObject(hDC, hBrushOld);
		}
	}
	return 0L;
}

LPVOID CKey::OnPress(HWND hWnd, UINT mButton, WPARAM sState, int X, int Y)
{
	if (!::IsWindow(CKey::Target))
		return 0L;

	CKey *key = reinterpret_cast<CKey*>(GetUserDataLong(hWnd));
	if (key)
	{
		if (key->Role == keyClose || key->Role == keySelector)
		{
			CKeyboard *kb = reinterpret_cast<CKeyboard*>(GetUserDataLong(key->GetParent()));
			if (kb)
			{
				kb->Hide();
				if (key->Role == keySelector)
				{
					kb->SetLanguage(kb->IsMongolian() ? LANG_ENGLISH : LANG_MONGOLIAN);
					kb->CreateKeys();
					kb->Show();
				}
			}
			return 0L;
		}

		UINT nLimitChars = (UINT)::SendMessage(CKey::Target, EM_GETLIMITTEXT, 0, 0);
		UINT nCurrentChars = ::GetWindowTextLength(CKey::Target);
		std::wstring szValue;
		if (nCurrentChars > 0)
		{
			UINT nMaxCount = nCurrentChars + 1;
			WCHAR *szBuffer = static_cast<WCHAR*>(::malloc(nMaxCount * sizeof(WCHAR*)));
			::GetWindowText(CKey::Target, szBuffer, nMaxCount);
			szValue = szBuffer;
			::free(szBuffer);
		}

		switch (key->Role)
		{
		case keyNumber:
		case keyText:
		{
			if (nCurrentChars < nLimitChars)
				szValue += key->AsString();
		} break;
		case keyBackspace:
		{
			if (nCurrentChars > 0)
				szValue = szValue.substr(0, szValue.length() - 1);
		} break;
		case keySpace:
		{
			if (nCurrentChars < nLimitChars && nCurrentChars > 0)
				szValue += TEXT(" ");
		} break;
		}
		::SetWindowText(CKey::Target, szValue.c_str());
		CKey::SetEditFocus(CKey::Target);
	}
	return 0L;
}

LPVOID CKeyboard::OnClick(HWND hWnd, UINT mButton, WPARAM sState, int X, int Y)
{
	if (CKey::Target)
		CKey::SetEditFocus(CKey::Target);

	return 0L;
}

LPVOID CKeyboard::OnChangeLanguage(HWND hWnd, UINT uLanguageId)
{
	return 0L;
}

void CKey::SetEditFocus(HWND hEditWnd)
{
	::SetFocus(hEditWnd);
	::SendMessage(hEditWnd, EM_SETSEL, (WPARAM)0, (LPARAM)-1);
	::SendMessage(hEditWnd, EM_SETSEL, (WPARAM)-1, (LPARAM)-1);
}

const bool CKey::IsAppending()
{
	return Role == keyNumber || Role == keyText || Role == keySpace;
}

const std::wstring CKeyboard::KeyLanguage()
{
	return cfg.DataPath(TEXT("keyboard\\")) + TEXT("lang-") + (IsMongolian() ? TEXT("en") : TEXT("mn")) + TEXT(".png");
}

LPVOID CKeyboard::OnEditPaint(HWND OwnerWnd, HDC hDC, PAINTSTRUCT pStruct)
{
	CEdit *edit = reinterpret_cast<CEdit*>(GetUserDataLong(OwnerWnd));
	if (edit)
	{
		RECT rect = edit->GetClientRect();
		edit->SetBkBrush(BRUSH_WHITE);
		
		if (*edit == ::GetFocus())
			::SelectObject(hDC, ::CreatePen(PS_SOLID, 1, RGB_RED));
		else
			::SelectObject(hDC, ::CreatePen(PS_SOLID, 1, RGB_BLACK));
		
		::Rectangle(hDC, rect.left, rect.top, rect.right, rect.bottom);

		rect.left += 5;
		::SetBkMode(hDC, TRANSPARENT);
		::SetTextColor(hDC, RGB_BLACK);
		::SelectObject(hDC, cfg.Font(24));
		
		std::wstring szText = edit->AsString();
		::DrawText(hDC, szText.c_str(), szText.length(), &rect, DT_LEFT | DT_SINGLELINE | DT_VCENTER);
	}
	return 0L;
}