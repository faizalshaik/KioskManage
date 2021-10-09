/**
* Thread.cpp
*
* @program		Global-Pay
* @author		АНКУ ХХК
* @copyright	Copyright (c) 2017 он, АНКУ ХХК.
* @version		0.1.0.0
* @date			2017.09.19
*/

#include "thread.h"

using namespace globalpay;

CThread::CThread()
{
	Handle   = NULL;
	ID       = 0;
	UserData = NULL;
	ExitCode = 0;
	onWork   = NULL;

	Detach();
}

CThread::~CThread()
{
	if (IsWorking())
		Stop(true);
}

DWORD CThread::Start(LPVOID arg)
{
	ExitCode = (DWORD)-1;
	UserData = arg;
	Handle   = ::CreateThread(NULL, 0, ThreadFunc, this, 0, &ID);

	return ::GetLastError();
}

DWORD CThread::Stop(bool bForceKill)
{
	if (ID)
	{
		GetExitCode();

		if (ExitCode == STILL_ACTIVE && bForceKill)
			::TerminateThread(Handle, DWORD(-1));

		Handle = NULL;
		ID     = 0;
	}
	
	return ExitCode;
}

void CThread::Attach(LPTHREAD_START_ROUTINE lpThreadFunc)
{
	ThreadFunc = lpThreadFunc;
}

void CThread::Detach(void)
{
	Attach(CThread::EntryPoint);
}

DWORD CThread::EntryPoint(LPVOID pArg)
{
	CThread *Owner = reinterpret_cast<CThread*>(pArg);

	Owner->TConstructor();
	if (Owner->onWork != NULL) // Check if worker function is set
		(*Owner->onWork)(Owner->GetData());
	Owner->TDestructor();

	return STILL_ACTIVE;
}

void CThread::Set(void(*WorkerFunc)(LPVOID))
{
	SetWorker(WorkerFunc);
}

void CThread::SetWorker(void(*WorkerFunc)(LPVOID))
{
	onWork = WorkerFunc;
}

HANDLE CThread::GetHandle()
{
	return Handle;
}

DWORD CThread::GetID()
{
	return ID;
}

LPVOID CThread::GetData()
{
	return UserData;
}

DWORD CThread::GetExitCode()
{
	if (IsWorking())
		::GetExitCodeThread(Handle, (LPDWORD)&ExitCode);
	return ExitCode;
}

BOOL CThread::IsWorking()
{
	return GetHandle() != NULL;
}

void CThread::TConstructor()
{
}

void CThread::TDestructor()
{
}