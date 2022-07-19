#pragma once
#include "../../CharacterTypes.hpp"

namespace mst::winapi::console::palette {
	
	using namespace characters_n;
	
	using bufforType = wchar;
	using buffor = bufforType*;
	
	class color { 
	public: 
		decimal<bufforType> red {}, green {}, blue {}; 
	} colorBlack { { L'0', L';' }, { L'0', L';' }, { L'0', L'm' } };
	
	
	
	template<typename T> 
	class characterType {
	public:
		T* characters = nullptr;
		size length = 0;
		characterType(const std::initializer_list<T> list) {
			characters = new T[list.size()];
			for (auto& character : list)
				characters[length++] = character;
		}
		//~characterType() { ;delete[] characters; }
	}; const characterType<bufforType> sampleA { L'0', L';' };
	
	class colorr {
	public: 
		characterType<bufforType> red { L'0', L';' }, green { L'0', L';' }, blue { L'0', L'm' }; 
	} const colorrBlack { { L'0', L';' }, { L'0', L';' }, { L'0', L'm' } };
	
	class brush {
	public:
		bufforType character {};
		colorr backgroundColor {}, characterColor {}; // This line breaks.
	};
}