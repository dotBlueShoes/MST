A diffrent approach to pointers...

# Array
This type represents an array of elements that once defined we're unable to change it's **length** and unable to change where the **pointer** points to.

## Constructors
...
### Copy
...
### Move
...

## Const
Making this datatype constant would disallow the programmer to change the **values** that are being pointed to by the pointer.

```
// Things that will bring me closer to the right implementation...
// https://stackoverflow.com/questions/2785612/c-what-does-the-colon-after-a-constructor-mean
// https://stackoverflow.com/questions/39792417/what-does-this-three-dots-means-in-c
// https://stackoverflow.com/questions/60208933/c-using-parameter-pack-in-constructors
// https://stackoverflow.com/questions/37964251/initializing-a-const-array-in-the-constructor

// 2022-04-08
// https://en.cppreference.com/w/cpp/language/string_literal
// https://crascit.com/2015/03/21/practical-uses-for-variadic-templates/
// https://duckduckgo.com/?q=cpp+return+string+literal&t=vivaldi&ia=web
// https://en.cppreference.com/w/cpp/string/basic_string/operator%22%22s
// https://en.cppreference.com/w/cpp/language/constructor
// https://www.fluentcpp.com/2019/01/25/variadic-number-function-parameters-type/

//#undef rename
//#include <utility>
//#undef rename
//#undef array
//#define rename __forceinline

/*
*  Old Initialization List constructor.
*  What was the problem:
*  - It is known that arrays can be decayed to pointers however it's hard the other way around.
*  Thats why we changed the cArray parameter to Pointer taking parameter. Which thankfully we can convert back to an array.
*  Probablly all while still staing const and compile time only.
* 
*	template <size ... index> // ( e.g.: size index1 = 1, index2 = 2... ) 
*	array(const T(&cArray)[length], std::index_sequence<index...>) : value { cArray[index]... } {}
*	constexpr const array(const T(&cArray)[length]) : array(cArray, std::make_index_sequence<length>()) {}
* 
*/

// TODO
		// - Copy constructors
		// - EnforceSame among parameter pack params.
		// - Make Sure we stay in bounds of specified length.

		/*
		// Copy Constructor.
		//constexpr const array(const array& reference) : array(reference.value, std::make_index_sequence<length>()) {}
		//constexpr const array(const T* pointer) : array(pointer, std::make_index_sequence<length>()) {}

		// Initialization as List Constructor. ( e.g. uint64 { 1, 2, 3, 4, 5, 6 }; )
		//  ! Watch out for length. It might produce a wrong error saying this is all wrong.

		// Parameter Pack Constructor
		//template <class... Types>
		//constexpr array(Types... newValue) : value { newValue... } {}

		//constexpr array(const T* pointer) {}

		// Constructor for at least 2.
		//constexpr array(const T newFirstValue, const T newSecondValue) : value { newFirstValue, newSecondValue } {}

		// Constructor for 3 and more.
		//template <class TypeFirst, class... TypesRest>
		//constexpr array(TypeFirst newFirstValue, TypeFirst newSecondValue, TypesRest... newRestValue) : value { newFirstValue, newSecondValue, newRestValue... } {}

		//template <class TypeFirst, class... TypesRest>
		//struct EnforceSame {
		//	static_assert(conjunction_v<is_same<TypeFirst, TypesRest>...>, "Error Code. ");
		//	using type = T;
		//};

		//template <class TypeFirst, class... TypesRest> 
		//constexpr array<class EnforceSame(TypeFirst, TypesRest...)::type, sizeof ...(TypesRest) + 1>
		//	(TypeFirst newFirstValue, TypesRest... newRestValue) : value {newFirstValue, newRestValue...} {}

		//array()

		//constexpr const array() = default;

		//template <class TypeFirst, class... TypesRest>
		//constexpr const array(T value, T... values)->
		//	array<class EnforceSame<T, T...>::type, sizeof...(TypesRest) + sizeof TypeFirst>(&value);

		//template <class TypeFirst, class... TypesRest>
		//constexpr const array(const TypeFirst newValue, const TypesRest... newValues)->
		//	array<class EnforceSame<TypeFirst, TypesRest...>::type, sizeof ...(TypesRest) + sizeof TypeFirst>() : value {(T)newValue} {};


		//template <class TypeFirst, class... TypesRest>
		//constexpr const array(const TypeFirst newValue, const TypesRest... newValues)->
		//	array<class EnforceSame<TypeFirst, TypesRest...>::type, sizeof...(TypesRest) + sizeof TypeFirst>(TypeFirst newValues...) : value { (T)newValue } {};

		//constexpr array(const array<T, length>& copy) : array(copy.Pointer(), std::make_index_sequence<length>()) {}
		// Does not work because it's an array that has to be known at compile time!
		// constexpr array(const array& reference) { value = reference.value; }
		// constexpr array(const array(&reference)) : array(reference.value, std::make_index_sequence<length>()) {} // WTF??
		*/

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
	
//template <class _First, class... _Rest>
//struct _Enforce_same {
//	static_assert(conjunction_v<is_same<_First, _Rest>...>,
//		"N4687 26.3.7.2 [array.cons]/2: "
//		"Requires: (is_same_v<T, U> && ...) is true. Otherwise the program is ill-formed.");
//	using type = _First;
//};
//
//template <class _First, class... _Rest>
//array(_First, _Rest...)->array<typename _Enforce_same<_First, _Rest...>::type, 1 + sizeof...(_Rest)>;


//template <class _Fn, class... _Args, enable_if_t<!is_same_v<_Remove_cvref_t<_Fn>, thread>, int> = 0>
//_NODISCARD_CTOR explicit thread(_Fn&& _Fx, _Args&&... _Ax) {
//	_Start(_STD forward<_Fn>(_Fx), _STD forward<_Args>(_Ax)...);
//}
```

# NArray
Using this type we're able to define an array of elements where **pointer** is being set at construction. The **length** is an incrementing value. That grows with each array assignment.

## Unsafe
This relation gives us the option to define the array with a size that is not being stored originally in it. Making the **length** variable only an information about how many of the values in that array were initialized. The system however is unsafe as it allows referencing memory out of scope. 

## Const
When NArray is being set as a const then we mean that it's **length** is unchanable and that the **pointer** cannot point somewhere else. Making the data the pointer points to still changeable.