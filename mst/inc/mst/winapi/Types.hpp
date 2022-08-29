// Windows types https://docs.microsoft.com/en-us/windows/win32/winprog/windows-data-types

#pragma once
#include "../BasicTypes.hpp"
#include "../Vector.hpp"
#include "../Array.hpp"

#define OEMRESOURCE /// For an application to use any of the OCR_ constants, the constant OEMRESOURCE must be defined
#include <windows.h>

namespace mst::winapi {

	using namespace array_n;

	#if WINDOWS_VERSION == _WIN32_WINNT_WIN10
		#define WINDOWS_VERSION_10 _WIN32_WINNT_WIN10
	#elif WINDOWS_VERSION == _WIN32_WINNT_WIN8
		#define WINDOWS_VERSION_8		_WIN32_WINNT_WIN8
	#elif WINDOWS_VERSION == _WIN32_WINNT_WIN7
		#define WINDOWS_VERSION_7		_WIN32_WINNT_WIN7
	#elif WINDOWS_VERSION == _WIN32_WINNT_VISTA
		#define WINDOWS_VERSION_VISTA	_WIN32_WINNT_VISTA
	#elif WINDOWS_VERSION == _WIN32_WINNT_WINXP
		#define WINODWS_VERSION_XP		_WIN32_WINNT_WINXP
	#endif

	#define stdcall		__stdcall
	//#define sal_in		_In_		// We have [in]
	//#define sal_io		_In_opt_	// We have [out]

	using handleInstance	= HINSTANCE;
	using moduleInstance	= HMODULE;
	using windowProcedure	= WNDPROC;
	using windowClass		= WNDCLASSEXW;

	using objectHandle = HANDLE;

	using multipleWindowHandle = HDWP;
	using displayContextHandle = HDC;
	using keysMapHandle = HACCEL;
	using cursorHandle = HCURSOR;
	using brushHandle = HBRUSH;
	using windowHandle = HWND;
	using iconHandle = HICON;
	using fontHandle = HFONT;

	using retrivedMessage = MSG;
	using messageW = WPARAM; // uint64
	using messageL = LPARAM; // int64
	
	using windowDrawContext = PAINTSTRUCT;
	using color = COLORREF;

	// containers

	using smallRect = SMALL_RECT;
	using coord = COORD;
	using point = POINT;
	using rect = RECT;

	// events-records

	using resizeEvent = WINDOW_BUFFER_SIZE_RECORD;
	using mouseEvent = MOUSE_EVENT_RECORD;
	using keyEvent = KEY_EVENT_RECORD;
	
	using dword [[deprecated("Use 'uint32' instead")]] = DWORD;
	// 86x because it can be 32bit and 64bit same as an 'int'.
	using bool86x = BOOL;

	// Because it's an OS thing there's no point of specifying the size of variable in it's name.
	// Native Windows character encoding. https://docs.microsoft.com/pl-pl/cpp/cpp/char-wchar-t-char16-t-char32-t?view=msvc-160
	using wchar = utf_16le;
	using wchars = wchar[];

	using inputRecord = INPUT_RECORD;

	using consoleScreenBufferInfo = CONSOLE_SCREEN_BUFFER_INFOEX;
	using consoleFontInfo = CONSOLE_FONT_INFOEX;

	using resourceType = uint32;

	// error-characters
	// CHANGE IT!
	const array<wchar, 6> generalError(L"Error");

}