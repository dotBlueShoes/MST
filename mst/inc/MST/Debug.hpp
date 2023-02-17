#pragma once
#include "BasicTypes.hpp"
#include "DebugLevel.hpp"

#include "WinAPI/Debug/Console.hpp"

#ifdef DEBUG_LEVEL
namespace mst::debug::logger {
	
	namespace log = mst::winapi::debug::console::logging;
	
	template <log::Level messageLogLevel = log::Level::Normal>
	constexpr block Info(const string message) {
		mst::winapi::debug::console::LogInfo(message);
	}
	
	template <log::Level messageLogLevel = log::Level::Normal>
	constexpr block Warning(const string message) {
		mst::winapi::debug::console::LogWarning(message);
	}
	
	template <log::Level messageLogLevel = log::Level::Normal>
	constexpr block Error(const string message) {
		mst::winapi::debug::console::LogError(message);
	}
	
}
#endif