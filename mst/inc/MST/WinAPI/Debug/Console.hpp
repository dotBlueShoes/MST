#include <windows.h>
#include <stdio.h>
#include <fcntl.h>
#include <io.h>
#include <iostream>
#include <fstream>

#include "../Types.hpp"

namespace mst::winapi::debug::console {
	
	#ifdef DEBUG
	
	enum class LogLevel : int64 {
        Min = -1,		// Messages that must be displayed 
        Normal = 0,		// Messages that would normally be displayed
        Verbose = 1,	// Messages that don't necessarily need to be displayed
		Max = 2			// Everything
    };
	
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
	
	FILE* stream;
	
	void RedirectIO() {
		
		AllocConsole();
		
		freopen_s(&stream, "conin$", "r", stdin);
		freopen_s(&stream, "conout$", "w", stdout);
		freopen_s(&stream, "conout$", "w", stderr);
		
	}
	
	void LogInfo(const LogLevel messageLogLevel, const string message) {
		if ((LogLevel)DEBUG >= messageLogLevel)
			std::cout << "Info: " << message << '\n';
	}
	
	void LogWarning(const LogLevel messageLogLevel, const string message) {
		if ((LogLevel)DEBUG >= messageLogLevel)
			std::cout << "Warning: " << message << '\n';
	}
	
	void LogError(const LogLevel messageLogLevel, const string message) {
		if ((LogLevel)DEBUG >= messageLogLevel)
			std::cout << "Error: " << message << '\n';
	}
	
	void ReleaseIO() {
		fclose(stdin);
		fclose(stdout);
	}
	
	#endif
	
}