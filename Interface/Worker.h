#pragma once

#include "window/wnd.h"

#include "footer.h"

#include "../config.hpp"

class CWorker : public CWnd
{
public:
	CWorker();
	~CWorker();

	HWND *childs;
	HBRUSH brush;

	void Show();
	void Hide();
	
	void Color(HBRUSH);
	void Icon(HBITMAP);
	void Status(LPCTSTR);
	void Timer(UINT);

	CFooterButton* Left(LPCTSTR, HBRUSH Color = NULL, HBITMAP Icon = NULL);
	CFooterButton* Right(LPCTSTR, HBRUSH Color = NULL, HBITMAP Icon = NULL);
	
	void Painter(LPVOID(CALLBACK *OnPaint)(HWND, HDC, PAINTSTRUCT) = NULL);

protected:
	static LPVOID CALLBACK DefaultPainter(HWND, HDC, PAINTSTRUCT);

private:
	void SetFooterButton(CFooterButton *, LPCTSTR, HBRUSH, HBITMAP);
};

extern CWorker Worker;