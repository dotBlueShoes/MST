#pragma once

#include "../../BasicTypes.hpp"
#include "../../Array.hpp"
#include "../Types.hpp"
#include "Palette.hpp"

namespace mst::winapi::console::frame {

	using namespace mst::array_n;
	using namespace palette;

	namespace window {
		const int16 x(140), y(24), bufforLength((x * y));

		// Frames consist of symbol that is being displayed and
		//  effect that is being set via command. Because the 
		//  commands amount is to very we're unable to set the size
		//  of a non character buffor.
		using frameCharacterBuffor = array<brush, bufforLength>;
		const int64 frameCount(3);
	}

	uint64 fps = 0;
	bool isRunning = true;

}