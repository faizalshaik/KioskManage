/**
* A Generic C++ Thread Class - Thread.h
*
* @program		Global-Pay
* @author		АНКУ ХХК
* @copyright	Copyright (c) 2017 он, АНКУ ХХК.
* @version		0.1.0.0
* @date			2017.09.19
*/

#pragma once

#include <windows.h>

namespace globalpay {
	class CThread
	{
	public:
		CThread();
		~CThread();

	private:
		LPTHREAD_START_ROUTINE
			   ThreadFunc;      // Thread Function Pointer
		HANDLE Handle;	        // Thread Handle
		DWORD  ID;              // Thread ID
		LPVOID UserData;        // User data pointer
		DWORD  ExitCode;        // Exit code of the thread

	protected:
		static DWORD WINAPI EntryPoint(LPVOID);

	public:
		DWORD  Start(LPVOID arg = NULL);
		DWORD  Stop(bool bForceKill = false);
		
		void   Attach(LPTHREAD_START_ROUTINE);
		void   Detach();

		void   Set(void(*)(LPVOID) = NULL);
		void   SetWorker(void(*)(LPVOID));

		HANDLE GetHandle();
		DWORD  GetID();
		LPVOID GetData();
		DWORD  GetExitCode();
		BOOL   IsWorking();

		void       (*onWork)(LPVOID);       // The Worker Function Pointer
		virtual void TConstructor();
		virtual void TDestructor();
	};
}