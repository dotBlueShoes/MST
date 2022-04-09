#pragma once

#include "types.hpp"
#include "..\array.hpp"
#include "..\CharacterTypes.hpp"

// HERE!
// WATCHOUT FOR THIS FILE!!!!
// _nt was being changed!!

namespace mst::winapi::registry {

	using characters_n::operator""_nt;

	// Generally, data consisting of more than one or two kilobytes (K) should be stored as a 
	//  file and referred to by using a key in the registry rather than being stored as a value. 
	//  Instead of duplicating large pieces of data in the registry, an application should 
	//  save the data as a file and refer to the file. Executable binary code should never be 
	//  stored in the registry.

	// An application must open a key before it can add data to the registry. To open a key, 
	//  an application must supply a handle to another key in the registry that is already open. 
	//  The system defines predefined keys that are always open.
	//  https://docs.microsoft.com/en-us/windows/win32/sysinfo/predefined-keys
	//  The following are handles to the predefined keys:
	//   - HKEY_CLASSES_ROOT,
	//   - HKEY_CURRENT_CONFIG,
	//   - HKEY_CURRENT_USER,
	//	 - HKEY_CURRENT_USER_LOCAL_SETTINGS,
	//	 - HKEY_LOCAL_MACHINE,
	//	 - HKEY_PERFORMANCE_DATA,
	//	 - HKEY_PERFORMANCE_NLSTEXT,
	//	 - HKEY_PERFORMANCE_TEXT,
	//	 - HKEY_USERS.

	// The following predefined keys are always open: 
	//  - HKEY_LOCAL_MACHINE, 
	//  - HKEY_CLASSES_ROOT, 
	//  - HKEY_USERS,
	//  - HKEY_CURRENT_USER.

	const size maxVaxTreeLevelDeep { 32 },
		maxValueName { 16383 },
		maxKeyName { 255 };
		//maxValue {};

	block AddRegistryKey(const windowHandle& window) {

		// So firstly we need to open the key that we want to modify.
		//  in a tree order from the highest key. each key has to be oppened.

		//LSTATUS RegOpenKeyExA(
		//	[in]           HKEY   hKey,
		//	[in, optional] LPCSTR lpSubKey,
		//	[in]           DWORD  ulOptions,
		//	[in]           REGSAM samDesired,
		//	[out]          PHKEY  phkResult
		//);

		LSTATUS status;
		HKEY result;

		const array<wchar, 57> fullPath(LR"(Directory\shell\mst.menu.openwith\sampleKey\subSampleKey)");

		status = RegCreateKeyEx(HKEY_CLASSES_ROOT, fullPath.Pointer(), 0, 0, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, 0, &result, 0);
		if (status == ERROR_SUCCESS) MessageBoxEx(window, L"Main", L"Success", MB_OK, 0);

		status = RegCloseKey(result);
		if (status == ERROR_SUCCESS) MessageBoxEx(window, L"Main", L"Close", MB_OK, 0);

		// \\shell\\mst.menu.openwith

		// status = RegOpenKeyEx(HKEY_CLASSES_ROOT, L"Directory\\shell\\find", 0, KEY_ALL_ACCESS, &result1);
		// if (status == ERROR_SUCCESS) MessageBoxEx(window, L"Main", L"Success1", MB_OK, 0);
		// //status = RegOpenKeyEx(result1, L"shell", 0, KEY_ALL_ACCESS, &result2);
		// //if (status == ERROR_SUCCESS) MessageBoxEx(window, L"Main", L"Success2", MB_OK, 0);
		// //status = RegOpenKeyEx(result2, L"find", 0, KEY_ALL_ACCESS, &result3);
		// //if (status == ERROR_SUCCESS) MessageBoxEx(window, L"Main", L"Success3", MB_OK, 0);
		// status = RegOpenKeyEx(result1, L"command", 0, KEY_ALL_ACCESS, &result4);
		// if (status == ERROR_SUCCESS) MessageBoxEx(window, L"Main", L"Success4", MB_OK, 0);

		// status = RegCloseKey(result4);
		// if (status == ERROR_SUCCESS) MessageBoxEx(window, L"Main", L"Close4", MB_OK, 0);
		// //status = RegCloseKey(result3);
		// //if (status == ERROR_SUCCESS) MessageBoxEx(window, L"Main", L"Close3", MB_OK, 0);
		// //status = RegCloseKey(result2);
		// //if (status == ERROR_SUCCESS) MessageBoxEx(window, L"Main", L"Close2", MB_OK, 0);
		// status = RegCloseKey(result1);
		// if (status == ERROR_SUCCESS) MessageBoxEx(window, L"Main", L"Close1", MB_OK, 0);

		// dzia³a!!!
		// ogarnij qemu windows 10/11
		// postaraj siê jak najwiêcej usun¹æ na nim
		// zbadaæ czy da siê to przywróciæ, czy wymaga restartu
		// i wszystkie odkryte œcie¿ki + œcie¿ki z przygody tworzenia context menu zapisaæ
		//  w formie gdzie rozdzielamy na root, machine, user : empty, file, folder, inne bo coœ chyba by³o.

	}

	block RemoveRegistryKey(const windowHandle& window) {

		const array<wchar, 57> subKey(LR"(Directory\shell\mst.menu.openwith\sampleKey\subSampleKey)");
		const array<wchar, 44> fullPath(LR"(Directory\shell\mst.menu.openwith\sampleKey)");

		LSTATUS status;
		
		status = RegDeleteKeyEx(HKEY_CLASSES_ROOT, subKey.Pointer(), KEY_WOW64_64KEY, 0);
		if (status == ERROR_SUCCESS) MessageBoxEx(window, L"Main", L"Remove1", MB_OK, 0);

		status = RegDeleteKeyEx(HKEY_CLASSES_ROOT, fullPath.Pointer(), KEY_WOW64_64KEY, 0);
		if (status == ERROR_SUCCESS) MessageBoxEx(window, L"Main", L"Remove2", MB_OK, 0);

	}

	block EditRegistryKey(const windowHandle& window) {
		// https://docs.microsoft.com/en-us/windows/win32/api/winternl/nf-winternl-ntrenamekey
	}

}