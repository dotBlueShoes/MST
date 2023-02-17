#pragma once

/* This is the starting point.
 *  Specific APIS that use this debugging system include this file.
 *  Then all is linked to Debug.hpp to be used.
*/

#define DEBUG if constexpr (DEBUG_LEVEL > 0)
#ifdef DEBUG_LEVEL
namespace mst::debug::logging {
		
	enum Level : int64 {
		Min 	= -1,	// Messages that must be displayed.
		None 	= 0,	// Treated as if DEBUG wouldn't be defined.
		Normal 	= 1,	// Messages that would normally be displayed.
		Verbose = 2,	// Messages that don't necessarily need to be displayed.
		Max 	= 3		// Everything.
	};
	
}
#endif