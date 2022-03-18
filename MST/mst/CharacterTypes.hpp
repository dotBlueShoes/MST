#pragma once

#include "basictypes.hpp"
#include <initializer_list>

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
}