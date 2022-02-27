#pragma once
#include "winapi/types.hpp"
#include "winapi/modules.hpp"
#include "winapi/window.hpp"
#include "winapi/theme.hpp"
#include "winapi/uahmenubar.hpp"
#include "winapi/application.hpp"
#include "winapi/registry.hpp"
#include "charactertypes.hpp"
#include "array.hpp"

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

		template <size arrSize>
		inline constexpr void Uint64ToWcharsTerminated(
			array<wchar, arrSize>& buffor,
			uint64 rawValue,
			const wchar& endSign = wcharTerminator
		) {
			array<wchar, arrSize> tempBuffor;										// Reverse next logic use.
			wchar* const tempBufforEnd = tempBuffor.Pointer() + tempBuffor.Length();
			wchar* rNext = tempBufforEnd;

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

		template <size arrSize>
		inline constexpr void Uint64ToWchars(
			array<wchar, arrSize>& buffor,
			uint64 rawValue
		) {
			const size wcharTypeLength = 16;
			array<wchar, arrSize> tempBuffor;									// Reverse next logic use.
			wchar* rNext = tempBuffor.Pointer() + tempBuffor.Length() - wcharTypeLength;

			do {
				*(--rNext) = (wchar)(L'0' + rawValue % 10);
				rawValue /= 10;
			} while (rawValue != 0);

			memcpy(buffor.Pointer(), tempBuffor.Pointer(), sizeof buffor);
		}

		namespace message {
			inline auto Get(retrivedMessage& retrived) { return GetMessage(&retrived, nullptr, 0, 0); }
			inline auto Translate(retrivedMessage& retrived) { return TranslateMessage(&retrived); }
			inline auto Dispatch(retrivedMessage& retrived) { return DispatchMessage(&retrived); }
		}

		namespace accelerator {
			inline auto Load(handleInstnace& instance, wchar* identifier) { return LoadAccelerators(instance, identifier); }
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
