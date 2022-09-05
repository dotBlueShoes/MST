#pragma once
#include "../../CharacterTypes.hpp"
#include <iostream>

namespace mst::winapi::console::palette {
	
	using namespace characters_n;
	
	using bufforType = wchar;
	using buffor = bufforType*;
	
	class color { 
	public: 
		decimal<bufforType> red, green, blue; 
		// Empty Constructor. Value is not being initialized. ( e.g. colour (); )
		constexpr color() { /* std::cerr << "b\a"; */ }
		
		color(decimal<bufforType> nRed, decimal<bufforType> nGreen, decimal<bufforType> nBlue) :
			red(nRed), green(nGreen), blue(nBlue) { /* std::cerr << "2\a"; */ };
			
	} const colorBlack ( { L'0', L';' }, { L'0', L';' }, { L'0', L'm' } );
	
	
	
	//template<typename T> 
	//class characterType {
	//public:
	//	T* characters = nullptr;
	//	size length = 0;
	//	
	//	characterType(const std::initializer_list<T> list) {
	//		/* std::cerr << "1\a"; */
	//		characters = new T[list.size()];
	//		for (auto& character : list)
	//			characters[length++] = character;
	//	}
	//	
	//	// Empty Constructor. Value is not being initialized. ( e.g. characterType (); )
	//	constexpr characterType() { /* std::cerr << "a\a"; */ }
	//	
	//	~characterType() { /* std::cerr << '\a'; */ };
	//	//~characterType() { ;delete[] characters; }
	//};
	
	//class colour {
	//public: 
	//	//characterType<bufforType> red { L'0', L';' }, green { L'0', L';' }, blue { L'0', L'm' }; 
	//	
	//	characterType<bufforType> red, green, blue; 
	//	
	//	// Empty Constructor. Value is not being initialized. ( e.g. colour (); )
	//	constexpr colour() { /* std::cerr << "b\a"; */ }
	//	
	//	colour(characterType<bufforType> nRed, characterType<bufforType> nGreen, characterType<bufforType> nBlue) :
	//		red(nRed), green(nGreen), blue(nBlue) { /* std::cerr << "2\a"; */ };
	//	
	//	~colour() { /* std::cerr << '\a'; */ };
	//} const colourBlack ( { L'0', L';' }, { L'0', L';' }, { L'0', L'm' } ); /*, colourWhite ( { L'5', L';' }, { L'5', L';' }, { L'5', L'm' } ) ; */
	
	class brush {
	public:
		bufforType character;
		color backgroundColor, textColor; // This line breaks.
		
		// Empty Constructor. Value is not being initialized. ( e.g. brush (); )
		constexpr brush() { /* std::cerr << "c\a"; */ }
		//brush(const brush &other);
		
		brush(bufforType character, color backgroundColor, color textColor) :
			character(character), backgroundColor(backgroundColor), textColor(textColor) { /* std::cerr << "3\a"; */ };
		
		~brush() { /* std::cerr << '\a'; */ };
	} const brushEmpty ( L'\0', colorBlack, colorBlack );
}

//2 + 3*3
// 36 960
//110 880 bytes ! 

// 1. I am not doing anything about that NEW statement and i am like requesting 0.1 MB with each compilation.
// 2. I should definetly use brushReference Type. Have pointers that point at certain brushes instead initializing all those brushes
//  just to replace them with any function that draws.