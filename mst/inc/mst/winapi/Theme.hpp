#pragma once
#include "DarkMode.hpp"

namespace mst::winapi::window::theme {
	
	template <class TypeFirst, class TypeOther>
	concept IsSameType = requires (const TypeFirst&, const TypeOther&) {
		std::same_as<TypeFirst, TypeOther>;
	};
	
	template <uint64 length = 6>
	class palette {
		
	public:
	
		template <class... ParameterPack>
		using FirstType = std::tuple_element_t<0, std::tuple<ParameterPack...>>;
	
		const array<const color, length> colors;
		
		const color& 
			text 		= colors[0],
			primar	 	= colors[1],
			second	 	= colors[2],
			selected 	= colors[3],
			hovered 	= colors[4],
			border 		= colors[5];
		
		// Parameter pack Constructor. ( e.g. palette { A, B }; )
		template <class... TypeRest> 
		requires IsSameType<color, FirstType<TypeRest...>>
		constexpr palette (const color& newFirstColor, const color& newSecondColor, const TypeRest&... newRestColors) 
			: colors { newFirstColor, newSecondColor, newRestColors... } {}
		
	};
	
	const palette<6> 
		light { 
			RGB(0xff, 0xff, 0xff),
			RGB(0xe5, 0xf3, 0xff),
			RGB(0xf0, 0xf0, 0xf0),
			RGB(0xff, 0xff, 0xff), 
			RGB(0xcc, 0xe8, 0xff), 
			RGB(0x00, 0x00, 0x20)
		}, dark {
			RGB(0xff, 0xff, 0xff),
			RGB(0x1d, 0x1d, 0x1d),
			RGB(0x00, 0x00, 0x00), 
			RGB(0x67, 0x67, 0x67), 
			RGB(0x4d, 0x4d, 0x4d), 
			RGB(0x1d, 0x1d, 0x1d)
		};

	// 1 struct theme {
	// 1 
	// 1 	const color
	// 1 		backgroundSecondary,
	// 1 		backgroundSelected,
	// 1 		backgroundHovered,
	// 1 		backgroundPrimary,
	// 1 		borderColor,
	// 1 		textPrimary;
	// 1 
	// 1 	theme(
	// 1 		const color newBackgroundSecondary,
	// 1 		const color newBackgroundSelected,
	// 1 		const color newBackgroundHovered,
	// 1 		const color newBackgroundPrimary,
	// 1 		const color newTextPrimary,
	// 1 		const color newBorderColor
	// 1 	) :
	// 1 		backgroundSecondary(newBackgroundSecondary),
	// 1 		backgroundSelected(newBackgroundSelected),
	// 1 		backgroundHovered(newBackgroundHovered),
	// 1 		backgroundPrimary(newBackgroundPrimary),
	// 1 		borderColor(newBorderColor),
	// 1 		textPrimary(newTextPrimary)
	// 1 	{}
	// 1 
	// 1 };
	// 1 
	// 1 const theme lightMode	(RGB(0xff, 0xff, 0xff), RGB(0xcc, 0xe8, 0xff), RGB(0xe5, 0xf3, 0xff), RGB(0xff, 0xff, 0xff), RGB(0x00, 0x00, 0x20), RGB(0xf0, 0xf0, 0xf0));
	// 1 
	// 1 #ifdef WINDOWS_VERSION_10
	// 1 const theme darkMode	(RGB(0x00, 0x00, 0x00), RGB(0x67, 0x67, 0x67), RGB(0x4d, 0x4d, 0x4d), RGB(0x1d, 0x1d, 0x1d), RGB(0xff, 0xff, 0xff), RGB(0x1d, 0x1d, 0x1d));
	// 1 #endif

	class solidBrush {
		brushHandle handle;
	public:
	
		// Requires Destroy() to be called explicitly. Any recall of this function before destruction will result in memory leak.
		void Create(const color& brushColor) { handle = CreateSolidBrush(brushColor); }

		// Because i was thinking i was able to change values instead of memory location.
		//void Replace(const color& brushColor) { *handle = *(CreateSolidBrush(brushColor)); } 

		block Destroy() { DeleteObject(handle); }
		
		getter const brushHandle& Get() const noexcept { return handle; }
		
	};

	//class palette {
	//public:
	//	brushHandle
	//		backgroundSecondary { nullptr },
	//		backgroundSelected	{ nullptr },
	//		backgroundHovered	{ nullptr },
	//		backgroundPrimary	{ nullptr };
	//
	//	enum class Brushes {
	//		Secondary,
	//		Selected,
	//		Hovered,
	//		Primary
	//	};
	//
	//	// Requires Destroy() to be called explicitly.
	//	void Create(const theme& colorTheme) {
	//		backgroundSecondary = CreateSolidBrush(colorTheme.backgroundSecondary);
	//		backgroundSelected = CreateSolidBrush(colorTheme.backgroundSelected);
	//		backgroundHovered = CreateSolidBrush(colorTheme.backgroundHovered);
	//		backgroundPrimary = CreateSolidBrush(colorTheme.backgroundPrimary);
	//	}
	//
	//	void Destroy() {
	//		DeleteObject(backgroundSecondary);
	//		DeleteObject(backgroundSelected);
	//		DeleteObject(backgroundHovered);
	//		DeleteObject(backgroundPrimary);
	//	}
	//
	//	void Get() {}
	//};

}