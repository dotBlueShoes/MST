#pragma once

#include "basictypes.hpp"

// Array where both value and length can be changed.
//  uses new and delete underneath.

// CONST:
// When nArray is being assigned as a const 
//  then we mean that it's length is unchanable and
//  that the pointer cannot point somewhere else.
// HOWEVER the data the pointer points to can be changed.

namespace mst::nArray_n {

	// Usafe : because thers nothing that will prompt correct error if we pass through the previously set length.
	namespace unsafe {
		template <class T>
		class nArray {

			using nArrayPointer = const nArray<T>*;

			T* value = nullptr;
			size length = 0;

		public:

			// Constructors

			constexpr nArray(const std::initializer_list<T> list) {
				value = new T[list.size()];
				for (; length < list.size(); length++)
					value[length] = *(list.begin() + length);
			}

			constexpr nArray(T* const nPointer, const size& nLength) { value = nPointer; length = nLength; }
			constexpr nArray(const T& character) { value = new T[1]; value[0] = character; ++length; }
			constexpr nArray(const std::initializer_list<nArrayPointer> nArrays) {

				size nLength = 0; for (auto& nArr_pointer : nArrays) nLength += (*nArr_pointer).Length();
				value = new T[nLength];

				for (auto& nArr_pointer : nArrays)
					for (auto& elem : *nArr_pointer) {
						value[length] = elem;
						++length;
					}
			}

			// Destructors

			~nArray() { delete[] value; }

			// Iteration

			getter const T* begin() const noexcept { return &value[0]; }
			getter const T* end() const noexcept { return &value[length]; }

			// Getters

			getter const size& Length() const noexcept { return length; }
			getter const T* const Pointer() const noexcept { return &value[0]; }

			// UNSAFE
			constexpr nArray(const size& nSize) { value = new T[nSize]; }
			block SetCurrentElement(const T& nValue)	{ value[length] = nValue; ++length; }
			block SetElement(const T& value, const size& index)	{ value[index] = value; }

		};
	}

	inline namespace safe {
		template <class T>
		class nArray {

			using nArrayPointer = const nArray<T>*;

			T* value = nullptr;
			size length = 0;

		public:

			// Standard constructor.
			constexpr nArray(const std::initializer_list<T> list) {

				// We're basiclly allocating memory for new array,
				//  then we're adding up to the length and assigning
				//  the right value to the created array.

				value = new T[list.size()];
				for (; length < list.size(); length++)
					value[length] = *(list.begin() + length);
			}

			// Retriver constructor.
			constexpr nArray(T* const nPointer, const size& nLength) { value = nPointer; length = nLength; }

			// Single Value Constructor.
			// Can i optimize it??? like cheat it???
			//  I would have to make sure that the type takes less then pointer size.
			//  also i could use an union.
			constexpr nArray(const T& character) { value = new T[1]; value[0] = character; ++length; }
			
			constexpr nArray(const std::initializer_list<nArrayPointer> nArrays) {

				// This constructor is used to avoid temporary creation of nArrays inside initializer_list
				//  we avoid it by using const pointers. By doing so no 
				//  destructors or constructors of nArray are being called

				size nLength = 0; for (auto& nArr_pointer : nArrays) nLength += (*nArr_pointer).Length();
				value = new T[nLength];

				for (auto& nArr_pointer : nArrays)
					for (auto& elem : *nArr_pointer) {
						value[length] = elem;
						++length;
					}
			}

			// Destructors

			~nArray() { delete[] value; }

			// Iteration

			getter const T* begin() const noexcept { return &value[0]; }
			getter const T* end() const noexcept { return &value[length]; }

			// Getters

			getter const size& Length() const noexcept { return length; }
			getter const T* const Pointer() const noexcept { return &value[0]; }

		};
	}
}

/*
//constexpr nArray(const std::initializer_list<nArray> nArrays, const std::initializer_list<T> arrays) {}

//constexpr nArray(const nArray<T>& other) {
//	data = new T[other.length];
//	for (; length < other.length; length++)
//		data[length] = *(other.data + length);
//}

//constexpr nArray(const nArray<T*>& other) {
//	data = new T[other.length];
//	for (; length < other.length; length++)
//		data[length] = *(other.data + length);
//}

//constexpr nArray(const std::initializer_list<nArray<T>>& nArrays) {
//	std::cerr << "will it call it's destructor?\n";
//}
*/
/*
	namespace nativeArray {

		// Only use it in inheritence otherwise use CNativeArray.
		template<typename T>
		class ICNativeArray {
		protected:
			const T* data = nullptr;
			size_t length = 0;
		public:
			constexpr ICNativeArray(const std::initializer_list<T> list) {
				length = list.size();
				data = list.begin();
			}

			// Iteration

			getter const T* begin() const noexcept { return &data[0]; }
			getter const T* end() const noexcept { return &data[length]; }

			// Getters

			getter const size_t Length() const noexcept { return length; }
			getter const T* Data() const noexcept { return data; }

		};

		// 'CNativeArray' is a i hope 0 cost container for
		//  array types that requires having both
		//  array and it's length known.
		// It is a const variant because we're not
		//  doing any dynamic allocation with it.
		template<typename T> using CNativeArray = const ICNativeArray<T>;

		template <typename T>
		class NativeArray {
			T* data = nullptr;
			size_t length;
		public:
			//NativeArray(const size_t newLength, T* newData) {
			//	data = newData;
			//	length = newLength;
			//}

			//NativeArray(const size_t newLength, const T* newData) {
			//	data = newData;
			//	length = newLength;
			//}

			// Do i want a std::initializer_list in a non const object???
			constexpr NativeArray(const std::initializer_list<T> list) {
				length = list.size();
				data = new T[length];
				for (size_t i = 0; i < length; i++)
					data[i] = *(list.begin() + i);
			}

			constexpr NativeArray(const size& nLength) {
				data = new T[nLength];
				length = nLength;
			}

			NativeArray(const NativeArray* other) {
				//data = other.data;
				//length = other.length;
				length = 0; data = new T[other.length];

				for (; length < other.length; length++)
					data[length] = other.data[length];
			}

			//NativeArray(T* nData) { data = nData; }

			//void* operator new[](size nLength) {
			//	length = nLength;
			//}

			//void* operator new(size nLength) {

			//	std::cout << "Overloading new operator with size: " << nLength << std::endl;
			//	void* p = malloc(nLength);
			//	length = nLength;

			//	return p;
			//}

			~NativeArray() {
				std::cout << "lol";
				delete[] data;
			}

			_NODISCARD constexpr T* begin() noexcept {
				return &data[0];
			}

			_NODISCARD constexpr T* end() noexcept {
				return &data[length];
			}

			//T& operator()();

			//NativeArray(const T* newData, const size_t newLength) {
			//	data = newData;
			//	length = newLength;
			//}

			//NativeArray() : data{1, 2} {}
			//NativeArray(int a, int b) {}
			//NativeArray(const char* a) {}
			//NativeArray(size_t newLength,) : data{ 3, 4, 5 } {
			//	length = newLength;
			//}

			//T& operator=(const T& other) {
			//	//std::cout << "lol!";
			//}

			//template<typename U>
			//T& operator=(const NativeArray<U>) {};

			getter size_t Length() const noexcept { return length; }
			getter T* Data() const noexcept { return data; }

			//inline size_t Length() { return length; }
			//inline T* Data() { return data; }

		};

	}
	*/
/*
	//template <typename T>
	//struct NativeArray {
	//	const T* pointer = nullptr;
	//	size_t length = 0;

	//	NativeArray(const T* newPointer, const size_t newLength) {
	//		pointer = newPointer;
	//		length = newLength;
	//	}

	//	NativeArray(T* newPointer, size_t newLength, T newElement) {
	//		length = newLength + 1;
	//		pointer = new T[newLength];
	//		for (size_t i = 0; i < newLength; i++)
	//			pointer[i] = newPointer[i];
	//		pointer[length] = newElement;
	//	}

	//	NativeArray(
	//		const T* newFisrtsPointer,
	//		const size_t newFirstLength,
	//		const T* newSecondPointer,
	//		const size_t newSecondLength
	//	) {

	//	}

	//};

	// The casting prob.
	//  - I've got to learn how new casts work..
	// dynamic_cast
	// static_cast
	// const_cast
	// reinterpret_cast
	// static_pointer_cast
	// When converting between some pointer types, 
	//  it's possible that the specific memory address held in the pointer needs to change.
	// That's where the two casts differ. static_cast will make the appropriate adjustment. 
	//  reinterpret_cast will not.

	//template <typename T>
	//struct array {
	//	T* pointer = nullptr;
	//	size_t length = 0;
	//};
	//inline array<const char*> GetRequiredGlobalExtensions() {
	//	array<const char*> extensions;
	//	extensions.pointer = glfwGetRequiredInstanceExtensions(extensions.length);

	//	#ifdef VALIDATION_LAYERS_ENABLED

	//	const char** temp = new const char* [extensions.length + 1];
	//	for (size_t i = 0; i < extensions.length; i++)
	//		temp[i] = extensions.pointer[i];

	//	temp[extensions.length] = ValidationLayer::validationLayerGlobalExtension;
	//	extensions.pointer = temp;
	//	extensions.length += 1;

	//	#endif

	//	return extensions;
	//}
*/