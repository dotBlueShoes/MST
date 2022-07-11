#pragma once

#include "../console.hpp"
#include "frame.hpp"

namespace mst::winapi::console::draw {

	namespace color {
		const console::color
			green { { L'0', L';' }, { L'2', L'5', L'6', L';' }, { L'0', L'm' } },
			blue { { L'0', L';' }, { L'0', L';' }, { L'2', L'5', L'6', L'm' } },
			red { { L'2', L'5', L'6', L';' }, { L'0', L';' }, { L'0', L'm' } };
	}

	block Fullfil(wchar* buffer, const wchar& character) {
		for (size i = 0; i < winapi::console::frame::window::bufforLength; ++i)
			buffer[i] = character;
	}
	

	constexpr block Rectangle(
		wchar* buffer,
		const wchar& character,
		const uint64& posX, 
		const uint64& posY, 
		const uint64& length, 
		const uint64& height
	) {
		
		if /* constexpr */ (posX + length > winapi::console::frame::window::x ||
			posY + height > winapi::console::frame::window::y) throw;

		// Example
		// 1. 2. 3. 4. 5. 6. 7. 8. 9. 10. 120.
		// 121. 122. 123. 124. 125. 126. 127. 128. 129. 130. 240
		// 241.

		for (size i = posX; i < posX + length; ++i)
			for (size j = posY * winapi::console::frame::window::x; j < ((posY + height) * winapi::console::frame::window::x); j += winapi::console::frame::window::x)
				buffer[i + j] = character;

	}

	block RedrawScreen(const frame::window::frameCharacterBuffor* frames) {
		namespace coc = console::output::command;
		// This sample code gives us information of:
		//  1. We cannot refer to frame size when counting virtual terminal functionality e.g. color changing.
		//  2. This sample does not show how frames 1, 2, 3 would be copied and drawed. only how a sinlge frame can be drawed.
		//  3. coc::command : It does suck that after all we need to copy all of them into one instead of having them all in one buffor from the start.
		//  4. debug information can be shown in seperate buffer?
		//  5. fps i belive can be counted better.
		//  6. we need better way of showing information in console title. as now we override previosly setted "Troghol" title.
		//  7. redrawScreen should simply call console::output::command::Write(output.Pointer(), output.Length())
		//   with all the buffor setting up being done seperatry.
		//  8. And so there would be drawing functions which would apply changes to the buffor.
		//  9. All the drawing should be done via console::output handle, all cerr's cout's should be flushed somewhere else.
		//  10. There should be place for keyboard events listener that ideally would be able to close the app, maybe change the buffor.
		// Similar Project:
		//  - https://www.youtube.com/watch?v=TW_OVQkmDXU
		//  - https://github.com/ssusnic/Windows-Console-Game-Framework
		// Screen Buffer:
		//  - https://docs.microsoft.com/en-us/windows/console/console-screen-buffers
		// FPS
		//  - https://stackoverflow.com/questions/5508922/whats-the-usual-way-of-controlling-frame-rate
		//  - https://noobtuts.com/cpp/frames-per-second
		//  - https://stackoverflow.com/questions/2808398/easily-measure-elapsed-time
		//  - https://en.cppreference.com/w/cpp/chrono/steady_clock/now
		//  - https://en.cppreference.com/w/cpp/chrono/duration

		coc::command setTextColorR { coc::construct::SetTextColor(color::red) };
		coc::command setTextColorG { coc::construct::SetTextColor(color::green) };

		const size length(frames[0].Length() / 2);
		coc::command text1(length);

		for (size i = 0; i < length; ++i)
			text1.SetCurrentElement(frames[0].Pointer()[i]);

		coc::command text2(length);
		for (size i = length; i < 2 * length; ++i)
			text2.SetCurrentElement(frames[0].Pointer()[i]);

		coc::command output { &setTextColorR, &text1, &setTextColorG, &text2 };
		console::output::command::Write(output.Pointer(), output.Length());
		//console::output::command::Write(frame[0].Pointer(), frame[0].Length());
	}

	block MainLoop(const frame::window::frameCharacterBuffor* frames) {

		// Game Loop
		auto begin = std::chrono::steady_clock::now();
		uint64 fpsCount = 0;

		while (frame::isRunning) {
			RedrawScreen(frames);

			{ // Frame Count

				namespace coc = console::output::command;
				auto end = std::chrono::steady_clock::now();
				//std::chrono::duration<double> difference = end - begin;
				// https://en.cppreference.com/w/cpp/chrono/duration

				if (auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() >= 1000) {

					begin = std::chrono::steady_clock::now();
					frame::fps = fpsCount;
					fpsCount = 0;

					{ // Set Title.
						array<winapi::wchar, 10> buffor;
						winapi::Uint64ToWcharsTerminated(buffor, frame::fps, '\0');

						coc::command setTitle { coc::construct::SetTitle(buffor.Pointer(), buffor.Length()) };
						coc::Write(setTitle.Pointer(), setTitle.Length());
					}
				}
			} ++fpsCount;
		}
	}
}