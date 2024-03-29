#pragma once

#include "../Console.hpp"
#include "Palette.hpp"
#include "Frame.hpp"

namespace mst::winapi::console::draw {
	
	using namespace palette;

	namespace color {
		const console::color
			green { { L'0', L';' }, { L'2', L'5', L'6', L';' }, { L'0', L'm' } },
			blue { { L'0', L';' }, { L'0', L';' }, { L'2', L'5', L'6', L'm' } },
			red { { L'2', L'5', L'6', L';' }, { L'0', L';' }, { L'0', L'm' } };
	}
	
	//2 console::output::command::command output ( winapi::console::frame::window::maxBufforLength );

	block Fullfil(brush* buffer, const brush& nBrush) {
		for (size i = 0; i < winapi::console::frame::window::bufforLength; ++i) {
			buffer[i].character = nBrush.character;
			buffer[i].textColor = nBrush.textColor;
			buffer[i].backgroundColor = nBrush.backgroundColor;
		}
	}
	

	constexpr block Rectangle(
		brush* buffer,
		const brush& nBrush,
		const uint64& posX, 
		const uint64& posY, 
		const uint64& width, 
		const uint64& height
	) {
		
		if /* constexpr */ (posX + width > winapi::console::frame::window::x ||
			posY + height > winapi::console::frame::window::y) throw;

		// Example
		// 1. 2. 3. 4. 5. 6. 7. 8. 9. 10. 120.
		// 121. 122. 123. 124. 125. 126. 127. 128. 129. 130. 240
		// 241.

		for (size i = posX; i < posX + width; ++i)
			for ( size j = posY * winapi::console::frame::window::x; j < ((posY + height) * winapi::console::frame::window::x); j += winapi::console::frame::window::x ) {
				buffer[i + j].character = nBrush.character;
				buffer[i + j].textColor = nBrush.textColor;
				buffer[i + j].backgroundColor = nBrush.backgroundColor;
			}

	}

	block RedrawScreen(const frame::window::frameCharacterBuffor& frame) {
		namespace coc = console::output::command;
		namespace coccu = coc::construct::unsafe;
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
		
		//coc::command setTextColor // Storing the Text Color Command
		//	( coc::construct::SetTextColor ( frame[0].textColor ) );
		//coc::command setTextBackgroundColor // Storing the Background Color Command
		//	( coc::construct::SetTextBackgroundColor ( frame[0].backgroundColor ) );
		
		// Here we're setting up the character buffor. other are untoched.. not implemented..
		const size length ( frame.Length() );
		
		coc::command text ( length );
		
		for (size i = 0; i < length; ++i)
			text.SetCurrentElement(frame.Pointer()[i].character);
		
		coc::command output { /*&setTextBackgroundColor, &setTextColor,*/ &text };
		console::output::command::Write(output.Pointer(), output.Length());
		
		//2 // colors redo
		//2 {
		//2 	const size frameLength ( frame.Length() );
		//2 	size length ( frameLength ); // equal to number of characters initially.
		//2 	
		//2 	
		//2 	/// Theres prob. with memory leaks/reads
		//2 	/// This like certainly does not fix that.
		//2 	/// It also does not fix the frame drop...
		//2 	output.SetLengthZero(); // Clear the indicator. 
		//2 	
		//2 	for (size i = 0; i < frameLength; ++i) {
		//2 		length += coccu::SetTextBackgroundColorLength(frame.Pointer()[i].backgroundColor); // adding space for color commands
		//2 		length += coccu::SetTextColorLength(frame.Pointer()[i].textColor); // adding space for color commands
		//2 	}
		//2 	
		//2 	{
		//2 		for (size i = 0; i < frameLength; ++i) {
		//2 			coccu::SetTextBackgroundColor(output, frame.Pointer()[i].backgroundColor);
		//2 			coccu::SetTextColor(output, frame.Pointer()[i].textColor);
		//2 			output.SetCurrentElement(frame.Pointer()[i].character);
		//2 		}
		//2 		
		//2 		console::output::command::Write(output.Pointer(), length);
		//2 	}
		//2 }
	}

	block MainLoop(const frame::window::frameCharacterBuffor& frame) {

		// Game Loop
		auto begin = std::chrono::steady_clock::now();
		uint64 fpsCount = 0;
		
		// Theres an ERROR with an I and bell sound appearing !
		//  also double square sign was seen.
		// I ALSO SHOUD HAVE LOGIC TO PUT THE BUFFOR THATS INSIDE Uint64ToWchars HERE!
		array<winapi::wchar, 12> buffor;

		while (frame::isRunning) {
			//RedrawScreen(frame);

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
						uint64 length = winapi::Uint64ToWchars(buffor, frame::fps);

						coc::command setTitle { coc::construct::SetTitle(buffor.Pointer(), length) };
						coc::Write(setTitle.Pointer(), setTitle.Length());
					}
				}
			} ++fpsCount;
		}
	}
}