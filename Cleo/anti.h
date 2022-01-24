/*
		This code is written by Orlando, I take absolutely no
		credit in writing this.

		Yes it is bad code. Yes I will replace this later.
*/

#pragma once
#include <string>
#include <Windows.h>
#include <process.h>
#include <TlHelp32.h>
#include <vector>
#include <thread>
#include <assert.h>
#include "XorStr.h"

class Anti final {
private:
	std::vector<const char*> processes;
	void toLowerCase(char* ptr, size_t size);
	void kill_process(const char* process_name) const;
public:
	void check_virtual_machine();
	void check_debugging();
	void check_analyzing();
	void watch_dog();
	Anti() = default;
	Anti(bool& check_virtual_machine, bool& check_debugging, bool& check_analyzing, bool& watch_dog);

	~Anti() = default;
};
