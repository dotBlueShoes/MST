#pragma once

#include "basictypes.hpp"

#undef rename
#include <utility>
#undef rename
#define rename __forceinline

//#undef rename
//#include <utility>
//#undef rename
//#undef array
//#define rename __forceinline

// This is being copied from the microsoft array definition.
//  what i do is simply use the same but have uppercase function letters.

// Things that will bring me closer to the right implementation...
// https://stackoverflow.com/questions/2785612/c-what-does-the-colon-after-a-constructor-mean
// https://stackoverflow.com/questions/39792417/what-does-this-three-dots-means-in-c
// https://stackoverflow.com/questions/60208933/c-using-parameter-pack-in-constructors
// https://stackoverflow.com/questions/37964251/initializing-a-const-array-in-the-constructor

namespace mst::array_n {

	template <class T, size length>
	class array {

		const size nonTemporaryLength { length };
		T value[length];

		// Forwarded Constructor. used by "Initialization via Forwarding constructor" constructor.
		template <size ... index> // ( e.g.: size index1 = 1, index2 = 2... ) 
		array(const T(&valuePointer)[length], std::index_sequence<index...>) : value { valuePointer[index]... } {}

	public:

		// Constructors

		// Initialization via Forwarding constructor. ( e.g.: wchar* sample { "Sample" }; )
		array(const T(&valuePointer)[length]) : array(valuePointer, std::make_index_sequence<length>()) {}
		// Initialization as List Constructor. ( e.g. uint64 { 1, 2, 3, 4, 5, 6 }; )
		//  ! Watch out for length. It might produce a wrong error saying this is all wrong.
		template <typename... Types> constexpr array(Types... newValue) : value { newValue... } {}

		/* Missing Constructors
		//template<size L1, size L2>
		//constexpr array(const array<T, L1>& arr1, const array<T, L2>& arr2) : value { *arr1.Pointer() } {}
		// How to make a const array construct from 2 or more different length same type const arrays???
		// Funny is that there actually might be something using the index-sequence.
		// 
		// https://stackoverflow.com/questions/37964251/initializing-a-const-array-in-the-constructor
		// https://stackoverflow.com/questions/41660062/how-to-construct-an-stdarray-with-index-sequence
		// https://en.cppreference.com/w/cpp/language/parameter_pack
		// https://riptutorial.com/cplusplus/example/3208/iterating-over-a-parameter-pack
		//template <size len1, array<T, len1> T1, size len2, array<T, len2> T2, size len3, array<T, len3> T3>
		//constexpr array(T1, T2, T3) : value {}
		//template <size... lengths, array<T, lengths...>... Types>
		//array(Types... newTypes)){
		//}
		//template <class T1, size length1, class T2, size length2>
		//array(array<T1, length1> val1, array<T2, length2> val2) {
		//	size i = 0;
		//	for (; i < length1; i++)
		//		value[i] = val1[i];
		//	for (size j = 0; j < length2; j++)
		//		value[i + j] = val2[j];
		//}
		//template <typename... Types>
		//constexpr array(array<Types>)
		*/

		// [] operator

		getter const T& operator[](size index) const noexcept { return value[index]; }
		getter T& operator[](size index) noexcept { return value[index]; }

		// Iteration

		getter const T* begin() const noexcept { return &value[0]; }
		getter const T* end() const noexcept { return &value[nonTemporaryLength]; }

		// Getters

		getter const size& Length() const noexcept { return nonTemporaryLength; }
		getter const T* const Pointer() const noexcept { return &value[0]; }
		getter T* const Pointer() noexcept { return &value[0]; }

	};

}