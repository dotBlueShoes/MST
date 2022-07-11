#pragma once

#include <cstdint>

namespace mst {
	/* Notes
	 *  Use this keywords in future [[deprecated]], noexcept
	 *  I don't get it. tf does a const keyword after func declaration.
	 *  when using vector use eplace_back insted push_back when creating an object. when moving an object use push_back.
	*/

#define forever while(true) // Infinite Loop 

	// force-inline is a msvs thing that might force a thing a little.
	// * Pretty much just for accesing a private memeber.
#define getter [[nodiscard]] __forceinline constexpr 
#define proxyf __forceinline constexpr

#ifdef deprecated
#define deprecated [[deprecated]]
#endif

// functions that exist to return a value, but aren't a getter. 
#define retriver [[nodiscard]]

#define block __forceinline void
#define rename __forceinline

/* Intieger Unsigned */
	using uint64 = uint64_t;		// Unsigned Intiger 64bit.
	using uint32 = uint32_t;		// Unsigned Intiger 32bit.
	using uint16 = uint16_t;		// Unsigned Intiger 16bit.
	using uint8 = uint8_t;			// Unsigned Intiger 8bit.

	/* Intieger Signed */
	using int64 = int64_t;			// Signed Intiger 64bit.
	using int32 = int32_t;			// Signed Intiger 32bit.
	using int16 = int16_t;			// Signed Intiger 16bit.
	using int8 = int8_t;			// Signed Intiger 8bit.

	/* Reals */
	//using real128 = ;
	using real64 = double;			// Floating Point 64bit.
	using real32 = float;			// Floating Point 32bit.

	/* Size */
	using size = size_t;			// https://en.cppreference.com/w/c/types/size_t

	/* Characters are a bit complicated because different Os's use different rules. */

	/* Characters Unsigned */
	using uchar32 = char32_t;		// Unsigned Character 32bit.
	using uchar16 = char16_t;		// Unsigned Character 16bit.
	using uchar8 = unsigned char;	// Unsigned Character 8bit.

	/* Characters Signed */
	using char8 = signed char;		// Signed Character 8bit.

	/* Unicode */					// Aperantly atleast in vs char != signed char as for now and soon char might get replaced with char8_t.
	using utf_8 = char;				// Unicode encoding for UTF-8 character. 
	using utf_16le = wchar_t;		// Unicode encoding for UTF-8 Little Endian character. http://www.herongyang.com/Unicode/UTF-16-UTF-16LE-Encoding.html


	/* C-array types */

	/* Intieger Unsigned */
	using uint64s = uint64[];
	using uint32s = uint32[];
	using uint16s = uint16[];
	using uint8s = uint8[];

	/* Intieger Signed */
	using int64s = int64[];
	using int32s = int32[];
	using int16s = int16[];
	using int8s = int8[];

	/* Reals */
	using real64s = real64[];
	using real32s = real32[];

	/* Size */
	using sizes = size[];

	/* Characters Unsigned */
	using uchar32s = uchar32[];
	using uchar16s = uchar16[];
	using uchar8s = uchar8[];

	/* Characters Signed */
	using char8s = char8[];

	/* Unicode */
	using utf_8s = utf_8[];
	using utf_16les = utf_16le[];

	/* Intiger registry datatype */
	/*
	union union8 {
		uint8 value;
		bool a, b, c, d, e, f, g, d;
	};

	union union16 {
		uint16 value;
		union8 low;
		union8 high;
	};

	union union32 {
		uint32 value;
		union16 low;
		union16 high;
	};

	union union64 {
		uint64 value;
		union32 low;
		union32 high;
	};
	*/
}