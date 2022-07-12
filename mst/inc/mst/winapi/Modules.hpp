#pragma once
#include "Types.hpp"
#include "../CharacterTypes.hpp"

#include <libloaderapi.h>
#include <commctrl.h>
#include <richedit.h>

// Runtime dll's loading
// https://docs.microsoft.com/en-us/windows/win32/api/libloaderapi/nf-libloaderapi-loadlibrarya
namespace mst::winapi::modules {

	const array<wchar, 13> msfteditDLL { L"Msftedit.dll" };
	moduleInstance msftedit { nullptr };

	block LoadModule(moduleInstance& module, const wchar* dll, void (*f) (void)) {
		if ((module = LoadLibraryW(dll))) { f(); return; }
		// If the needed DLL won't be found.
		MessageBoxExW(nullptr, generalError.Pointer(), dll, MB_OK, 0);
		throw;
	}

	block LoadModule(moduleInstance& module, const wchar* dll) {
		if ((module = LoadLibraryW(dll))) return;
		// If the needed DLL won't be found.
		MessageBoxExW(nullptr, generalError.Pointer(), dll, MB_OK, 0);
		throw;
	}

	block UnloadModule(const moduleInstance& module) { FreeLibrary(module); }
}
