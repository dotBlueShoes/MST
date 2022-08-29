#include <windows.h>
#include <stdio.h>
#include <fcntl.h>
#include <io.h>
#include <iostream>
#include <fstream>

#include "../Types.hpp"

namespace mst::winapi::debug::console {
	
	#ifdef DEBUG
	
	// Redirecting IO to console
	// https://cplusplus.com/forum/windows/58206/
	
	// Customize DEBUG info
	// https://www.codeproject.com/Articles/3135812/Creating-a-Console-based-window-for-debugging
	
	// To (get exsisting)/create WindowsTerminal TAB as DEBUG console
	// https://stackoverflow.com/questions/4089050/how-to-use-win32-api-to-get-handle-of-a-process-for-an-application-in-windows
	// https://duckduckgo.com/?q=powershell+retrive+console+handle&t=vivaldi&ia=web
	// https://docs.microsoft.com/en-us/troubleshoot/windows-server/performance/obtain-console-window-handle
	// https://stackoverflow.com/questions/25369285/how-can-i-get-all-window-handles-by-a-process-in-powershell
	
	void RedirectIO() {
		
		using namespace std;
		
		// Maximum mumber of lines the output console should have
		const WORD MAX_CONSOLE_LINES ( 500 );
		int64 hConHandle, lStdHandle;
		
		CONSOLE_SCREEN_BUFFER_INFO coninfo;
		FILE *fp;
	
		// Allocate a console for this app
		AllocConsole();
	
		// Set the screen buffer to be big enough to let us scroll text
		GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE),&coninfo);
		coninfo.dwSize.Y = MAX_CONSOLE_LINES;
		SetConsoleScreenBufferSize(GetStdHandle(STD_OUTPUT_HANDLE),coninfo.dwSize);
	
		// Redirect unbuffered STDOUT to the console
		lStdHandle = (long)GetStdHandle(STD_OUTPUT_HANDLE);
		hConHandle = _open_osfhandle(lStdHandle, _O_TEXT);
	
		fp = _fdopen( hConHandle, "w" );
	
		*stdout = *fp;
	
		setvbuf( stdout, NULL, _IONBF, 0 );
	
		// Redirect unbuffered STDIN to the console
	
		lStdHandle = (long)GetStdHandle(STD_INPUT_HANDLE);
		hConHandle = _open_osfhandle(lStdHandle, _O_TEXT);
	
		fp = _fdopen( hConHandle, "r" );
		*stdin = *fp;
		setvbuf( stdin, NULL, _IONBF, 0 );
	
		// Redirect unbuffered STDERR to the console
		lStdHandle = (long)GetStdHandle(STD_ERROR_HANDLE);
		hConHandle = _open_osfhandle(lStdHandle, _O_TEXT);
	
		fp = _fdopen( hConHandle, "w" );
	
		*stderr = *fp;
	
		setvbuf( stderr, NULL, _IONBF, 0 );
	
		// Make cout, wcout, cin, wcin, wcerr, cerr, wclog and clog
		// Point to console as well
		ios::sync_with_stdio();
	}
	
	#endif
	
}