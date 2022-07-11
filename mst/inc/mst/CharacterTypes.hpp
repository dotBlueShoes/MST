#pragma once

#include "basictypes.hpp"
#include <initializer_list>
#include "winapi/types.hpp"

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

		// Known Termination. Use only if you know the length.
		constexpr const mst::winapi::wchar* operator "" _nt(const wchar_t* string, const size length) { 
			return string; 
		};

		//constexpr std::wstring operator ""_nt(const mst::winapi::wchar * string, size length) {
		//	return std::wstring(string);
		//};

		//constexpr const std::string operator ""_nt(const char* string, size length) {
		//	return std::string(string);
		//};

		//const char[] operator ""_nt(const char* string, size length) {
		//	char* sample = "wonet return me";
		//	return sample;
		//};

		
		//template <char...> 
		//char* operator "" _nt();

		//constexpr const std::string operator ""_nt(const char(&notAPointer)[length]) {
		//	return std::string(string);
		//};
		


		//array& operator ""_nt(const winapi::wchar * string, size length) { return winapi::wchar(&string); };
		//const T(&valuePointer)[length]
		//winapi::wchar[7] operator ""_nt(const winapi::wchar * string, size length) { return string; };
		//void operator ""_test(unsigned long long x) { std::cout << x; } // working

		// ERROR / Prob. - String termination. ( I don't want all string's to be not null terminated. )
		// Maybe these might help next time.
		//  generally the thing is:
		//   I was trying to use literals to solve the issue of terminated strings eg. "Troghol" - being 8 characters long due to termination
		//  This might be solvable it's just complicated due to CPP not friendly syntax.
		//  The issue i run at last was the fact that this constructor
		//  "constexpr const array(const T* reference) : array(reference, std::make_index_sequence<length>()) {}"
		//  is being called for both "_nt" and "initializer list" parameters.
		//   Still it came a long path... 
		//  Maybe it is actually possible to return index_sequance instead
		//  Maybe i can somehow diffrensiate initializer list from single "_nt"
		//  AND MAYBE BECAUSE I DON"T KNOW the "_nt" constructor works as it is written right now...
		// https://www.google.com/search?q=Check+if+parameter+pack+is+the+same+type&sxsrf=APq-WBsm_IlVk2sZs6uYt3LN_29aSOX3BA%3A1648989813206&source=hp&ei=dZZJYrTbCs6RlwSn1bjoCw&iflsig=AHkkrS4AAAAAYkmkheWfRby88MP2Op6ArzfYGyXB6AWw&ved=0ahUKEwj06bXg9ff2AhXOyIUKHacqDr0Q4dUDCAc&uact=5&oq=Check+if+parameter+pack+is+the+same+type&gs_lcp=Cgdnd3Mtd2l6EAMyBQghEKABMgUIIRCgAVCUBFiUBGDZCmgBcAB4AIAByQGIAckBkgEDMi0xmAEAoAECoAEBsAEA&sclient=gws-wiz
		// https://www.google.com/search?q=return+initializer+list+c%2B%2B&biw=1885&bih=954&sxsrf=APq-WBsMIEno_19_j32NtU_vML_juzC5rw%3A1648990350021&ei=jphJYsl158iuBNShgpgH&oq=return+initializer+list&gs_lcp=Cgdnd3Mtd2l6EAMYATIGCAAQBxAeMgUIABCABDIECAAQHjIECAAQHjIICAAQBxAFEB4yCAgAEAcQBRAeMgYIABAIEB46BwgAEEcQsAM6BwgAELADEEM6BAgAEA06CAgAEAcQChAeOggIABAIEAcQHkoECEEYAEoECEYYAFCxEFjpGmC7N2gBcAF4AIABngGIAcIHkgEDMC43mAEAoAEByAEJwAEB&sclient=gws-wiz

	}
}