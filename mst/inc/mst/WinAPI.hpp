#pragma once
#include "WinAPI/Types.hpp"
#include "WinAPI/Modules.hpp"
#include "WinAPI/Window.hpp"
#include "WinAPI/Theme.hpp"
#include "WinAPI/UAHMenuBar.hpp"
#include "WinAPI/Application.hpp"
#include "WinAPI/Registry.hpp"
#include "CharacterTypes.hpp"
#include "Array.hpp"

using namespace mst::array_n;

#undef SetConsoleTitle

namespace mst {

	namespace characters_n {

		inline bool operator==(const decimal<winapi::wchar>& lhs, const char& rhs) {
			if (lhs.characters[0] == rhs) return true;
			else return false;
		}

		// Use of this requires us to know that the rhs is the same size or longer.
		inline bool operator==(const decimal<winapi::wchar>& lhs, const char* rhs) {
			for (size i = 0; i < lhs.length; i++)
				if (lhs.characters[i] != rhs[i])
					return false;
			return true;
		}

	}

	namespace winapi {

		const wchar wcharTerminator = '\0';

		// THIS FUNCTION MIGHT NOT RUN AS SUPPOSED !!!
		template <size arrSize>
		inline constexpr void Uint64ToWcharsTerminated(
			array<wchar, arrSize>& buffor,
			uint64 rawValue,
			const wchar& endSign = wcharTerminator
		) {
			array<wchar, arrSize> tempBuffor; // Reverse next logic use.
			wchar* const tempBufforEnd ( tempBuffor.Pointer() + tempBuffor.Length() );
			wchar* rNext ( tempBufforEnd );

			// Termination
			*(--rNext) = endSign;

			do {
				*(--rNext) = (wchar)(L'0' + rawValue % 10);
				rawValue /= 10;
			} while (rawValue != 0);

			{
				// Don't know why byt for some reason A - 0 = 5 and not 10... 
				//  i dont know why but i need to double it. 
				//  I guess ptrdiff_t works diff then i thought.
				const ptrdiff_t digitsWritten = tempBufforEnd - rNext; 
				memcpy(buffor.Pointer(), rNext, (size)(digitsWritten) * 2);
			}
		}

		// THIS FUNCTION MIGHT NOT RUN AS SUPPOSED !!!
		template <size arrSize>
		inline constexpr uint64 Uint64ToWchars(
			array<wchar, arrSize>& buffor,
			uint64 rawValue
		) {
			array<wchar, arrSize> tempBuffor; // Reverse next logic use.
			wchar* const tempBufforEnd ( tempBuffor.Pointer() + tempBuffor.Length() );
			wchar* rNext ( tempBufforEnd );
			uint64 length ( 0 );

			do {
				*(--rNext) = (wchar)(L'0' + rawValue % 10);
				rawValue /= 10;
				length++;
			} while (rawValue != 0);

			{
				// Don't know why byt for some reason A - 0 = 5 and not 10... 
				//  i dont know why but i need to double it. 
				//  I guess ptrdiff_t works diff then i thought.
				const ptrdiff_t digitsWritten = tempBufforEnd - rNext; 
				memcpy(buffor.Pointer(), rNext, (size)(digitsWritten) * 2);
			}
			
			return length;
		}

		namespace message {
			inline auto Get(retrivedMessage& retrived) { return GetMessage(&retrived, nullptr, 0, 0); }
			inline auto Translate(retrivedMessage& retrived) { return TranslateMessage(&retrived); }
			inline auto Dispatch(retrivedMessage& retrived) { return DispatchMessage(&retrived); }
		}

		namespace accelerator {
			inline auto Load(handleInstnace& instance, wchar* identifier) { return LoadAcceleratorsW(instance, identifier); }
			inline auto Translate(windowHandle& window, keysMapHandle& keys, retrivedMessage& retrived) { return TranslateAccelerator(window, keys, &retrived); }
		}

		namespace wideString {
			inline auto Load(const handleInstnace& instance, const resourceType& resourceId, wchar* buffor, const int32& bufforLength) {
				return LoadStringW(instance, resourceId, buffor, bufforLength); }
		}

		inline constexpr auto GetMenuInput(messageW& w) { return LOWORD(w); }
		inline auto DefaultWindowProcedure(windowHandle& window, uint32& message, messageW& w, messageL& l) { 
			return DefWindowProc(window, message, w, l); }

	}
}
