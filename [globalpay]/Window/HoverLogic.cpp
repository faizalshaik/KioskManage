/**
* HoverLogic.cpp
*
* @program		Global-Pay
* @author		АНКУ ХХК
* @copyright	Copyright (c) 2017 он, АНКУ ХХК.
* @version		0.1.0.0
* @date			2017.09.19
*/

#include "hoverlogic.h"

using namespace globalpay;

CHoverLogic::CHoverLogic(void)
{
	hsCurrent      = _hsNormal;
	m_bLButtonDown = false;
	m_bFixable     = false;
	m_bFixed       = false;
	onHoverClick   = NULL;

	SetFrame();
}

CHoverLogic::~CHoverLogic(void)
{
}

void CHoverLogic::OnLogicMouseMove(HWND hWnd)
{
	if (hsCurrent != _hsOver)
	{
		hsCurrent = _hsOver;
		InvalidateWnd(hWnd);
		UpdateWnd(hWnd);
	}
}

void CHoverLogic::OnLogicMouseLeave(HWND hWnd)
{
	hsCurrent = _hsNormal;
	InvalidateWnd(hWnd);
	UpdateWnd(hWnd);
}

void CHoverLogic::OnLogicMouseDown(HWND hWnd)
{
	if (!m_bLButtonDown)
	{
		m_bLButtonDown = true;
		hsCurrent = _hsDown;
		InvalidateWnd(hWnd);
		UpdateWnd(hWnd);
	}
}

void CHoverLogic::OnLogicMouseUp(HWND hWnd, UINT mButton, BOOL bIsMouseOver)
{
	if (m_bLButtonDown)
	{
		m_bLButtonDown = false;
		
		if (bIsMouseOver)
		{
			hsCurrent = _hsOver;
			if (onHoverClick != NULL)
				(*onHoverClick)(hWnd);
		}
		else
			hsCurrent = _hsNormal;
			
		if (m_bFixable)
			m_bFixed = !m_bFixed;

		InvalidateWnd(hWnd);
		UpdateWnd(hWnd);
	}
}

void CHoverLogic::OnLogicMouseDblClick(HWND hWnd, UINT mButton, BOOL bIsMouseOver)
{
	if (bIsMouseOver)
	{
		hsCurrent = _hsOver;
		if (onHoverClick != NULL)
			(*onHoverClick)(hWnd);
	}
	else
		hsCurrent = _hsNormal;

	if (m_bFixable)
		m_bFixed = !m_bFixed;

	InvalidateWnd(hWnd);
	UpdateWnd(hWnd);
}

void CHoverLogic::SetFramePos(int nFrame, int oFrame, int dFrame)
{
	FramePos[_hsNormal] = nFrame;
	FramePos[_hsOver] = oFrame;
	FramePos[_hsDown] = dFrame;
}

void CHoverLogic::SetFrame(int ntFrame)
{
	switch (ntFrame)
	{
	case TYPE1FRAME: SetFramePos(_hsNormal, _hsNormal, _hsNormal); break;
	case TYPE2FRAME: SetFramePos(_hsNormal, _hsOver, _hsNormal); break;
	case TYPE3FRAME: SetFramePos(_hsNormal, _hsOver, _hsDown); break;
	}
}

void CHoverLogic::SetHoverClick(void(*OnClickEvent)(HWND OwnerWnd))
{
	onHoverClick = OnClickEvent;
}

void CHoverLogic::MakeFixable(bool isFixable)
{
	m_bFixable = isFixable;
}

bool CHoverLogic::IsFixed()
{
	return m_bFixable && m_bFixed;
}