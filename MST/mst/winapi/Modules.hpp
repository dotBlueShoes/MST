#pragma once
#include "types.hpp"
#include "../charactertypes.hpp"

#include <commctrl.h>
#include <richedit.h>

// Runtime dll's loading
// https://docs.microsoft.com/en-us/windows/win32/api/libloaderapi/nf-libloaderapi-loadlibrarya
namespace mst::winapi::modules {

	using namespace array_n;
	using namespace characters_n;

	//const array<wchar, 13> msfteditDLL (L"Msftedit.dll"_nt);
	const array<wchar, 13> msfteditDLL { L"Msftedit.dll" };
	moduleInstance msftedit { nullptr };

	block LoadModule(moduleInstance& module, const wchar* dll, void (*f) (void)) {
		if (module = LoadLibrary(dll)) { f(); return; }
		// If the needed DLL won't be found.
		//MessageBoxEx(nullptr, generalError.Pointer(), dll, MB_OK, 0);
		throw;
	}

	block LoadModule(moduleInstance& module, const wchar* dll) {
		if (module = LoadLibrary(dll)) return;
		// If the needed DLL won't be found.
		//MessageBoxEx(nullptr, generalError.Pointer(), dll, MB_OK, 0);
		throw;
	}

	block UnloadModule(const moduleInstance& module) { FreeLibrary(module); }
}
