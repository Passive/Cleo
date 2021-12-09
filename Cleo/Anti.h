// Anti.h and Anti.cpp - Made by Orlando
#pragma once
#include <string>
#include <Windows.h>
#include <process.h>
#include <TlHelp32.h>
#include <vector>
#include <thread>
#include <winternl.h>
#include "AY_Obfuscate.h"

class Anti final {
private:
	typedef BOOL(WINAPI* LPFN_ISWOW64PROCESS) (HANDLE, PBOOL);
	std::vector<const char*> processes;
	void toLowerCase(char* ptr, size_t size);
	BOOL IsWow64();
	void check_usernames();
	inline HANDLE find_process(const char* process_name) const;
public:
	// Main
	void check_virtual_machine();
	void check_debugging();
	void check_analyzing();
	// Other
	void watch_dog();
	// Const/Deconst
	Anti() = default;
	Anti(bool& check_virtual_machine, bool& check_debugging, bool& check_analyzing, bool& watch_dog);
	~Anti() = default;
};