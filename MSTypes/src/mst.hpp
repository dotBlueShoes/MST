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

#include <cstdint>
#include <string>

/* [ Types & Keywords ] */
namespace mst {

	#define forever while(true) // Infinite Loop 

	#define getter [[nodiscard]] inline constexpr
	#define block inline void
	#define rename inline

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

	/* Characters Unsigned */
	using uchar32 = char32_t;		// Unsigned Character 32bit.
	using uchar16 = char16_t;		// Unsigned Character 16bit.
	using uchar8 = unsigned char;	// Unsigned Character 8bit.

	/* Characters Signed */
	using char8 = char;				// Signed Character 8bit.

	/* Reals */
	using real64 = double;			// Floating Point 64bit.
	using real32 = float;			// Floating Point 32bit.

	namespace string {

		using std::string;			// String Type.

		template <typename T>
		rename string toString(const T arg) {
			return std::to_string(arg);
		}
	}

	namespace nativeArray {

		// ConstNativeArray
		//  This is a i hope 0 cost container for
		//  array types that requires having both
		//  array and it's length known.
		// It is a const variant because we're not
		//  doing any dynamic allocation with it.
		template<typename T>
		class NativeArrayConst {
			const T* data = nullptr;
			size_t length = 0;
		public:
			constexpr NativeArrayConst(const std::initializer_list<T> list) {
				length = list.size();
				data = list.begin();
			}

			/* Iteration */
			getter const T* begin() const noexcept { return &data[0]; }
			getter const T* end() const noexcept { return &data[length]; }

			/* Getters */
			getter const size_t Length() { return length; }
			getter const T* Data() { return data; }

		};

		template <typename T>
		struct NativeArray {
		private:
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
			constexpr NativeArray(std::initializer_list<T> list) {
				length = list.size();
				data = new T[length];
				for (size_t i = 0; i < length; i++)
					data[i] = *(list.begin() + i);
			}

			~NativeArray() {
				delete[] data;
			}

			_NODISCARD  constexpr T* begin() noexcept {
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

			inline size_t Length() { return length; }
			inline T* Data() { return data; }

		};
	}
	
	

	/* NativeArray */
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

	template <typename T>
	struct array {
		T* pointer = nullptr;
		size_t length = 0;
	};
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
	namespace utility {
		template <typename T>
		class DiscardArray {

			struct Element {
				//Element* parent; logika na cofanie siê w dialogu.
				size_t discardLength = 1;
				T value;

			} *elements = nullptr;

			Element* first = nullptr;

		public:
			size_t length = 0;

			DiscardArray(const size_t& newLength) {
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

			void Add(const T& newValue, const size_t& newDiscardLength = 1) {
				elements[length].discardLength = newDiscardLength;
				elements[length].value = newValue;
				length++;
			}

			T Retrive(const size_t pointerPosition = 0) {
				length -= (first->discardLength + pointerPosition);
				first = &elements[length];

				return elements[length].value;
			}
		};
	}

	namespace math {

		// Consider using Pi() function instead.
		template <typename T> constexpr T pi_v = 3.14159265358979;

		// Returns compile time calculated pi constant.
		// It would be great to implement flag like logic and parameters after acording to the settled flat.
		//  - multiplier shouldn't be 0!
		//  - divider shouldn't be 0!
		template <typename T>
		constexpr T Pi(const T& multiplier = 1, const T& divider = 1) {
			return multiplier * pi_v<T> / divider;
		}

	}

	// see what alignas and alignof does
	namespace samples {
		size_t sizeofSample() {
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

	namespace Bool64 {

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