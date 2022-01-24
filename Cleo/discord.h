/*
		This code is written by Orlando, I take absolutely no
		credit in writing this. 
		
		Yes it is bad code. Yes I will replace this later.
*/

#pragma once
#include <iostream>
#include <Windows.h>
#include <fstream>
#include <filesystem>
#include <cpr/cpr.h>
#include <json/value.h>
#include <json/json.h>

namespace Discord {
	bool checkToken(const std::string& match);
	struct Discord {
	private:
		cpr::Response GetRequest(const std::string& token);
	public:
		void init(const std::string& token);
		std::string getTokenInfo();
		std::string token;
		std::string username = "none";
		std::string id = "none";
		std::string avatar = "none";
		std::string email = "none";
		std::string phone = "none";
		std::string locale = "none";
		bool nsfw_allowed{};
		bool mfa_enabled{};
		bool verified{};
		int nitro{};
	};

};
