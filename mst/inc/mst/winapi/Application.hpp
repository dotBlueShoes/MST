#pragma once

#include "Debug/Console.hpp"
#include "Types.hpp"
#include "DarkMode.hpp"
#include "Modules.hpp"

namespace mst::winapi::application {
	
	block Initialize() { 
	
		// Initializating DEBUG console window.
		{	namespace dbg = mst::winapi::debug::console;
			if constexpr (DEBUG != dbg::LogLevel::None)
				dbg::RedirectIO();
		}
	
		modules::LoadModule(modules::msftedit, modules::msfteditDLL.Pointer()); 

		if constexpr (SYSTEM_VERSION == SystemVersion::Windows10)  {
			// Those that work as function proxys can be initialized once. 
			//  As they are done with assigns they are being hopefully unloaded at the end of this function call.
			window::darkmode::Initialize();
		}
	}
	
	block Destroy() { 
		modules::UnloadModule(modules::msftedit); 
		
		// ReleaseIO used by DEBUG assigned with dbg::RedirectIO().
		{	namespace dbg = mst::winapi::debug::console;
			dbg::ReleaseIO();
		}
	}
	
}
