// original name : Sankyo.cpp
// more info @   : Sankyo.h

#include "sankyo.h"

using namespace codesaur;

Sankyo::Sankyo()
{
	Prepare("COM1", 115200, 20000);
}

Sankyo::~Sankyo()
{
	Disconnect();
}

void Sankyo::Prepare(const char* port, DWORD baudrate, UINT timeout)
{
	Port       = port;
	dwBaudRate = baudrate;
	nTimeOut   = timeout;
	bConnected = false;
}

DWORD Sankyo::GetResult()
{
	return dwResult;
}

BOOL Sankyo::HasError()
{
	return GetResult() != _NO_ERROR;
}

BOOL Sankyo::IsPositive()
{
	return Reply.replyType == PositiveReply;
}

DWORD Sankyo::Connect()
{
	dwResult = ConnectDevice(Port.c_str(), dwBaudRate);
	bConnected = (dwResult == _NO_ERROR);
	return dwResult;
}

DWORD Sankyo::Disconnect()
{
	if (IsConnected())
	{
		dwResult = DisconnectDevice(Port.c_str());
		if (dwResult == _NO_ERROR)
		{
			bConnected = FALSE;
		}
		return dwResult;
	}
	return _NO_ERROR;
}

BOOL Sankyo::IsConnected()
{
	return bConnected;
}

void Sankyo::SetCommand(BYTE Cmd, BYTE Parameter, BYTE DataBody[], DWORD DataSize)
{
	Command.bCommandCode     = Cmd;
	Command.bParameterCode   = Parameter;
	if (DataBody != NULL)
	{
		Command.Data.lpbBody = DataBody;
		Command.Data.dwSize  = (DataSize == 0) ? sizeof(DataBody) / sizeof(BYTE) : DataSize;
	}
	else
	{
		Command.Data.dwSize  = 0;
	}
}

DWORD Sankyo::Execute()
{
	dwResult = ExecuteCommand(Port.c_str(), Command, nTimeOut, &Reply);
	return dwResult;
}

DWORD Sankyo::Execute2()
{
	dwResult = ExecuteCommand2(Port.c_str(), &Command, nTimeOut, &Reply);
	return dwResult;
}

DWORD Sankyo::Cancel()
{
	return CancelCommand(Port.c_str());
}

DWORD Sankyo::Initialize(BYTE parametr)
// pm - Parameter by default 0x30
// 0x30 => Initialize, set up and eject a card
// 0x31 => Initialize, set up and capture a card
// 0x32 => Initialize, set up and re-positioning a card
// 0x33 => Initialize, set up without card operation
// 0x34 => Initialize parameter report
{
	BYTE Data[] = { 0x33, 0x32, 0x34, 0x31, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30 };
	SetCommand(0x30, parametr, Data);
	return Execute2();
}

DWORD Sankyo::Status(BYTE pm)
// pm - Parameter by default 0x30
// 0x30 => Report presence of card and its position
// 0x31 => Report presence of sensor status in detail
{
	SetCommand(0x31, pm);
	return Execute2();
}

DWORD Sankyo::Entry()
{
	SetCommand(0x32, 0x30);
	return Execute2();
}

DWORD Sankyo::Retrieve()
{
	SetCommand(0x34, 0x30);
	return Execute2();
}

DWORD Sankyo::Eject()
{
	SetCommand(0x33, 0x30);
	return Execute2();
}

DWORD Sankyo::Capture()
{
	SetCommand(0x33, 0x31);
	return Execute2();
}

std::string Sankyo::MagneticRead(int ISO)
{
	if (ISO < 0 || ISO > 3)
	{
		ISO = 1;
	}

	switch (ISO)
	{
	case 2:  SetCommand(0x36, 0x32); break;
	case 3:  SetCommand(0x36, 0x33); break;
	default: SetCommand(0x36, 0x31); break;
	}
	
	Execute2();

	std::string data;
	if (IsPositive())
	{
		for (UINT n = 0; n < Reply.message.positiveReply.Data.dwSize; ++n)
		{
			data += Reply.message.positiveReply.Data.bBody[n];
		}
	}
	return data;
}

bool Sankyo::IsCardAtFront()
{
	if (!IsPositive())
	{
		return (Reply.message.negativeReply.bCommandCode == 50
			&& Reply.message.negativeReply.bParameterCode == 48
			&& Reply.message.negativeReply.ErrorCode.bE0 == 50
			&& Reply.message.negativeReply.ErrorCode.bE1 == 48);
	}
	return false;
}