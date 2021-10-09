/*
--------------------------------------------------------------------------------------------------
									INFORMATION OF SOURCE FILE
--------------------------------------------------------------------------------------------------

	Module Name:
		ICT3K5_6240DLL.h

	Abstract:
		Master include file for applications that call functions provided
		by ICT3K5_6240DLL.dll.

	Author:

--------------------------------------------------------------------------------------------------
*/

#ifndef	_ICT3K5_6240DLL_H_
#define	_ICT3K5_6240DLL_H_


#include <windows.h>


#define	MAX_DATA_ARRAY_SIZE		(1024)


#pragma pack(8)


typedef	struct
{
	struct
	{
		CHAR	szFilename[ _MAX_FNAME];
		CHAR	szRevision[ 32];
	}
	upperDll;

	struct
	{
		CHAR	szFilename[ _MAX_FNAME];
		CHAR	szRevision[ 32];
	}
	lowerDll;
}
DLL_INFORMATION, *LPDLL_INFORMATION;



/* Data structure for command message	*/
typedef	struct
{
	BYTE	bCommandCode;
	BYTE	bParameterCode;

	struct
	{
		DWORD	dwSize;
		LPBYTE	lpbBody;
	}
	Data;
}
COMMAND, *LPCOMMAND;



/* Received message type	*/
typedef	enum
{
	PositiveReply,
	NegativeReply,
	ReplyReceivingFailure,
	CommandCancellation,
	ReplyTimeout,
}
REPLY_TYPE, *LPREPLY_TYPE;



/* Data structure for positive reply message	*/
typedef	struct
{
	BYTE	bCommandCode;
	BYTE	bParameterCode;

	struct
	{
		BYTE	bSt1;
		BYTE	bSt0;
	}
	StatusCode;

	struct
	{
		DWORD	dwSize;
		BYTE	bBody[ MAX_DATA_ARRAY_SIZE];
	}
	Data;
}
POSITIVE_REPLY, *LPPOSITIVE_REPLY;



/* Data structure for negative reply message	*/
typedef	struct
{
	BYTE	bCommandCode;
	BYTE	bParameterCode;

	struct
	{
		BYTE	bE1;
		BYTE	bE0;
	}
	ErrorCode;

	struct
	{
		DWORD	dwSize;
		BYTE	bBody[ MAX_DATA_ARRAY_SIZE];
	}
	Data;
}
NEGATIVE_REPLY, *LPNEGATIVE_REPLY;



/* Data structure for reply message	*/
typedef struct
{
	REPLY_TYPE		replyType;

	union
	{
		POSITIVE_REPLY	positiveReply;
		NEGATIVE_REPLY	negativeReply;
	}
	message;

}
REPLY, *LPREPLY;

/*
--------------------------------------------------------------------------------------------------
	Return codes of the APIs
*/

#define	_ERROR_CODE_ORIGIN		(0x0000)


/*
	Return codes of the following API(s):
	- GetDllInformation
	- ConnectDevice
	- DisconnectDevice
	- ExecuteCommand
	- ExecuteCommand2
	- CancelCommand
*/
#define	_NO_ERROR							( _ERROR_CODE_ORIGIN + 0x0 )
#define	_DEVICE_NOT_CONNECTED_ERROR			( _ERROR_CODE_ORIGIN + 0x1 )
#define	_CANCEL_COMMAND_SESSION_ERROR		( _ERROR_CODE_ORIGIN + 0x2 )
#define	_FAILED_TO_SEND_COMMAND_ERROR		( _ERROR_CODE_ORIGIN + 0x3 )
#define _FAILED_TO_RECEIVE_REPLY_ERROR		( _ERROR_CODE_ORIGIN + 0x4 )
#define _COMMAND_CANCELED					( _ERROR_CODE_ORIGIN + 0x5 )
#define _REPLY_TIMEOUT						( _ERROR_CODE_ORIGIN + 0x6 )



/*
	Return codes of the following API(s):
 	- GetDllInformation

	nothing unique ErrorCode
*/

/*
	Return codes of the following API(s):
	- ConnectDevice
*/
#define	_CANNOT_CREATE_OBJECT_ERROR			( _ERROR_CODE_ORIGIN + 0x0101 )
#define	_DEVICE_NOT_READY_ERROR				( _ERROR_CODE_ORIGIN + 0x0102 )
#define	_CANNOT_OPEN_PORT_ERROR				( _ERROR_CODE_ORIGIN + 0x0103 )
#define	_FAILED_TO_BEGIN_THREAD_ERROR		( _ERROR_CODE_ORIGIN + 0x0104 )
#define	_DEVICE_ALREADY_CONNECTED_ERROR		( _ERROR_CODE_ORIGIN + 0x0105 )


/*
	Return codes of the following API(s):
	- DisconnectDevice

	nothing unique ErrorCode
*/

/*
	Return codes of the following API(s):
	- CancelCommand

	nothing unique ErrorCode
*/

/*
	Return codes of the following API(s):
	- UpdateFirmware
*/
#define	_UPDATE_FIRMWARE_UNEXPECTED_ERROR						( _ERROR_CODE_ORIGIN + 0x1001 )
#define	_UPDATE_FIRMWARE_UNKNOWN_FILE_TYPE_ERROR				( _ERROR_CODE_ORIGIN + 0x1002 )
#define	_UPDATE_FIRMWARE_CANNOT_OPEN_FILE_ERROR					( _ERROR_CODE_ORIGIN + 0x1003 )
#define	_UPDATE_FIRMWARE_FAILED_TO_ALLOCATE_MEMORY_REGION_ERROR	( _ERROR_CODE_ORIGIN + 0x1004 )
#define	_UPDATE_FIRMWARE_CANNOT_READ_FILE_ERROR					( _ERROR_CODE_ORIGIN + 0x1005 )
#define	_UPDATE_FIRMWARE_CONNECT_DEVICE_FAILED_ERROR			( _ERROR_CODE_ORIGIN + 0x1006 )
#define	_UPDATE_FIRMWARE_DEVICE_ALREADY_CONNECTED_ERROR			( _ERROR_CODE_ORIGIN + 0x1007 )
#define	_UPDATE_FIRMWARE_COMMAND_EXECUTION_FAILED_ERROR			( _ERROR_CODE_ORIGIN + 0x1008 )
#define	_UPDATE_FIRMWARE_NEGATIVE_REPLY_RECEIVED_ERROR			( _ERROR_CODE_ORIGIN + 0x1009 )
#define	_UPDATE_FIRMWARE_DISCONNECT_DEVICE_FAILED_ERROR			( _ERROR_CODE_ORIGIN + 0x100A )
#define	_UPDATE_FIRMWARE_UNEXPECTED_FILE_CONTENTS_ERROR			( _ERROR_CODE_ORIGIN + 0x100B )
#define	_UPDATE_FIRMWARE_IDENTICAL_REVISION_ERROR				( _ERROR_CODE_ORIGIN + 0x100C )


/*
	Return codes of the following API(s):
	- ICCardTransmit
*/
#define	_ICC_TRANSMIT_UNEXPECTED_ERROR							( _ERROR_CODE_ORIGIN + 0x2001 )
#define	_ICC_TRANSMIT_FAILED_TO_ALLOCATE_MEMORY_REGION_ERROR	( _ERROR_CODE_ORIGIN + 0x2002 )
#define	_ICC_TRANSMIT_ABORT_REQUEST_RECEIVED_ERROR				( _ERROR_CODE_ORIGIN + 0x2003 )
#define	_ICC_TRANSMIT_COMMAND_EXECUTION_FAILED_ERROR			( _ERROR_CODE_ORIGIN + 0x2004 )
#define	_ICC_TRANSMIT_NEGATIVE_REPLY_RECEIVED_ERROR				( _ERROR_CODE_ORIGIN + 0x2005 )


/*
	Return codes of the following API(s):
	- SAMTransmit
*/
#define	_SAM_TRANSMIT_UNEXPECTED_ERROR							( _ERROR_CODE_ORIGIN + 0x3001 )
#define	_SAM_TRANSMIT_FAILED_TO_ALLOCATE_MEMORY_REGION_ERROR	( _ERROR_CODE_ORIGIN + 0x3002 )
#define	_SAM_TRANSMIT_ABORT_REQUEST_RECEIVED_ERROR				( _ERROR_CODE_ORIGIN + 0x3003 )
#define	_SAM_TRANSMIT_COMMAND_EXECUTION_FAILED_ERROR			( _ERROR_CODE_ORIGIN + 0x3004 )
#define	_SAM_TRANSMIT_NEGATIVE_REPLY_RECEIVED_ERROR				( _ERROR_CODE_ORIGIN + 0x3005 )



/*
	The end of the declaration of return codes for the APIs
--------------------------------------------------------------------------------------------------
*/

typedef VOID ( WINAPI *CALL_BACK_FUNCTION) ( WPARAM, LPARAM);


#ifndef	_DLL_DEVELOPMENT_
#define	DLL_API	__declspec( dllimport)
#else
#define	DLL_API	__declspec( dllexport)
#endif

DLL_API	DWORD WINAPI	GetDllInformation( LPDLL_INFORMATION);
DLL_API	DWORD WINAPI	ConnectDevice( LPCSTR, CONST DWORD);
DLL_API	DWORD WINAPI	DisconnectDevice( LPCSTR);
DLL_API	DWORD WINAPI	CancelCommand( LPCSTR);
DLL_API	DWORD WINAPI	ExecuteCommand( LPCSTR, CONST COMMAND, CONST DWORD, LPREPLY);
DLL_API	DWORD WINAPI	ExecuteCommand2( LPCSTR, LPCOMMAND, CONST DWORD, LPREPLY);
DLL_API	DWORD WINAPI	UpdateFirmware( LPCSTR, CONST DWORD, LPCSTR, CONST BOOL, CALL_BACK_FUNCTION, LPDWORD);
DLL_API	DWORD WINAPI	ICCardTransmit( LPCSTR, CONST DWORD, LPBYTE, CONST DWORD, LPDWORD, LPBYTE, LPDWORD, LPREPLY);
DLL_API	DWORD WINAPI	SAMTransmit( LPCSTR, CONST DWORD, LPBYTE, CONST DWORD, LPDWORD, LPBYTE, LPDWORD, LPREPLY);


#endif	/* _ICT3K5_6240DLL_H_	*/
