#pragma once
#include "BasicTypes.hpp"

namespace mst {
	
	/// FORWARD DECLARATION
	template <class T>
	struct vector3;
	
	template <class T>
	struct vector2 {
		T x, y;
		
		constexpr vector2(const T& newX, const T& newY) : x(newX), y(newY) {}
		constexpr vector2(const vector3<T>& other) : x(other.x), y(other.y) {}
		constexpr vector2(const vector2& other) : x(other.x), y(other.y) {}
	};

	template <class T>
	struct vector3 {
		T x, y, z;
		
		constexpr vector3(const T& newX, const T& newY, const T& newZ) : x(newX), y(newY), z(newZ) {}
		constexpr vector3(const vector3& other) : x(other.x), y(other.y), z(other.z) {}
		constexpr vector3(const vector2<T>& other) : x(other.x), y(other.y), z(0) {}
	};
	
}

// template <class T>
// struct rect {
// 	T a, b, c, d;
// }