#pragma once

#include "basictypes.hpp"

#undef rename
#include <string>
#undef rename
#define rename __forceinline

namespace mst::string_n {
	using string = std::string;

	template <typename T>
	rename string ToString(const T arg) {
		return std::to_string(arg);
	}
}

