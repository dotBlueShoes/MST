#pragma once
#include "types.hpp"
//#include "../framework.hpp"
//using namespace winapi;

namespace mst::winapi::window {

	uint16 Register(
		const handleInstnace& windowInstance,
		const wchar* windowClassName,
		const windowProcedure& procedure,
		const resourceType& iconId,
		const resourceType& iconSmallId,
		const resourceType& menuId
	) {
		windowClass windowProperties;
		windowProperties.cbSize = { sizeof windowClass };

		const iconHandle
			icon		{ LoadIcon(windowInstance, MAKEINTRESOURCE(iconId)) },
			iconSmall	{ LoadIcon(windowInstance, MAKEINTRESOURCE(iconSmallId)) };

		const wchar*		menu			{ MAKEINTRESOURCEW(menuId) };
		const brushHandle	backgroundColor { CreateSolidBrush(RGB(29, 29, 29)) };  //(brushHandle)(COLOR_WINDOW + 1)
		const cursorHandle	cursor			{ LoadCursor(nullptr, IDC_ARROW) };
		const uint32				windowStyle		{ CS_HREDRAW | CS_VREDRAW };

		windowProperties.style			= windowStyle;
		windowProperties.lpfnWndProc	= procedure;
		windowProperties.cbClsExtra		= 0;
		windowProperties.cbWndExtra		= 0;
		windowProperties.hInstance		= windowInstance;
		windowProperties.hIcon			= icon;
		windowProperties.hCursor		= cursor;
		windowProperties.hbrBackground	= backgroundColor;
		windowProperties.lpszMenuName	= menu;
		windowProperties.lpszClassName	= windowClassName;
		windowProperties.hIconSm		= iconSmall;

		return RegisterClassExW(&windowProperties);
	}

	int64 Initialize(
		const handleInstnace& windowInstance,
		const wchar* windowClassName,
		const wchar* windowTitle,
		const int32& nCmdShow
	) {
		windowHandle window = CreateWindowEx(
			WS_EX_APPWINDOW,
			windowClassName,
			windowTitle,
			WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT, 0,
			CW_USEDEFAULT, 0,
			nullptr,
			nullptr,
			windowInstance,
			nullptr
		);

		if (!window) return 1;

		ShowWindow(window, nCmdShow);
		UpdateWindow(window);

		return 0;
	}

}