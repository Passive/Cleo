#include "Anti.h"

// Private funcs

void Anti::toLowerCase(char* ptr, const size_t size)
{
	for (uint32_t i = 0; i < size; i++) {
		if (isupper(ptr[i]))
			ptr[i] = tolower(ptr[i]);
	}
}

BOOL Anti::IsWow64()
{
	BOOL bIsWow64 = FALSE;
	if (const auto fnIsWow64Process = reinterpret_cast<LPFN_ISWOW64PROCESS>(GetProcAddress(
		GetModuleHandle(TEXT("kernel32")), "IsWow64Process")); nullptr != fnIsWow64Process && !fnIsWow64Process(GetCurrentProcess(), &bIsWow64)) {
		return 0;
	}
	return bIsWow64;
}

void Anti::check_usernames() const
{
	char szUsername[1024];
	DWORD dwUser = sizeof(szUsername);
	GetUserNameA(szUsername, &dwUser);

	/*
	* Online Auto Analysis VM's use these windows usernames
	*/

	if (const char* user1 = AY_OBFUSCATE("george"); strcmp(szUsername, user1) == 0) // <- Virus Total
		exit(EXIT_FAILURE);

	if (const char* user2 = AY_OBFUSCATE("JOHN-PC"); strcmp(szUsername, user2) == 0) // <- Virus Total
		exit(EXIT_FAILURE);

	if (const char* user3 = AY_OBFUSCATE("Sandbox"); strcmp(szUsername, user3) == 0)
		exit(EXIT_FAILURE);

	if (const char* user4 = AY_OBFUSCATE("sand box"); strcmp(szUsername, user4) == 0)
		exit(EXIT_FAILURE);

	if (const char* user5 = AY_OBFUSCATE("John Doe"); strcmp(szUsername, user5) == 0)
		exit(EXIT_FAILURE);

	if (const char* user6 = AY_OBFUSCATE("malware"); strcmp(szUsername, user6) == 0)
		exit(EXIT_FAILURE);

	if (const char* user7 = AY_OBFUSCATE("Peter Wilson"); strcmp(szUsername, user7) == 0)// <- Virus Total
		exit(EXIT_FAILURE);

	if (const char* user8 = AY_OBFUSCATE("virus"); strcmp(szUsername, user8) == 0)
		exit(EXIT_FAILURE);

	if (const char* user9 = AY_OBFUSCATE("maltest"); strcmp(szUsername, user9) == 0)
		exit(EXIT_FAILURE);

	if (const char* user10 = AY_OBFUSCATE("CurrentUser"); strcmp(szUsername, user10) == 0)
		exit(EXIT_FAILURE);
}

inline HANDLE Anti::find_process(const char* process_name)
{
	const HANDLE hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPALL, NULL);
	PROCESSENTRY32 pEntry;
	pEntry.dwSize = sizeof(pEntry);
	BOOL hRes = Process32First(hSnapShot, &pEntry);
	while (hRes)
	{
		if (strcmp(pEntry.szExeFile, process_name) == 0)
		{
			if (const HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, 0, pEntry.th32ProcessID); hProcess != nullptr)
				return hProcess;
		}
		hRes = Process32Next(hSnapShot, &pEntry);
	}
	CloseHandle(hSnapShot);
	return 0;
}

// Public Funcs

void Anti::check_virtual_machine()
{
	char buf[1000];
	DWORD sz = 1000;

	HKEY hKey1;
	if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, AY_OBFUSCATE("HARDWARE\\ACPI\\DSDT\\VBOX__"), 0, KEY_READ, &hKey1) == ERROR_SUCCESS)
		exit(EXIT_FAILURE);

	HKEY hKey2;
	if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, AY_OBFUSCATE("SOFTWARE\\Microsoft\\Virtual Machine\\Guest\\Parameters"), 0, KEY_READ, &hKey2) == ERROR_SUCCESS)
		exit(EXIT_FAILURE);

	// Wine isn't a virtual machine, but it should still be detected
	HKEY hKey3;
	if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, AY_OBFUSCATE("SOFTWARE\\Wine"), 0, KEY_READ, &hKey3) == ERROR_SUCCESS)
		exit(EXIT_FAILURE);

	int ret = RegGetValueA(HKEY_LOCAL_MACHINE, AY_OBFUSCATE("SYSTEM\\CurrentControlSet\\Control\\SystemInformation"),
		AY_OBFUSCATE("SystemManufacturer"),
		RRF_RT_ANY, nullptr, &buf[0], &sz);

	toLowerCase(buf, strlen(buf));
	if (std::string sysManufacturer = buf; ret == ERROR_SUCCESS && (sysManufacturer.find(AY_OBFUSCATE("vmware")) != std::string::npos ||
		sysManufacturer.find(AY_OBFUSCATE("innotek gmbh")) != std::string::npos ||
		sysManufacturer.find(AY_OBFUSCATE("qemu")) != std::string::npos ||
		sysManufacturer.find(AY_OBFUSCATE("Apple inc.")) != std::string::npos ||
		sysManufacturer.find(AY_OBFUSCATE("kvm")) != std::string::npos ||
		sysManufacturer.find(AY_OBFUSCATE("parallel")) != std::string::npos ||
		sysManufacturer.find(AY_OBFUSCATE("system manufacturer")) != std::string::npos))
		exit(EXIT_FAILURE);

	ret = RegGetValueA(HKEY_LOCAL_MACHINE, AY_OBFUSCATE("SYSTEM\\CurrentControlSet\\Control\\SystemInformation"), AY_OBFUSCATE("SystemProductName"),
		RRF_RT_ANY, nullptr, &buf[0], &sz);

	toLowerCase(buf, strlen(buf));

	if (std::string sysName = buf; ret == ERROR_SUCCESS && (sysName.find(AY_OBFUSCATE("vmware")) != std::string::npos ||
		sysName.find(AY_OBFUSCATE("virtualbox")) != std::string::npos ||
		sysName.find(AY_OBFUSCATE("parallel")) != std::string::npos ||
		sysName.find(AY_OBFUSCATE("qemu")) != std::string::npos ||
		sysName.find(AY_OBFUSCATE("virtio")) != std::string::npos ||
		sysName.find(AY_OBFUSCATE("vbox")) != std::string::npos ||
		sysName.find(AY_OBFUSCATE("system product name")) != std::string::npos))
		exit(EXIT_FAILURE);
}

void Anti::check_debugging()
{
	// Check 1
	if (IsDebuggerPresent())
		exit(EXIT_FAILURE);
	// Check 2
	BOOL is_debugging;
	CheckRemoteDebuggerPresent(GetCurrentProcess(), &is_debugging);
	if (is_debugging)
		exit(EXIT_FAILURE);
	// Check 3
	SetLastError(0);
	OutputDebugStringW(L"null");
	if (GetLastError() != 0)
		exit(EXIT_FAILURE);

	// Check 4
	__try {
		DebugBreak();
	}
	__except (GetExceptionCode() == EXCEPTION_BREAKPOINT ?
		EXCEPTION_EXECUTE_HANDLER : EXCEPTION_CONTINUE_SEARCH) {
	}

	// Check 5
	CONTEXT ctx = {};
	ctx.ContextFlags = CONTEXT_DEBUG_REGISTERS;
	if (GetThreadContext(GetCurrentThread(), &ctx)) {
		if (ctx.Dr0 != 0 || ctx.Dr1 != 0 || ctx.Dr2 != 0 || ctx.Dr3 != 0)
			exit(EXIT_FAILURE);
	}

	// Check 6
	// KD debug check
	constexpr ULONG_PTR UserSharedData = 0x7FFE0000;
	const UCHAR KdDebuggerEnabledByte = *reinterpret_cast<UCHAR*>((UserSharedData + 0x2D4));
	const BOOLEAN KdDebuggerEnabled = (KdDebuggerEnabledByte & 0x1) == 0x1;
	if (const BOOLEAN KdDebuggerNotPresent = (KdDebuggerEnabledByte & 0x2) == 0; KdDebuggerEnabled || !KdDebuggerNotPresent)
		exit(EXIT_FAILURE);

	PDWORD pNtGlobalFlagWoW64 = nullptr;
	const auto _teb32 = reinterpret_cast<BYTE*>(__readfsdword(0x18));
	const DWORD _peb32 = *reinterpret_cast<DWORD*>(_teb32 + 0x30);
	PDWORD pNtGlobalFlag = reinterpret_cast<PDWORD>(_peb32 + 0x68);
	if (this->IsWow64())
	{
		BYTE* _teb64 = reinterpret_cast<BYTE*>(__readfsdword(0x18)) - 0x2000;
		const DWORD64 _peb64 = *reinterpret_cast<DWORD64*>(_teb64 + 0x60);
		pNtGlobalFlagWoW64 = reinterpret_cast<PDWORD>(_peb64 + 0xBC);
	}

	const BOOL normalDetected = pNtGlobalFlag && *pNtGlobalFlag & 0x00000070;
	if (const BOOL wow64Detected = pNtGlobalFlagWoW64 && *pNtGlobalFlagWoW64 & 0x00000070; normalDetected || wow64Detected)
		exit(EXIT_FAILURE);
}

void Anti::check_analyzing()
{
	const HMODULE hKernel32 = GetModuleHandle("kernel32.dll");
	if (hKernel32 == nullptr)
		exit(EXIT_FAILURE);
	if (GetProcAddress(hKernel32, AY_OBFUSCATE("wine_get_unix_file_name")) != nullptr)
		exit(EXIT_FAILURE);

	// Kill all blacklisted processes (ONCE)
	for (auto const& process : this->processes) {
		const HANDLE proc = find_process(process);
		if (proc != nullptr) {
			CloseHandle(proc);
			exit(EXIT_FAILURE);
		}
		CloseHandle(proc);
	}
}

// This function should only run on a *detached* thread
void Anti::watch_dog()
{
	while (true) {
		for (const auto& process : this->processes) {
			const HANDLE hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPALL, NULL);
			PROCESSENTRY32 pEntry;
			pEntry.dwSize = sizeof(pEntry);
			BOOL hRes = Process32First(hSnapShot, &pEntry);
			while (hRes)
			{
				if (strcmp(pEntry.szExeFile, process) == 0)
				{
					if (const HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, 0, pEntry.th32ProcessID); hProcess != nullptr)
					{
						TerminateProcess(hProcess, 9);
						CloseHandle(hProcess);
					}
				}
				hRes = Process32Next(hSnapShot, &pEntry);
			}
			CloseHandle(hSnapShot);
			Sleep(100);
		}
		Sleep(200);
	}
}

Anti::Anti(bool& check_virtual_machine, bool& check_debugging, bool& check_analyzing, bool& watch_dog)
{
	this->check_usernames(); // Top Priority

	if (check_virtual_machine)
		this->check_virtual_machine();

	if (check_debugging)
		this->check_debugging();

	if (check_analyzing) {
		// Add more if you like
		const char* procmon = AY_OBFUSCATE("procmon.exe"); // Sysinternals
		const char* ollydbg = AY_OBFUSCATE("ollydbg.exe");
		const char* x32dbg = AY_OBFUSCATE("x32dbg.exe"); // x64dbg blacklist is useless considering Cleo is meant to be on x86.
		const char* glasswire = AY_OBFUSCATE("glasswire.exe");
		const char* mmc = AY_OBFUSCATE("mmc.exe");
		const char* wireshark = AY_OBFUSCATE("Wireshark.exe");
		const char* fiddler = AY_OBFUSCATE("Fiddler.exe");
		const char* netlimiter = AY_OBFUSCATE("NLClientApp.exe");
		const char* cheat_engine1 = AY_OBFUSCATE("cheatengine-x86_64.exe");
		const char* ida = AY_OBFUSCATE("idaq.exe");
		const char* vm_proc1 = AY_OBFUSCATE("VMSrvc.exe");
		const char* vm_proc2 = AY_OBFUSCATE("VMUSrvc.exe");
		const char* http_debugger = AY_OBFUSCATE("httpdebugger.exe");
		const char* windbg = AY_OBFUSCATE("windbg.exe");
		const char* dumpcap = AY_OBFUSCATE("dumpcap.exe");
		const char* process_hacker = AY_OBFUSCATE("ProcessHacker.exe");
		const char* cutter = AY_OBFUSCATE("cutter.exe");
		const char* immunity_debugger = AY_OBFUSCATE("ImmunityDebugger.exe");
		const char* binary_ninja = AY_OBFUSCATE("binaryninja.exe");
		const char* cheat_engine2 = AY_OBFUSCATE("cheatengine-x86_64-SSE4-AVX2.exe");
		//Note: Dangerous game to be played if task manager is added to the vector :)
		//const char* taskmgr = AY_OBFUSCATE("Taskmgr.exe");
		this->processes = { procmon, ollydbg, x32dbg, glasswire, mmc, wireshark, fiddler, netlimiter,
			cheat_engine1, ida, vm_proc1, vm_proc2, http_debugger, windbg, dumpcap, process_hacker, cutter,
			immunity_debugger, binary_ninja, cheat_engine2 }; // taskmgr
		this->check_analyzing();
	}

	if (watch_dog) {
		std::thread wd(&Anti::watch_dog, this);
		wd.detach();
	}
}
