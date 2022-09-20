#pragma once

#include "../../DebugLevel.hpp"
#include "../Types.hpp"

#include <stdio.h>
#include <fcntl.h>
#include <io.h>
#include <iostream>
#include <fstream>

#ifdef DEBUG
namespace mst::winapi::debug::console {
	
	/// Because DebugLevel
	using namespace mst::debug;
	
	FILE* stream; // NOT RLY SURE HOW IT WORKS.
	
	block RedirectIO() {
		AllocConsole();
		
		freopen_s(&stream, "conin$", "r", stdin);
		freopen_s(&stream, "conout$", "w", stdout);
		freopen_s(&stream, "conout$", "w", stderr);
	}
	
	template <LogLevel messageLogLevel = LogLevel::Normal>
	constexpr block LogInfo(const string message) {
		if constexpr ((LogLevel)DEBUG >= messageLogLevel)
			std::cout << " [ Info ]   " << message << '\n';
	}
	
	template <LogLevel messageLogLevel = LogLevel::Normal>
	constexpr block LogWarning(const string message) {
		if constexpr ((LogLevel)DEBUG >= messageLogLevel)
			std::cout << " [ Warn ]   " << message << '\n';
	}
	
	template <LogLevel messageLogLevel = LogLevel::Verbose>
	constexpr block LogError(const string message) {
		if constexpr ((LogLevel)DEBUG >= messageLogLevel)
			std::cout << " [ Error ]  " << message << '\n';
	}
	
	block ReleaseIO() {
		fclose(stdin);
		fclose(stdout);
	}
	
}
#endif

/* ABOUT - TODO
// Redirecting IO to console
// https://cplusplus.com/forum/windows/58206/
// https://stackoverflow.com/questions/13074435/getting-line-input-from-a-allocconsole-c
// https://www.educative.io/answers/what-is-freopens-in-c
// https://stackoom.com/en/question/1Ma8k
// Customize DEBUG info
// https://www.codeproject.com/Articles/3135812/Creating-a-Console-based-window-for-debugging
// To (get exsisting)/create WindowsTerminal TAB as DEBUG console
// https://stackoverflow.com/questions/4089050/how-to-use-win32-api-to-get-handle-of-a-process-for-an-application-in-windows
// https://duckduckgo.com/?q=powershell+retrive+console+handle&t=vivaldi&ia=web
// https://docs.microsoft.com/en-us/troubleshoot/windows-server/performance/obtain-console-window-handle
// https://stackoverflow.com/questions/25369285/how-can-i-get-all-window-handles-by-a-process-in-powershell
*/