#include "../clientwnd.h"

#include "worker.h"

CWorker Worker;

CWorker::CWorker()
	: CWnd()
{
	SetClass(TEXT("KioskMan::Worker"));

	childs = NULL;
	brush = BRUSH_PRIMARY;

	Painter();
}

CWorker::~CWorker()
{
	SafeDeleteObject(brush);
	SafeDelete(childs);
}

void CWorker::Show()
{
	Client.menu.Hide();
	Client.header.Show();
	Client.footer.Show();
	CWndBasic::Show();
}

void CWorker::Hide()
{
	CWndBasic::Hide();
	Client.header.Hide();
	Client.footer.Hide();
	Client.menu.Show(Client.GetState() == ST_DISCONNECTED);
}

void CWorker::Color(HBRUSH hBrush)
{
	SafeDeleteObject(brush);
	brush = hBrush;
}

void CWorker::Icon(HBITMAP hBmp)
{
	Client.header.icon.AssignBitmap(hBmp);
}

void CWorker::Status(LPCTSTR szText)
{
	Client.header.SetStatus(szText);
}

void CWorker::Timer(UINT nInterval)
{
	Client.header.timer.Start(nInterval);
}

void CWorker::SetFooterButton(CFooterButton *btn, LPCTSTR szText, HBRUSH Color, HBITMAP Icon)
{
	btn->SetText(szText);

	if (Color)
		btn->SetColor(Color);

	if (Icon)
	{
		SafeDeleteObject(btn->Custom);
		btn->Custom = Icon;
	}		

	btn->Show();
	btn->Redraw();
}

CFooterButton* CWorker::Left(LPCTSTR szText, HBRUSH Color, HBITMAP Icon)
{
	SetFooterButton(&Client.footer.left, szText, Color, Icon);
	return &Client.footer.left;
}

CFooterButton* CWorker::Right(LPCTSTR szText, HBRUSH Color, HBITMAP Icon)
{
	SetFooterButton(&Client.footer.right, szText, Color, Icon);
	return &Client.footer.right;
}

void CWorker::Painter(LPVOID(CALLBACK *OnPaint)(HWND, HDC, PAINTSTRUCT))
{
	onPaint = (OnPaint != NULL) ? OnPaint : DefaultPainter;
}

LPVOID CWorker::DefaultPainter(HWND hWnd, HDC hDC, PAINTSTRUCT pStruct)
{
	return 0L;
}