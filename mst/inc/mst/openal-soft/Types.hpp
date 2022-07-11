#pragma once
#include <AL\al.h>

namespace mst {
	namespace openal {

		using bool8 = ALboolean;	// 8-bit boolean (std::int8_t)
		using char8 = ALchar;		// character (char)

		using enum32 = ALenum;		// enumerated 32-bit value (std::uint32_t)

		using int8 = ALbyte;		// signed 8-bit integer (std::int8_t)
		using uint8 = ALubyte;		// unsigned 8-bit integer (std::uint_8)
		
		using int16 = ALshort;		// signed 16-bit integer (std::int16_t)
		using uint16 = ALushort;	// unsigned 16-bit integer (std::uint16_t)

		using int32 = ALint;		// signed 32-bit integer (std::int32_t)
		using uint32 = ALuint;		// unsigned 32-bit integer (std::uint32_t)

		using size = ALsizei;		// non-negative 32-bit binary integer size (std::uint32_t)

		using real32 = ALfloat;		// 32-bit floating-point (float)
		using real64 = ALdouble;	// 64-bit floating-pint (double)

		using void0 = ALvoid;		// void value (void)
	}
}
