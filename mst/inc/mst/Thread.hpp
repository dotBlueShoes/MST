#pragma once

#include "BasicTypes.hpp"

#undef rename
#include <thread>
#undef rename
#define rename __forceinline

namespace mst::thread_n {
	using thread = std::thread;
}