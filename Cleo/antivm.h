#pragma once
#include <Windows.h>
#include <iostream>
#include <cpr/cpr.h>

// Made by orlando

void toLowerCase(char* ptr, size_t size) {
	for (uint32_t i = 0; i < size; i++) {
		if (isupper(ptr[i]))
			ptr[i] = tolower(ptr[i]);
	}
}

bool isVM()
{
	std::string sysManufacturer, sysName;
	char buf[1000];
	DWORD sz = 1000;
	int ret;

	HKEY hKey;
	if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, "HARDWARE\\ACPI\\DSDT\\VBOX__", 0, KEY_READ, &hKey) == ERROR_SUCCESS) {
		return 1;
	}

	ret = RegGetValueA(HKEY_LOCAL_MACHINE, "SYSTEM\\CurrentControlSet\\Control\\SystemInformation", "SystemManufacturer",
		RRF_RT_ANY, NULL, &buf[0], &sz);

	toLowerCase(buf, strlen(buf));
	sysManufacturer = buf;

	if (ret == ERROR_SUCCESS && (sysManufacturer.find("vmware") != std::string::npos ||
			sysManufacturer.find("innotek gmbh") != std::string::npos ||
			sysManufacturer.find("qemu") != std::string::npos ||
			sysManufacturer.find("Apple inc.") != std::string::npos ||
			sysManufacturer.find("kvm") != std::string::npos ||
			sysManufacturer.find("system manufacturer") != std::string::npos)) {
				return 1;
		}

	ret = RegGetValueA(HKEY_LOCAL_MACHINE, "SYSTEM\\CurrentControlSet\\Control\\SystemInformation", "SystemProductName",
		RRF_RT_ANY, NULL, &buf[0], &sz);

	toLowerCase(buf, strlen(buf));
	sysName = buf;

	if (ret == ERROR_SUCCESS && (sysName.find("vmware") != std::string::npos ||
		sysName.find("virtualbox") != std::string::npos ||
		sysName.find("dell") != std::string::npos ||
		sysName.find("qemu") != std::string::npos ||
		sysName.find("intel") != std::string::npos ||
		sysName.find("asus") != std::string::npos ||
		sysName.find("system product name") != std::string::npos)) {
			return 1;
		}

	return 0;

}
