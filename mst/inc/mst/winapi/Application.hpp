#pragma once
#include "Types.hpp"
#include "DarkMode.hpp"
#include "Modules.hpp"

namespace mst::winapi::application {
	block Initialize() { 
		modules::LoadModule(modules::msftedit, modules::msfteditDLL.Pointer()); 

		#ifdef WINDOWS_VERSION_10 
		// Those that work as function proxys can be initialized once. 
		//  As they are done with assigns they are being hopefully unloaded at the end of this function call.
		window::darkmode::Initialize();
		#endif
	}
	block Destroy() { modules::UnloadModule(modules::msftedit); }
}
