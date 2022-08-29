#pragma once

#include "BasicTypes.hpp"

#undef rename
#include <thread>
#undef rename
#define rename __forceinline

using thread = std::thread;