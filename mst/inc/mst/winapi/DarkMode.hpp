#pragma once

// My understanding of what is happening...
//  basicly the uxtheme.lib file does not contain these definitions.
//  but the .dll itself does have them... and as it is answeard here:
//  https://stackoverflow.com/questions/3250467/what-is-inside-lib-file-of-static-library-statically-linked-dynamic-library-an
//  when we don't have a .lib and we want to comunicate with .dll
//  we need to manually call "LoadLibrary" and "FreeLibrary" alongside
//  with GetProcAddress to obtain the address of the functions and data items
//  stored in the .dll.
//  (We also need to cast the returned address to an appropriate pointer-to-function in order)

// 1903, 1809 are specfied here
// https://docs.microsoft.com/en-us/windows/win32/accessibility/accessibility-whatsnew

#include "types.hpp"

#include <uxtheme.h>

// What i really need now is a namespace that can be public/private as class
//  but ain't an class so i can use namespace aa = asample::asample;
//class {
//
//} darkmode;
// 
// This below is stupid as i want to access the value in this whole file. The above would be the solution.
//class {
//	bool86x value { false };
// public:
//	block Set() { value = proxy::ShouldAppsUseDarkMode() && !IsHighContrast(); }
//
//	getter const bool86x* const Reference() { return &value; }
//	getter const size Size() { return sizeof value; }
//
//} newIsEnabled;

namespace mst::winapi::window::darkmode {

	namespace proxy {

		enum class WINDOWCOMPOSITIONATTRIB {
			WCA_UNDEFINED = 0,
			WCA_NCRENDERING_ENABLED = 1,
			WCA_NCRENDERING_POLICY = 2,
			WCA_TRANSITIONS_FORCEDISABLED = 3,
			WCA_ALLOW_NCPAINT = 4,
			WCA_CAPTION_BUTTON_BOUNDS = 5,
			WCA_NONCLIENT_RTL_LAYOUT = 6,
			WCA_FORCE_ICONIC_REPRESENTATION = 7,
			WCA_EXTENDED_FRAME_BOUNDS = 8,
			WCA_HAS_ICONIC_BITMAP = 9,
			WCA_THEME_ATTRIBUTES = 10,
			WCA_NCRENDERING_EXILED = 11,
			WCA_NCADORNMENTINFO = 12,
			WCA_EXCLUDED_FROM_LIVEPREVIEW = 13,
			WCA_VIDEO_OVERLAY_ACTIVE = 14,
			WCA_FORCE_ACTIVEWINDOW_APPEARANCE = 15,
			WCA_DISALLOW_PEEK = 16,
			WCA_CLOAK = 17,
			WCA_CLOAKED = 18,
			WCA_ACCENT_POLICY = 19,
			WCA_FREEZE_REPRESENTATION = 20,
			WCA_EVER_UNCLOAKED = 21,
			WCA_VISUAL_OWNER = 22,
			WCA_HOLOGRAPHIC = 23,
			WCA_EXCLUDED_FROM_DDA = 24,
			WCA_PASSIVEUPDATEMODE = 25,
			WCA_USEDARKMODECOLORS = 26,
			WCA_LAST = 27
		};

		struct WINDOWCOMPOSITIONATTRIBDATA {
			WINDOWCOMPOSITIONATTRIB Attrib;
			PVOID pvData;
			SIZE_T cbData;
		};

		enum class IMMERSIVE_HC_CACHE_MODE {
			IHCM_USE_CACHED_VALUE,
			IHCM_REFRESH
		};

		enum class PreferredAppMode {
			Default,
			AllowDark,
			ForceDark,
			ForceLight,
			Max
		};

		// ----------
		// 1903 18362
		// ----------
		using rtlGetNtVersionNumbers =					void(__stdcall*)	(LPDWORD major, LPDWORD minor, LPDWORD build);	// 
		using setWindowCompositionAttribute =			int32(__stdcall*)	(HWND hWnd, WINDOWCOMPOSITIONATTRIBDATA*);		// 
		// ----------
		// 1809 17763
		// ----------
		using shouldAppsUseDarkMode =					bool(__stdcall*)	();												// ordinal 132
		using allowDarkModeForWindow =					bool(__stdcall*)	(HWND hWnd, bool allow);						// ordinal 133
		using allowDarkModeForApp =						bool(__stdcall*)	(bool allow);									// ordinal 135, in 1809
		using flushMenuThemes =							void(__stdcall*)	();												// ordinal 136
		using refreshImmersiveColorPolicyState =		void(__stdcall*)	();												// ordinal 104
		using isDarkModeAllowedForWindow =				bool(__stdcall*)	(HWND hWnd);									// ordinal 137
		using getIsImmersiveColorUsingHighContrast =	bool(__stdcall*)	(IMMERSIVE_HC_CACHE_MODE mode);					// ordinal 106
		using openNcThemeData =							HTHEME(__stdcall*)	(HWND hWnd, LPCWSTR pszClassList);				// ordinal 49
		// ----------
		// 1903 18362
		// ----------
		using shouldSystemUseDarkMode =					bool(__stdcall*)	();												// ordinal 138
		using setPreferredAppMode =			PreferredAppMode(__stdcall*)	(PreferredAppMode appMode);						// ordinal 135, in 1903
		using isDarkModeAllowedForApp =					bool(__stdcall*)	();												// ordinal 139


		setWindowCompositionAttribute			SetWindowCompositionAttribute			{ nullptr };
		shouldAppsUseDarkMode					ShouldAppsUseDarkMode					{ nullptr };
		allowDarkModeForWindow					AllowDarkModeForWindow					{ nullptr };
		allowDarkModeForApp						AllowDarkModeForApp						{ nullptr };
		flushMenuThemes							FlushMenuThemes							{ nullptr };
		refreshImmersiveColorPolicyState		RefreshImmersiveColorPolicyState		{ nullptr };
		isDarkModeAllowedForWindow				IsDarkModeAllowedForWindow				{ nullptr };
		getIsImmersiveColorUsingHighContrast	GetIsImmersiveColorUsingHighContrast	{ nullptr };
		openNcThemeData							OpenNcThemeData							{ nullptr };
		// ----------
		// 1903 18362
		// ----------
		shouldSystemUseDarkMode					ShouldSystemUseDarkMode					{ nullptr };
		setPreferredAppMode						SetPreferredAppMode						{ nullptr };

	}

	const wchar* nameImmersiveColorSet { L"ImmersiveColorSet" };

	// Whether the windows version we're running supports it or not.
	bool isSupported { false };	

	// Whether it is turned on or off. It can be only set as true if it is isSupported.
	//  ! To make it load new current value call 'CheckEnableState()'.
	bool86x isEnabled { false }; 

	proxyf bool AllowDarkModeForWindow(HWND window) { return proxy::AllowDarkModeForWindow(window, isEnabled); }
	proxyf bool AllowDarkModeForApp() { return proxy::AllowDarkModeForApp(isEnabled); }

	bool IsHighContrast() {
		HIGHCONTRASTW highContrast { sizeof highContrast };
		if (SystemParametersInfo(SPI_GETHIGHCONTRAST, sizeof highContrast, &highContrast, FALSE))
			return highContrast.dwFlags & HCF_HIGHCONTRASTON;

		return false;
	}

	block CheckEnableState() { isEnabled = proxy::ShouldAppsUseDarkMode() && !IsHighContrast(); }

	block RefreshTitleBarTheme(windowHandle window) {

		//if (proxy::SetWindowCompositionAttribute) { - dunno removed
			//if proxy::IsDarkModeAllowedForWindow(window)&& - dunno removed so messagebox works correctly.
		//if (proxy::ShouldAppsUseDarkMode() && !IsHighContrast()) dark = TRUE; // literary isEnabled var.
		
		proxy::WINDOWCOMPOSITIONATTRIBDATA data { proxy::WINDOWCOMPOSITIONATTRIB::WCA_USEDARKMODECOLORS, &isEnabled, sizeof isEnabled };
		proxy::SetWindowCompositionAttribute(window, &data);
	}

	inline constexpr bool IsEqual(const wchar* first, const wchar* second) {
		byte equals = 0;
		for (int i = 0; first[i] != L'\0'; ++i)
			equals += first[i] == second[i];
		return equals;
	}

	proceeded CheckWhetherImmersiveColorSet(const windowHandle& window, const wchar* pointer, uint64& counter) {
		// Ckeck : For some reason we get 10 of those messages but we want only 1.
		if (IsEqual(pointer, nameImmersiveColorSet) && ++counter == 10) {
			counter = 0;
			darkmode::CheckEnableState();
			SendMessageW(window, (uint32)input::ThemeChange, 0, 0);
			return proceeded::True;
		}

		return proceeded::False;
	}

		// For some reason the changing from lightmode to darkmode and vice versa
		// always take 10 calls in here with only wParam chaning.
		// This wParam holds information obaut "SystemParametersInfo"
		// I tried to decode it however those are not bit flags.
		// Instead they have a hex meaning and they are added to each other.

		// I found the a solution to discard unnecesery for me messages.
		// Which would be done with "PeekMessage" and flags PM_NOREMOVE, PM_REMOVE
		// however the actual working solution is to applay a counter and hope it will lock at 10 msgs.
	proceeded CheckMainWindowWhetherImmersiveColorSet(const windowHandle& window, const wchar* pointer, uint64& counter) {
		// Ckeck : For some reason we get 10 of those messages but we want only 1.
		if (IsEqual(pointer, nameImmersiveColorSet) && ++counter == 10) {
			counter = 0;
			darkmode::proxy::RefreshImmersiveColorPolicyState();
			darkmode::proxy::GetIsImmersiveColorUsingHighContrast(darkmode::proxy::IMMERSIVE_HC_CACHE_MODE::IHCM_REFRESH);
			darkmode::CheckEnableState();
			SendMessageW(window, (uint32)input::ThemeChange, 0, 0);
			return proceeded::True;
		}

		return proceeded::False;
	}

	void Initialize() {
		if (moduleInstance uxtheme { LoadLibraryEx(L"uxtheme.dll", nullptr, LOAD_LIBRARY_SEARCH_SYSTEM32) }) {

			// Getting the functions from the .dll
			proxy::OpenNcThemeData =						(proxy::openNcThemeData)						GetProcAddress(uxtheme, MAKEINTRESOURCEA(49));
			proxy::RefreshImmersiveColorPolicyState =		(proxy::refreshImmersiveColorPolicyState)		GetProcAddress(uxtheme, MAKEINTRESOURCEA(104));
			proxy::GetIsImmersiveColorUsingHighContrast =	(proxy::getIsImmersiveColorUsingHighContrast)	GetProcAddress(uxtheme, MAKEINTRESOURCEA(106));
			proxy::ShouldAppsUseDarkMode =					(proxy::shouldAppsUseDarkMode)					GetProcAddress(uxtheme, MAKEINTRESOURCEA(132));
			proxy::AllowDarkModeForWindow =					(proxy::allowDarkModeForWindow)					GetProcAddress(uxtheme, MAKEINTRESOURCEA(133));
			proxy::SetPreferredAppMode =					(proxy::setPreferredAppMode)					GetProcAddress(uxtheme, MAKEINTRESOURCEA(135));
			proxy::IsDarkModeAllowedForWindow =				(proxy::isDarkModeAllowedForWindow)				GetProcAddress(uxtheme, MAKEINTRESOURCEA(137));

			{
				// Why the checking?
				// Well either each function is being checked or
				// we're sure everything is in the right place.
				// We're not, however theres prob. a way to establish certain
				// things at the beggining and make these if's go, however this
				// requires a good understanging of whats in specific windows versions and
				// quite a big overhead on top of the program.
				//  yeah most certainly a thing to do one day.

				moduleInstance user32 { nullptr };
				if (!GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT, L"user32.dll", &user32))
					throw "Module address not found!";
				else {
					const char* functionName = "SetWindowCompositionAttribute";
					FARPROC functionAddress = GetProcAddress(user32, functionName);

					if (functionAddress == NULL) {
						throw "Function address not found!";
						return;
					}

					proxy::SetWindowCompositionAttribute = (proxy::setWindowCompositionAttribute)functionAddress;
				}
			}

			

			if (isSupported = proxy::OpenNcThemeData &&
				proxy::RefreshImmersiveColorPolicyState &&
				proxy::ShouldAppsUseDarkMode &&
				proxy::AllowDarkModeForWindow &&
				proxy::SetPreferredAppMode &&
				proxy::IsDarkModeAllowedForWindow) {

				//proxy::AllowDarkModeForApp(true);
				proxy::SetPreferredAppMode(proxy::PreferredAppMode::AllowDark);
				proxy::RefreshImmersiveColorPolicyState();

				CheckEnableState();

			}

			FreeLibrary(uxtheme);
			
		}
	}

}

//extern "C" {
//	NTSYSAPI	void stdcall RtlGetNtVersionNumbers			(sal_io LPDWORD major, sal_io LPDWORD minor, sal_io LPDWORD build);
//	WINUSERAPI	BOOL stdcall SetWindowCompositionAttribute	(sal_in HWND hWnd, sal_in WINDOWCOMPOSITIONATTRIBDATA*);
	// 1809 17763
//	THEMEAPI_(bool) ShouldAppsUseDarkMode(); // ordinal 132
//	THEMEAPI_(bool) AllowDarkModeForWindow(HWND hWnd, bool allow); // ordinal 133
//	THEMEAPI_(bool) AllowDarkModeForApp(bool allow); // ordinal 135, in 1809
//	THEMEAPI_(VOID) FlushMenuThemes(); // ordinal 136
//	THEMEAPI_(VOID) RefreshImmersiveColorPolicyState(); // ordinal 104
//	THEMEAPI_(bool) IsDarkModeAllowedForWindow(HWND hWnd); // ordinal 137
//	THEMEAPI_(bool) GetIsImmersiveColorUsingHighContrast(IMMERSIVE_HC_CACHE_MODE mode); // ordinal 106
//	THEMEAPI_(HTHEME) OpenNcThemeData(HWND hWnd, LPCWSTR pszClassList); // ordinal 49
//}