/*
>>> Wrapper class for
	Nidec Sankyo card reader API

Founder : coden aka Narankhuu N
mailto  : codesaur@gmail.com

Copyright © 2017 Metatech LLC.
All rights reserved.

>>> 2017.03.22 - Initial version
*/

#pragma once

#include "[vendor]/sankyo/ICT3K5_6240DLL.h"

#include "types/string.h"

namespace codesaur {
	class Sankyo
	{
	public:
		Sankyo();
		~Sankyo();

		COMMAND Command;
		REPLY   Reply;

		void  Prepare(const char*, DWORD baudrate = 115200, UINT timeout = 20000);
		DWORD GetResult();
		BOOL  HasError();
		BOOL  IsPositive();

		void  SetCommand(BYTE, BYTE, BYTE DataBody[] = NULL, DWORD DataSize = 0);
		DWORD Execute();
		DWORD Execute2();
		DWORD Cancel();

		DWORD Connect();
		DWORD Disconnect();
		BOOL  IsConnected();
		DWORD Initialize(BYTE pm = 0x30);
		DWORD Status(BYTE pm = 0x30);
		DWORD Entry();
		DWORD Retrieve();
		DWORD Eject();
		DWORD Capture();

		bool        IsCardAtFront();
		std::string MagneticRead(int ISO = 1);

	private:
		std::string Port;
		DWORD       dwBaudRate;
		UINT        nTimeOut;
		DWORD       dwResult;
		BOOL        bConnected;
	};
}