#pragma once
#include "Types.hpp"

namespace mst::winapi::window {

	enum class proceeded : int64 {
		False = 0,
		True = 1
	};

	enum class input : uint32 {

		/// "return 0" means that WM_PAINT instead handles the background color setting up.
		/// It makes passing the background color at window class initialization useless.
		EraseBackgroundOnCalledInvalidPortion = WM_ERASEBKGND,
		
		ControlStaticBeforeDraw = WM_CTLCOLORSTATIC,
		DialogWindowBeforeDraw = WM_CTLCOLORDLG,
		InitializeDialogWindow = WM_INITDIALOG,
		OnMouseLeftClickDown = WM_LBUTTONDOWN,
		NonClientAreaFocus = WM_NCACTIVATE,
		SettingChange = WM_SETTINGCHANGE, /// This calls whenever we switch from dark mode to light mode and reverse.
		NonClientAreaPaint = WM_NCPAINT,
		ThemeChange = WM_THEMECHANGED,
		Command = WM_COMMAND,
		Destroy = WM_DESTROY,
		Create = WM_CREATE,
		OnMove = WM_MOVING,
		Paint = WM_PAINT,
		Resize = WM_SIZE
	};

	namespace windowMode {

		// The correct way of handling a borderless window, that can be moved
		//  has no title bar and does have a menu.
		//  - Is to draw the whole window as custom. As making windows movable is possible
		//  using messages to control window position. The menus without WM_CAPTION
		//  appeard imposible to be fully edited without the style.
		// ! Thats why those functions will be only supposed to be called in the
		//  - CPP_WINAPI_FULLSCREEN_SAMPLE, - where no action is needed as there is no menu to handle.
		//  - CPP_WINAPI_CUSTOM_SAMPLE - where we're custom drawing the whole window.

		// The "windowBorderlessStyle" is not ment to work with windows that have menus and are supposed to be dragged windows.
		const uint64 windowBorderStyle = WS_OVERLAPPEDWINDOW,
			windowBorderlessStyle = WS_POPUP;

		// Contains information about the placement of the "windowed" window on the screen. Used when switching Fullscreen view.
		WINDOWPLACEMENT windowedPlacement { sizeof(windowedPlacement) };

		// Contains information about the placement of the "fullscreened" window on the single screen. Used when switching Fullscreen view.
		MONITORINFO monitorInfo { sizeof(monitorInfo) };

		void Windowed(windowHandle window) {
			SetWindowLongPtrW(window, GWL_STYLE, windowBorderStyle);						// Changes an attribute of the specified window.
			SetWindowPlacement(window, &windowedPlacement);								// Sets the show state and the restored, minimized, and maximized positions of the specified window.
			SetWindowPos(																		// Changes the size, position, and Z order of a child, pop-up, or top-level window.
				window,																	// Window Handle
				HWND_TOP,																		// Places the window at the top of the Z order.
				0, 0, 0, 0,																		// - skiped via flags
				SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_NOOWNERZORDER | SWP_FRAMECHANGED	// No change in place, size, z-order and other. SWP_FRAMECHANGED - Applies new frame styles set using the SetWindowLongPtr function.
			);
		}

		// Using this style it is imposible to custom draw the menu. 
		//  It is intended for windows which either don't have menus, or thosse that hide them afterwards.
		void WindowedBorderless(windowHandle window) {
			SetWindowLongPtr(window, GWL_STYLE, windowBorderlessStyle);					// Changes an attribute of the specified window.
			SetWindowPlacement(window, &windowedPlacement);								// Sets the show state and the restored, minimized, and maximized positions of the specified window.
			SetWindowPos(																		// Changes the size, position, and Z order of a child, pop-up, or top-level window.
				window,																	// Window Handle
				HWND_TOP,																		// Places the window at the top of the Z order.
				0, 0, 0, 0,																		// - skiped via flags
				SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_NOOWNERZORDER | SWP_FRAMECHANGED	// No change in place, size, z-order and other. SWP_FRAMECHANGED - Applies new frame styles set using the SetWindowLongPtr function.
			);
		}

		// Using this style it is imposible to custom draw the menu. 
		//  It is intended for windows which either don't have menus, or thosse that hide them afterwards.
		void Fullscreen(windowHandle window) {
			const uint64 windowStyles = GetWindowLongPtr(window, GWL_STYLE);
			//GetWindowPlacement(window, &windowMode::windowedPlacement);						// Just so instead of returning to the beginging state we return to the before this action placement.
			SetWindowLongPtr(window, GWL_STYLE, windowStyles & ~windowBorderStyle);			// Dunno knwo what is happening here.
			SetWindowPos(																	// Changes the size, position, and Z order of a child, pop-up, or top-level window.
				window,																		// A handle to the window.
				HWND_TOP,																	// Places the window at the top of the Z order.
				monitorInfo.rcMonitor.left, monitorInfo.rcMonitor.top,						// The new position.
				monitorInfo.rcMonitor.right - monitorInfo.rcMonitor.left,					// The new width of the window.
				monitorInfo.rcMonitor.bottom - monitorInfo.rcMonitor.top,					// The new height of the window.
				SWP_NOZORDER | SWP_NOOWNERZORDER | SWP_FRAMECHANGED							// The window sizing and positioning flags.
			);
		}

		// Because Windows 10 style hides border instead of removing it. The window when maximized will have an open space.
		//  This is not my issue. It's WinApi issue.
		void FullscreenWindowed(windowHandle window) {
			const uint64 windowStyles = GetWindowLongPtr(window, GWL_STYLE);
			//GetWindowPlacement(window, &windowMode::windowedPlacement);											// Just so instead of returning to the beginging state we return to the before this action placement.
			SetWindowLongPtr(window, GWL_STYLE, (windowStyles & ~windowBorderStyle) | windowBorderStyle);		// Dunno knwo what is happening here.
			SetWindowPos(																						// Changes the size, position, and Z order of a child, pop-up, or top-level window.
				window,																							// A handle to the window.
				HWND_TOP,																						// Places the window at the top of the Z order.
				monitorInfo.rcMonitor.left, monitorInfo.rcMonitor.top,											// The new position.
				monitorInfo.rcMonitor.right - monitorInfo.rcMonitor.left,										// The new width of the window.
				monitorInfo.rcMonitor.bottom - monitorInfo.rcMonitor.top,										// The new height of the window.
				SWP_NOZORDER | SWP_NOOWNERZORDER | SWP_FRAMECHANGED												// The window sizing and positioning flags.
			);
		}
	}

	const uint16 Register(
		const handleInstance& process,
		const wchar* windowClassName,
		const windowProcedure& procedure,
		const resourceType& iconId,
		const resourceType& iconSmallId,
		const resourceType& menuId,
		const brushHandle& backgroundBrush
	) {
		const iconHandle
			icon		{ LoadIcon(process, MAKEINTRESOURCE(iconId)) },
			iconSmall	{ LoadIcon(process, MAKEINTRESOURCE(iconSmallId)) };

		const wchar*		menu			{ MAKEINTRESOURCEW(menuId) };
		const cursorHandle	cursor			{ LoadCursor(nullptr, IDC_ARROW) };
		const uint32		windowStyle		{ CS_HREDRAW | CS_VREDRAW };
		
		windowClass windowProperties;
		windowProperties.cbSize 		= ( sizeof(windowClass) );
		windowProperties.style			= windowStyle;
		windowProperties.lpfnWndProc	= procedure;
		windowProperties.cbClsExtra		= 0;
		windowProperties.cbWndExtra		= 0;
		windowProperties.hInstance		= process;
		windowProperties.hIcon			= icon;
		windowProperties.hCursor		= cursor;
		windowProperties.hbrBackground	= backgroundBrush;
		windowProperties.lpszMenuName	= menu;
		windowProperties.lpszClassName	= windowClassName;
		windowProperties.hIconSm		= iconSmall;

		return RegisterClassExW(&windowProperties);
	}
	
	const uint16 RegisterCustom(
		const handleInstance& process,
		const wchar* windowClassName,
		const windowProcedure& procedure,
		const resourceType& iconId,
		const brushHandle& backgroundBrush
	) {
		const iconHandle icon { LoadIcon(process, MAKEINTRESOURCE(iconId)) };
			//iconSmall	{ LoadIcon(process, MAKEINTRESOURCE(iconSmallId)) };

		//const wchar*		menu			{ MAKEINTRESOURCEW(menuId) };
		const cursorHandle	cursor			{ LoadCursor(nullptr, IDC_ARROW) };
		const uint32		windowStyle		{ CS_HREDRAW | CS_VREDRAW };
		
		windowClass windowProperties;
		windowProperties.cbSize 		= ( sizeof(windowClass) );
		windowProperties.style			= windowStyle;
		windowProperties.lpfnWndProc	= procedure;
		windowProperties.cbClsExtra		= 0;
		windowProperties.cbWndExtra		= 0;
		windowProperties.hInstance		= process;
		windowProperties.hIcon			= icon;
		windowProperties.hCursor		= cursor;
		windowProperties.hbrBackground	= backgroundBrush;
		windowProperties.lpszMenuName	= nullptr;
		windowProperties.lpszClassName	= windowClassName;
		windowProperties.hIconSm		= nullptr;

		return RegisterClassExW(&windowProperties);
	}
	
	const windowHandle InitializeCustom (
		const handleInstance& process,
		const wchar* windowClassName,
		const wchar* windowTitle,
		const int32& windowState,
		const vector2<uint64>& windowPosition,
		const vector2<uint64>& windowSize
	) {
		
		windowHandle window = CreateWindowExW(
			WS_EX_APPWINDOW,
			windowClassName,
			windowTitle,
			WS_THICKFRAME | WS_POPUP,
			windowPosition.x, windowPosition.y,
			windowSize.x, windowSize.y,
			nullptr,
			nullptr,
			process,
			nullptr
		);

		if (!window) return nullptr;

		// Setup information needed for controlling the mode of the window. 
		GetWindowPlacement(window, &windowMode::windowedPlacement);
		GetMonitorInfo(												// Retrieves information about a display monitor. If the function succeeds, the return value is nonzero.
			MonitorFromWindow(window, MONITOR_DEFAULTTOPRIMARY),	// Retrieves a handle to the display monitor that has the largest area of intersection with the bounding rectangle of a specified window.
			&windowMode::monitorInfo								// monirotInfo reference variable.
		);

		//windowMode::WindowedBorderless(window);
		//SetMenu(window, nullptr);

		ShowWindow(window, windowState);
		UpdateWindow(window);

		return window;
	}

	const windowHandle Initialize(
		const handleInstance& process,
		const wchar* windowClassName,
		const wchar* windowTitle,
		const int32& windowState,
		const vector2<uint64>& windowPosition,
		const vector2<uint64>& windowSize
	) {
		
		windowHandle window = CreateWindowExW(
			WS_EX_APPWINDOW,
			windowClassName,
			windowTitle,
			WS_OVERLAPPEDWINDOW,
			windowPosition.x, windowPosition.y,
			windowSize.x, windowSize.y,
			nullptr,
			nullptr,
			process,
			nullptr
		);

		if (!window) return nullptr;

		// Setup information needed for controlling the mode of the window. 
		GetWindowPlacement(window, &windowMode::windowedPlacement);
		GetMonitorInfo(												// Retrieves information about a display monitor. If the function succeeds, the return value is nonzero.
			MonitorFromWindow(window, MONITOR_DEFAULTTOPRIMARY),	// Retrieves a handle to the display monitor that has the largest area of intersection with the bounding rectangle of a specified window.
			&windowMode::monitorInfo								// monirotInfo reference variable.
		);

		//windowMode::WindowedBorderless(window);
		//SetMenu(window, nullptr);

		ShowWindow(window, windowState);
		UpdateWindow(window);

		return window;
	}
	
	const windowHandle CreateChildWindow(
		const handleInstance& process,
		const windowHandle& parentWindow,
		const windowProcedure& procedure,
		const cursorHandle& cursor,
		const wchar* windowClassName,
		const brushHandle& backgroundBrush,
		const int32& windowState,
		const vector2<uint64>& windowOffset,
		const vector2<uint64>& windowSize,
		const uint32 windowExtraStyles = 0
	) {
		const uint32 classStyle ( CS_HREDRAW | CS_VREDRAW );
		
		windowClass windowProperties;
			
		windowProperties.cbSize 		= ( sizeof(windowClass) );
		windowProperties.style			= classStyle;
		windowProperties.lpfnWndProc	= procedure;
		windowProperties.cbClsExtra		= 0;
		windowProperties.cbWndExtra		= 0;
		windowProperties.hInstance		= process;
		windowProperties.hIcon			= nullptr;
		windowProperties.hCursor		= cursor;
		windowProperties.hbrBackground	= backgroundBrush;
		windowProperties.lpszMenuName	= nullptr;
		windowProperties.lpszClassName	= windowClassName;
		windowProperties.hIconSm		= nullptr;
		
		RegisterClassExW(&windowProperties);
		
		{
			const windowHandle childWindow = CreateWindowExW(
				0, 
				windowClassName, 
				nullptr, 
				WS_CHILD | windowExtraStyles, 
				windowOffset.x, 
				windowOffset.y, 
				windowSize.x, 
				windowSize.y, 
				parentWindow, 
				nullptr, 
				process, 
				nullptr
			);
			
			ShowWindow(childWindow, windowState);
			UpdateWindow(childWindow);
			return childWindow;
		}
	}

}