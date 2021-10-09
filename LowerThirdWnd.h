/**
* TicketMachine -> Lower third window - Initial version
*
* @program		Global-Pay
* @author		АНКУ ХХК
* @copyright	Copyright (c) 2017 он, АНКУ ХХК.
* @version		0.1.0.0
* @date			2017.09.19
*/

#pragma once

#include "control/shockwaveflash.h"

using namespace globalpay;

class CLowerThirdWnd : public CFlashControl
{
public:
	void MarqueeSetText(std::wstring);
};

extern CLowerThirdWnd LowerThird;