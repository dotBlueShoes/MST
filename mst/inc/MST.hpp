// Matthew's Strumillo Types

// ToDo:
// 1. Discrard array class to brand new file cpp and hpp.
// 2. ogarnij gdzie jest plik z linkiem do tych masm commend z bit operacjami
// 3. rename to msw Matthews Strumi³³o Workfied.
// 4. See if i can overlap c pointers into some sort of template "pointer<int32> a"

// Making an a library.
//  I am not a fan of an idea that certain function, class
//  has to think more about what value is being passed to it.
//  The class, function should't try to resolve whatever someone have passed.
//   handling exception is what i mean.
//  Instead what i find way more performent is to use an another library that
//  inlines the code in it and checks if the passed value is correct before 
//  sending it to the original function.
// functions like this could be named SafeFunc(), or Safe::Func();

// Things:
// #define produces error see why. https://aticleworld.com/typedef-vs-define-in-c/
// so i changed them to using instead.

// Yeah those types might produce some weird effects and errors.
// I might have also tried to outsmart something thats definitely smarter then me. oh well.
// https://stackoverflow.com/questions/28171556/what-are-the-minimum-width-integer-types-useful-for
// https://stackoverflow.com/questions/16164025/difference-between-int32-t-and-int-fast32-t

// pmr - memmory thingy to use with my bool64 type
// https://www.youtube.com/results?search_query=cpp+pmr
// cpp benchmark
// https://www.quick-bench.com

// ratio type, logic
// https://www.youtube.com/watch?v=BdHFLfv-ThQ (7:36)

// other
// https://en.cppreference.com/w/cpp/language/value_initialization
// https://stackoverflow.com/questions/39429510/what-do-empty-braces-mean-in-struct-declaration
// https://en.cppreference.com/w/cpp/language/default_constructor
// https://docs.microsoft.com/en-us/cpp/cpp/constructors-cpp?view=msvc-160

#pragma once

#include <stdint.h>

#include "MST/CharacterTypes.hpp"
#include "MST/BasicTypes.hpp"
#include "MST/Pointer.hpp"
#include "MST/String.hpp"
#include "MST/Thread.hpp"
#include "MST/Vector.hpp"
#include "MST/NArray.hpp"
#include "MST/Array.hpp"
#include "MST/Debug.hpp"

#pragma warning(disable:6386)

/* [ Types & Keywords ] */
namespace mst {

	//namespace container_n {
	//	// An object that instance can have vary length.
	//	template <class T>
	//	union container {
	//		T stream; // All defined variables in a form of a single varibale.
	//		const size length = 0; //{*T} ; The size of the stream.
	//		uint8 variable1;
	//		uint8 variable2;
	//	};
	//}

	/* Vectors */
	//template <typename T> struct Vector2 { T x, y; };
	//template <typename T> struct Vector3 { T x, y, z; };
	//template <typename T> struct Vector4 { T x, y, z, w; };

	/* Maps */
	//template <typename AT, typename BT> struct Map2 { AT x; BT y; };
	//template <typename AT, typename BT, typename CT> struct Map3 { AT x; BT y; CT z; };
	//template <typename AT, typename BT, typename CT, typename DT> struct Map4 { AT x; BT y; CT z; DT w; };

	// ?? weird thing that i am gonna delete.
	//namespace Uint64 {
	//	inline string::string ToString(const uint64 uintiger) { return std::to_string(uintiger); }
	//	//inline real64 ToReal64(uint64 uintiger) { return (real64)uintiger; }
	//}
	
	enum SystemVersion : uint64 {
		Windows10 = 0,
	};

	// something
	namespace utility_n {
		template <typename T>
		class DiscardArray {

			struct Element {
				//Element* parent; logika na cofanie siê w dialogu.
				size_t discardLength = 1;
				T value;

			} *elements = nullptr;

			Element* first = nullptr;

		public:
			size length = 0;

			DiscardArray(const size& newLength) {
				elements = new Element[newLength];
				first = &elements[newLength - 1];
			}

			//const Element* begin() const {
			//	return &elements[0];
			//}

			//const Element* end() const {
			//	return &elements[count];
			//}

			// Destructor...
			void Dispose() {
				delete elements;
			}

			void Add(const T& newValue, const size& newDiscardLength = 1) {
				elements[length].discardLength = newDiscardLength;
				elements[length].value = newValue;
				length++;
			}

			T Retrive(const size pointerPosition = 0) {
				length -= (first->discardLength + pointerPosition);
				first = &elements[length];

				return elements[length].value;
			}
		};
	}

	namespace math_n {

		// Consider using Pi() function instead.
		template <typename T> constexpr T pi = 3.14159265358979;

		// Returns compile time calculated pi constant.
		// It would be great to implement flag like logic and parameters after acording to the settled flat.
		//  - multiplier shouldn't be 0!
		//  - divider shouldn't be 0!
		template <typename T>
		constexpr T Pi(const T& multiplier = 1, const T& divider = 1) {
			return (multiplier * pi<T>) / divider;
		}

	}

	// see what alignas and alignof does
	namespace samples {
		//size sizeofSample() {
		//	int var = 1;
		//	return sizeof(real32);
		//}

		//struct Sample {
		//	uint32 a, b;
		//	real32 c;
		//};

		//uint32 SampleFunc() {
		//	Sample sample{};
		//	return 0;
		//}
	}

	namespace flag_n {

		/* Bool[] */ // try using unions!

		using bool64 = uint64;
		using mask64 = bool64;

		using bool32 = uint32;
		using mask32 = bool32;

		using bool16 = uint16;
		using mask16 = bool16;

		using bool8 = uint8;
		using mask8 = bool8;



		// Soon.
		//int boolTest();
	}

}

// Other things
// https://stackoverflow.com/questions/94794/what-is-the-cost-of-a-function-call - the ms seconds of "calling" a function.
// https://stackoverflow.com/questions/21028773/free-function-versus-member-function - "free function" vs "member function".