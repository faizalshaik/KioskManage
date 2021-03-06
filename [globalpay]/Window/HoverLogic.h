/**
* HoverLogic Object - HoverLogic.h
*
* @program		Global-Pay
* @author		АНКУ ХХК
* @copyright	Copyright (c) 2017 он, АНКУ ХХК.
* @version		0.1.0.0
* @date			2017.09.19
*/

#pragma once

#include "wndfunc.h"

namespace globalpay {
#define TYPE1FRAME 1
#define TYPE2FRAME 2
#define TYPE3FRAME 3

	typedef	enum
	{
		_hsNormal = 0,
		_hsOver   = 1,
		_hsDown   = 2
	} HoverState;

	class CHoverLogic
	{
	public:
		CHoverLogic(void);
		~CHoverLogic(void);

		UINT       FramePos[3];
		HoverState hsCurrent;

	protected:
		bool m_bLButtonDown;
		bool m_bFixable;
		bool m_bFixed;

		void SetFramePos(int nFrame, int oFrame, int dFrame);

	public:
		void SetFrame(int ntFrame = TYPE1FRAME);
		void SetHoverClick(void(*)(HWND) = NULL);
		void MakeFixable(bool isFixable = true);
		bool IsFixed();

		void(*onHoverClick)(HWND hWnd);

		void OnLogicMouseMove(HWND hWnd);
		void OnLogicMouseLeave(HWND hWnd);
		void OnLogicMouseDown(HWND hWnd);
		void OnLogicMouseUp(HWND hWnd, UINT mButton, BOOL);
		void OnLogicMouseDblClick(HWND hWnd, UINT mButton, BOOL);
	};
}