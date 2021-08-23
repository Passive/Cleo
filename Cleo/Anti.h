// Anti.h and Anti.cpp - Made by Orlando
#pragma once
#include <string>
#include <Windows.h>
#include <process.h>
#include <TlHelp32.h>
#include <vector>
#include <thread>
#include "confuser.h"
#include <assert.h>

class Anti final {
private:
	// Priv Vars
	std::vector<const char*> processes;
	// Priv Funcs
	void toLowerCase(char* ptr, size_t size);
	void kill_process(const char* process_name) const;
public:
	//mutable bool virtual_machine = false;
	//mutable bool debugging = false;
	//mutable bool analyzing = false;
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
