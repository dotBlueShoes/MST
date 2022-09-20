#pragma once

#ifdef DEBUG
namespace mst::debug {
	
	enum LogLevel : int64 {
		Min 	= -1,	// Messages that must be displayed.
		None 	= 0,	// Treated as if DEBUG wouldn't be defined.
		Normal 	= 1,	// Messages that would normally be displayed.
		Verbose = 2,	// Messages that don't necessarily need to be displayed.
		Max 	= 3		// Everything.
	};
	
}
#endif