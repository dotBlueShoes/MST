#pragma once
#include "BasicTypes.hpp"

namespace mst::pointer_n {
	
	template <class T>
	class Pointer {
		T* value;
	public:
	
		// [] Operator

		getter const T& operator[](size index) const noexcept { return &value[index]; }
		getter T& operator[](size index) noexcept { return &value[index]; }
		
		// Getters
		
		getter const T Value() const noexcept { return &value[0]; }
	};
	
}