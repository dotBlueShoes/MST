#pragma once

#include "basictypes.hpp"

// This is being copied from the microsoft array definition.
//  what i do is simply use the same but have uppercase function letters.

// Things that will bring me closer to the right implementation...
// https://stackoverflow.com/questions/2785612/c-what-does-the-colon-after-a-constructor-mean
// https://stackoverflow.com/questions/39792417/what-does-this-three-dots-means-in-c
// https://stackoverflow.com/questions/60208933/c-using-parameter-pack-in-constructors

namespace mst::array_n {

	template <class T, size length>
	class array {

		const size nonTemporaryLength { length };
		T value[length];

	public:

		// Constructors

		template <typename... Types>
		constexpr array(Types... newValue) : value { newValue... } {}

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