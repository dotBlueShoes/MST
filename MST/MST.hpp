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

#include <thread>
#include <string>

#include "mst/basictypes.hpp"
#include "mst/narray.hpp"
#include "mst/array.hpp"

//#include <array>
//#include <type_traits>
//#include <cstdio>
//#include <iostream>

#pragma warning(disable:6386)

/* [ Types & Keywords ] */
namespace mst {

	namespace characters_n {
		// I need a way of getting digits in both decimal and heximal form
		//  as an character.
		// Not Getting but a structure that will allow me to increamenet and decrement them.

		// Whenever i will need a new digit i will need another char to represent that number.
		//  For decimal - past 9
		//  For hex		- past F

		// Not for constants tho.
		template<typename T>
		struct decimal {
			T* characters = nullptr;
			size length = 0;

			decimal(const std::initializer_list<T> list) {
				characters = new T[list.size()];

				for (auto& character : list)
					characters[length++] = character;
			}

			// Buffor Assignment Constructor.
			//  Have in mind that such construction will call this type destructor
			//  making the value buffor is pointing to unaccesible after this use.
			//  thus it's recommended to use something like std::array as a pointer.
			//decimal(T* const arrayPointer, const size nLength) {
			//	characters = arrayPointer;
			//	length = nLength;
			//}

			// Copy Constructor that looks for termination character.
			decimal(const T* const arr, const T& sTerminator) {
				size nLength = 0; while (arr[nLength] != sTerminator) ++nLength;

				characters = new T[++nLength];

				for (; length < nLength; length++)
					characters[length] = arr[length];
			}

			//Decimal(const Decimal& other) {
			//	length = 0; characters = new T[other.length];
			//
			//	for (; length < other.length; length++)
			//		characters[length] = other.characters[length];
			//}

			~decimal() { delete[] characters; }

			// This is copy. As said it requires characters to exist.
			decimal& operator=(const std::initializer_list<T> other) {
				delete[] characters;

				characters = new T[other.size()];
				length = 0;

				for (auto& character : other)
					characters[length++] = character;

				return *this;
			}

			decimal& operator=(const decimal& other) {

				delete[] characters;
				characters = new T[other.length]; length = 0;

				for (; length < other.length; length++)
					characters[length] = other.characters[length];

				return *this;
			}		

			// This is assign.
			//Decimal& operator=(const Decimal& other) {
			//	delete[] characters;
			//	chatacters = other->characters;
			//	length = other->length;
			//	return *this;
			//}

			// Prefix increment.
			decimal& operator++() {

				// First we're checking whether the first decimal is a 9 or not.
				//  If it is then we check wheter the string is in need of resizing like changing from the (99) to (100).
				//   If it is then we resize, set it's new value and return.
				//   If it isn't then we check what range of digits do we increment. (range eg. 1(899)\o or 10(89)\o ).
				//  If it's not then we simply increment the value of the char so by it's implementation it increaments as a number would.

				if (characters[length - 2] == '9') {				// length - 2 because: 1. arrays start at 0 and we count the /0 that is the last character.

					bool isContainingOnly9s = true;
					for (size i = 0; i < length - 1; i++)			// Remember that the first element is the highest digit.
						if (characters[i] != '9') {
							isContainingOnly9s = false;
							break;
						}

					if (isContainingOnly9s) {
						size newLength = length + 1;				// Each time we increase our buffor by 1.
						T* newArray = new T[newLength];

						newArray[0] = '1';
						for (size i = 1; i < newLength - 1; i++) {	// length - 1; We don't want the '\0' symbol.
							newArray[i] = '0';
						} newArray[newLength - 1] = characters[length - 1];

						delete[] characters;						// Althrough we could earlier we want characters always to refer to something.
						characters = newArray;
						length = newLength;

						//return *this;
					} else {

						//  So we look at the smallest digit thats value of 9 which always is in fact the fisrt digit...
						//  then we look at index of next non 9-value digit.
						//  we increment the value at that index by one and set all digits before to 0.
						//   We don't need to look at the first digit.

						size non9ValueIndex = length - 2;

						for (size i = non9ValueIndex - 1; characters[i] == '9'; i--)
							non9ValueIndex = i;

						++characters[non9ValueIndex - 1];
						for (size i = non9ValueIndex; i < length - 1; i++)
							characters[i] = '0';
					}

				} else ++characters[length - 2];

				return *this;
			}

			// Postfix increment.
			//Decimal operator++(int) {
			//	Decimal copy = *this;
			//	operator++();
			//	return copy;
			//}

		};

		template<typename T>
		struct Hexadecimal {
			T* characters = nullptr;
			size length = 0;

			// Maybe i should have a pointer to a data that would be a bool<8-64>
			//  that would say whether to use lowercase or uppercase??? hmmm how would that reflect on the code.
			//  maybe just a seperate type tho.

			Hexadecimal(std::initializer_list<T> list) {
				characters = new T[list.size()];
				length = 0;

				for (auto& character : list) {
					characters[length++] = character;
				}
			}

			~Hexadecimal() {
				delete[] characters;
			}

			// So we're doing hexadecimals now..
			//  this means that not only we have numbers that range from 48 to57
			//  but we also have litters of range (65-70) upper-case
			//   and litters of lower-case (97-102)

			Hexadecimal& operator++() {
				if (characters[length - 2] == 'F') {

					bool isContainingOnlyFs = true;
					for (size i = 0; i < length - 1; i++)
						if (characters[i] != 'F') {
							isContainingOnlyFs = false;
							break;
						}

					if (isContainingOnlyFs) {
						size newLength = length + 1;
						T* newArray = new T[newLength];

						newArray[0] = '1';
						for (size i = 1; i < newLength - 1; i++) {
							newArray[i] = '0';
						} newArray[newLength - 1] = '\0';

						delete[] characters;
						characters = newArray;
						length = newLength;
					} else {
						size non9ValueIndex = length - 2;

						for (size i = non9ValueIndex - 1; characters[i] == 'F'; i--)
							non9ValueIndex = i;

						// Here in comparision to decimal we need to check whether we're incrementing to an A or not.
						characters[non9ValueIndex - 1] == '9' ? 
							characters[non9ValueIndex - 1] == 'A' : 
							++characters[non9ValueIndex - 1];

						for (size i = non9ValueIndex; i < length - 1; i++)
							characters[i] = '0';
					}

				} else if (characters[length - 2] == '9') {

					// Here we're looking for a range of both '9's and 'F's but we need to dermine which one is it
					//  to properly apply the incrementation.

					size non9ValueIndex = length - 2;

					for (size i = non9ValueIndex - 1; uint8 ifMap = characters[i] == '9' || characters[i] == 'F'; i--)
						non9ValueIndex = i;

					//size non9ValueIndex = length - 2;

				} else ++characters[length - 2];

				return *this;
			}
		};

	}

	namespace string_n {
		using string = std::string;

		template <typename T>
		rename string ToString(const T arg) {
			return std::to_string(arg);
		}
	}

	namespace thread_n {
		using thread = std::thread;
	}

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
		size sizeofSample() {
			int var = 1;
			return sizeof real32;
		}

		struct Sample {
			uint32 a, b;
			real32 c;
		};

		uint32 SampleFunc() {
			Sample sample{};
			return 0;
		}
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