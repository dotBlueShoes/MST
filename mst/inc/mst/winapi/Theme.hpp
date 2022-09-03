#pragma once
#include "DarkMode.hpp"

namespace mst::winapi::window::theme {
	
	template <class TypeFirst, class TypeRest>
	concept IsTypeColor = requires (const TypeFirst& valueFirst, const TypeRest& valueRest) {
		std::same_as<TypeFirst, TypeRest>;
		std::same_as<color, TypeFirst>;
	};
	
	template <uint64 length>
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
		template <class TypeFirst, class... TypeRest> 
		requires IsTypeColor<TypeFirst, FirstType<TypeRest...>>
		constexpr palette (const TypeFirst& newFirstColor, const TypeRest&... newRestColors) 
			: colors { newFirstColor, newRestColors... } {}
		
	};
	
	const palette<6> sample { 
		RGB(0xf0, 0xf0, 0xf0),
		RGB(0xff, 0xff, 0xff), 
		RGB(0xcc, 0xe8, 0xff), 
		RGB(0xe5, 0xf3, 0xff), 
		RGB(0xff, 0xff, 0xff), 
		RGB(0x00, 0x00, 0x20)
	};

	struct theme {

		const color
			backgroundSecondary,
			backgroundSelected,
			backgroundHovered,
			backgroundPrimary,
			borderColor,
			textPrimary;

		theme(
			const color newBackgroundSecondary,
			const color newBackgroundSelected,
			const color newBackgroundHovered,
			const color newBackgroundPrimary,
			const color newTextPrimary,
			const color newBorderColor
		) :
			backgroundSecondary(newBackgroundSecondary),
			backgroundSelected(newBackgroundSelected),
			backgroundHovered(newBackgroundHovered),
			backgroundPrimary(newBackgroundPrimary),
			borderColor(newBorderColor),
			textPrimary(newTextPrimary)
		{}

	};

	const theme lightMode	(RGB(0xff, 0xff, 0xff), RGB(0xcc, 0xe8, 0xff), RGB(0xe5, 0xf3, 0xff), RGB(0xff, 0xff, 0xff), RGB(0x00, 0x00, 0x20), RGB(0xf0, 0xf0, 0xf0));

	#ifdef WINDOWS_VERSION_10
	const theme darkMode	(RGB(0x00, 0x00, 0x00), RGB(0x67, 0x67, 0x67), RGB(0x4d, 0x4d, 0x4d), RGB(0x1d, 0x1d, 0x1d), RGB(0xff, 0xff, 0xff), RGB(0x1d, 0x1d, 0x1d));
	#endif

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