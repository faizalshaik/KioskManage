#include "../clientwnd.h"

#include "popup.h"

PopupBackground::PopupBackground()
	: CWnd()
{
	SetClass(TEXT("KioskMan::PopupBackground"));
}

Popup::Popup()
	: CPanel()
{
	SetClass(TEXT("KioskMan::Popup"));
}

Menu1::Menu1()
	: CPanel()
{
	SetClass(TEXT("KioskMan::Popup::Menu"));

	onShow = OnPopupShow;
}

LPVOID Menu1::OnPopupShow(HWND hParent, UINT nShow)
{
	Menu1 *me = reinterpret_cast<Menu1*>(GetUserDataLong(hParent));

	if (me)
		::MessageBox(*me, TEXT("Menu1"), TEXT("ALERT"), MB_OK);

	return 0L;
}

bool Popup::Init(HINSTANCE hInst, HWND hParent, RECT rcParent)
{
	RECT myRect = GetClientRect();
	myRect.bottom = coord::height(myRect);
	myRect.top = 0;
	myRect.left = myRect.right - 400;

	menu1.Register(CS_VREDRAW | CS_HREDRAW, GetInstance(), NULL, NULL, NULL, NULL);
	menu1.Create(TEXT(""), WS_CHILD, myRect, *this, NULL, GetInstance());
	menu1.Initialize(GetInstance(), hParent, myRect, TEXT("Popup::Menu"));
	menu1.SetBkBrush(BRUSH_WHITE);
	menu1.Show();

	return true;
}