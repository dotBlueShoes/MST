#pragma once

#include "BasicTypes.hpp"

#undef rename
#include <string>
#undef rename
#define rename __forceinline


using string = std::string;

template <typename T>
rename string ToString(const T arg) {
	return std::to_string(arg);
}

