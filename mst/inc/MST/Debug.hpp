#pragma once
#include "BasicTypes.hpp"
#include "DebugLevel.hpp"

#include "WinAPI/Debug/Console.hpp"

#ifdef DEBUG
namespace mst::debug {
	
	template <mst::winapi::debug::console::LogLevel messageLogLevel = mst::winapi::debug::console::LogLevel::Normal>
	constexpr block LogInfo(const string message) {
		mst::winapi::debug::console::LogInfo(message);
	}
	
	template <mst::winapi::debug::console::LogLevel messageLogLevel = mst::winapi::debug::console::LogLevel::Normal>
	constexpr block LogWarning(const string message) {
		mst::winapi::debug::console::LogWarning(message);
	}
	
	template <mst::winapi::debug::console::LogLevel messageLogLevel = mst::winapi::debug::console::LogLevel::Normal>
	constexpr block LogError(const string message) {
		mst::winapi::debug::console::LogError(message);
	}
	
}
#endif