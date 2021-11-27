#pragma once
#include <windows.h>

namespace mst::winapi {

	// Windows types https://docs.microsoft.com/en-us/windows/win32/winprog/windows-data-types

//#undef HANDLE

	#define stdcall __stdcall
	#define sal_in _In_
	#define sal_io _In_opt_

	using handleInstnace	= HINSTANCE;
	using windowProcedure	= WNDPROC;
	using windowClass		= WNDCLASSEXW;

	using objectHandle = HANDLE;

	using displayContextHandle = HDC;
	using cursorHandle = HCURSOR;
	using brushHandle = HBRUSH;
	using windowHandle = HWND;
	using iconHandle = HICON;

	// If one day i would do something with x32..
	using messageW = WPARAM; // uint64
	using messageL = LPARAM; // int64
	using retrivedMessage = MSG;

	using windowDrawContext = PAINTSTRUCT;
	using keysMap = HACCEL;

	enum class proceeded : int64 {
		pFalse = 0,
		pTrue = 1
	};

	//using dword = DWORD;

	// This is commonly outdated.
	//  It's recomended to use LPCWSTR instead.
	// using pcwstr = PCWSTR;

	//using lpcwstr = LPCWSTR;
	//using lpwstr = LPWSTR;

	// containers

	using smallRect = SMALL_RECT;
	using rect = RECT;
	using coord = COORD;

	// events-records

	using resizeEvent = WINDOW_BUFFER_SIZE_RECORD;
	using mouseEvent = MOUSE_EVENT_RECORD;
	using keyEvent = KEY_EVENT_RECORD;

	// 86x because it can be 32bit and 64bit same as an 'int'.
	using bool86x = BOOL;

	// Because it's an OS thing there's no point of specifying the size of variable in it's name.
	// Native Windows character encoding. https://docs.microsoft.com/pl-pl/cpp/cpp/char-wchar-t-char16-t-char32-t?view=msvc-160
	using wchar = utf_16le;
	using wchars = wchar[];

	using inputRecord = INPUT_RECORD;

	using consoleScreenBufferInfo = CONSOLE_SCREEN_BUFFER_INFOEX;
	using consoleFontInfo = CONSOLE_FONT_INFOEX;

}