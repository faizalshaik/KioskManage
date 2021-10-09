#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include <psapi.h>

#include "updater.h"

CUpdater Updater;

CUpdater::CUpdater()
	: CBitmapWindow()
{
	SetClass(UPDATER_CLASS);

	onCreate = OnCreate;
	onClose = OnClose;

	Client = NULL;
	Status = usNone;
}

LPVOID CUpdater::OnCreate(HWND hWnd, LPARAM lParam)
{
	CUpdater *updater = reinterpret_cast<CUpdater*>(GetUserDataLong(hWnd));
	if (updater)
	{
		if (!updater->SetClient(MAIN_CLASS, MAIN_TITLE) && !::IsDebuggerPresent())
		{
			::PostQuitMessage(0);
			return 0L;
		}

		updater->Status = usIdle;
		updater->AssignBitmap(updater->LoadBitmapResource(IDB_BACKGROUND));
	}
	return 0L;
}

LPVOID CUpdater::OnClose(HWND hWnd)
{
	::PostQuitMessage(0);
	return 0L;
}

HWND CUpdater::SetClient(LPCTSTR szClientClass, LPCTSTR szClientTitle)
{
	Client = ::FindWindow(szClientClass, szClientTitle);

	if (Client)
	{
		DWORD processID;
		::GetWindowThreadProcessId(Client, &processID);
		HANDLE hProcess = ::OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processID);
		if (hProcess)
		{
			WCHAR szBuffer[MAX_PATH];
			if (::GetModuleFileNameEx(hProcess, NULL, szBuffer, MAX_PATH))
			{
				Module = std::wstring(szBuffer);
				return Client;
			}
		}
	}
	return NULL;
}

void CUpdater::GetClientVersion()
{
	Version.ReadFrom(Module.c_str());
}

void CUpdater::Initiate()
{
	if (Status == UpdateStatus::usIdle)
	{

/*		std::string version = "version=1.0.0.3";
		LPCTSTR szVersionPost = TEXT("service/get_version");
		LPCTSTR szHashPost = TEXT("service/get_version_hash");

		const std::wstring response = postConnection(szVersionPost, version);
		szUrl = response.substr(5);
		fileHash = postConnection(szHashPost, version);

		int behindBy;
		try
		{
			behindBy = _wtoi(response.substr(0, 3).c_str()); // Check if we can actually get how many updates we are behind by
		}
		catch (...)
		{
			// TODO: Error handling here
			InitiateUpdate();
			return;
			::PostQuitMessage(0);
		}

		if (behindBy > 1)
		{
			// TODO: Download the full version. 
			// Do some cleanup before doing so
			//DownloadFile(TEXT("http://speed.hetzner.de/100MB.bin"), filePath);
			//DownloadFile(fileURL, filePath);
		}
		else
		{
			// TODO: Download only the updates	
			// Possible cleanup
		}

		if (!quitKioskman()) {
			//::MessageBox(hWnd, L"Couldn't exit Kioskman", L"Title", MB_OK);
		}
		downloadStage();*/
	}
}