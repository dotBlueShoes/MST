//
// ! Important
//  Initialize() & Destroy() actually don't make the flow of this file clear.
// If you're inlcuding this file make sure that these are being called otherwise
// It will you give you errors as there are actions that take part at 
// initialization of global variables for example "input thread".
//
#pragma once

#include "../winapi.hpp"
#include "../narray.hpp"
#include "../charactertypes.hpp"
#include "../string.hpp"
#include "../thread.hpp"

#undef rename
#include <iostream>
#include <charconv>

// As said our enviroment is windows enviroment only.
//  Including windows console.

#include <windows.h>
#undef rename
#define rename __forceinline

#pragma warning(disable:6386)

// Notes
/*
// New Console API thing i guess.
// https://docs.microsoft.com/en-us/windows/console/console-virtual-terminal-sequences
// Input
// https://devblogs.microsoft.com/oldnewthing/20130506-00/?p=4453
// https://stackoverflow.com/questions/6285270/how-can-i-get-the-mouse-position-in-a-console-program

// About handling commands.
//  because we never know whether the higher function want to set up single command
//  or sumup and flush multiple at once. We could simply leave one extra space on heat.
//  For that null termination that we're questioning.
// Therefore it goes like this:
//  - We create a command with it's length returned to us.
//  - We either create another or we don't.
//  - We sum up those onto single one.
//  - We write that to the console output.	// Well lets learn about the limit. If there is we might create a whole system with buffors and flushing.

// It appears that the maximum of characters that can be possibly written is '4,294,967,295'
//  which is in fact the maximum 32-bit intiger value.
//  Due to such high value i think it's impossible to bypass the limit intentionally.
// Therefore it isn't needed to have a custom system around it.
//  Which leads to assumtion that i can freely connect as many string as i want and fluch them
//  whit this function whenever i wanted.
*/
//MoveWindow(GetConsoleWindow(), windowRect.left, windowRect.top, windowRect.right - windowRect.left, windowRect.bottom - windowRect.top, true);

// About Fonts...
/*
// About Font Customization in Windows Console.
//	raster font or true type / open type are possible if monospaced.
//	letter spacing cannot be done in console.
//	i would need a font that would share fixed width and height.
//	apperantly those fonts aren't easy to find
*/

namespace mst::winapi::console {

	using namespace mst::characters_n;
	using namespace mst::thread_n;
	using namespace mst::string_n;
	using namespace mst::nArray_n;
	using namespace mst::array_n;
	//using namespace mst::vector_n;

	using bufforType = winapi::wchar;
	using buffor = bufforType*;

	struct color { decimal<bufforType> red, green, blue; };

	// Have in mind that winapi here uses nullTermination therefore we use it too.
	struct font {
		uint32 sysFontTableIndex { 0 };												// Index of the font in the system's console font table.
		struct { bufforType value[32] { 0 }; size length { 0 }; } faceName;
		struct { int16 x { 0 }, y { 0 }; } length;
		uint32 fontFamily { FF_DONTCARE };
		uint32 fontWeight { FW_NORMAL };
	};

	#ifdef LOGGER
	namespace logger {
		PROCESS_INFORMATION processInformation { 0 };

		block CreateLogger() {
			STARTUPINFO startupInformation { 0 };
			startupInformation.cb = sizeof startupInformation;

			LPCWSTR appName { L"C:\\WINDOWS\\system32\\cmd.exe" };
			//LPCWSTR appName { L"C:\\WINDOWS\\system32\\notepad.exe" };

			if ( !CreateProcess (
				appName,				// No module name (use command line).
				NULL,					// Command line.
				NULL,					// Process handle not inheritable.
				NULL,					// Thread handle not inheritable.
				FALSE,					// Set handle inheritance to FALSE.
				CREATE_NEW_CONSOLE,		// No creation flags.
				NULL,					// Use parent's environment block.
				NULL,					// Use parent's starting directory.
				&startupInformation,	// Pointer to STARTUPINFO structure.
				&processInformation		// Pointer to PROCESS_INFORMATION structure.
			)) {
				printf("CreateProcess failed (%d).\n", GetLastError());
				return;
			}
			std::cerr << processInformation.dwThreadId << '\n';
			// Wait until child process exits.
			while (true);
			//WaitForSingleObject(processInformation.hProcess, INFINITE);

			// Close process and thread handles. 
			//CloseHandle(pi.hProcess);
			//CloseHandle(pi.hThread);
		}
	}
	#endif

	namespace output {

		winapi::objectHandle outputHandle { 0 };

		// Console TODO 
		//  It's appears very unneded to create these types each time we need their information.
		//  Instead i could create them once, call the function they need to work once
		//  and then from now on read it's value using my logic:
		//   private class
		//   pointers / addreses
		//   union

		struct consoleInfo {
			winapi::consoleScreenBufferInfo bufferInfo;
			winapi::consoleFontInfo fontInfo;

			consoleInfo(const winapi::objectHandle& outputHandle) {
				bufferInfo.cbSize = sizeof bufferInfo;
				fontInfo.cbSize = sizeof fontInfo;

				GetConsoleScreenBufferInfoEx(outputHandle, &bufferInfo);
				GetCurrentConsoleFontEx(outputHandle, false, &fontInfo);
			}

		};

		struct {
			winapi::smallRect consoleSize {};
			winapi::coord bufferSize {};
			uint32 outputMode {};
			color color {};
			font font {};
		} previousConsoleSetup;

		// AKA. Console Virtual Terminal Sequences API.
		namespace command {
			using command = unsafe::nArray<bufforType>;

			template <size length>
			using compileTime_command = const array<bufforType, length>;

			const compileTime_command<11>
				clear { L'\x1b', L'[', L'2', L'J', L'\x1b', L'[', L'H', L'\x1b', L'[', L'3', L'J' };
				//clear(L"\x1b[2J\x1b[H\x1b[3J"_nt);

			const compileTime_command<6>
				cursorShow { L'\x1b', L'[', L'?', L'2', L'5', L'h' },
				cursorHide { L'\x1b', L'[', L'?', L'2', L'5', L'l' };

			const compileTime_command<5>
				textFormattingNegativeColorRemove { L'\x1b', L'[', L'2', L'7', L'm' },
				textFormattingUnderlineRemove { L'\x1b', L'[', L'2', L'4', L'm' },
				textFormattingBoldRemove { L'\x1b', L'[', L'2', L'2', L'm' };

			const compileTime_command<4>
				textFormattingNegativeColor { L'\x1b', L'[', L'7', L'm' },
				textFormattingUnderline { L'\x1b', L'[', L'4', L'm' },
				textFormattingNormal { L'\x1b', L'[', L'0', L'm' },
				textFormattingBold { L'\x1b', L'[', L'1', L'm' };

			namespace construct {

				// We have 2 ways of Retriving the commands.
				// - one returns the Object and fulfilds constructor
				// - one gets the Object pointer and unsafety assigns data to the object inner array.

				// Remember construct namespace is only being used for creating commands both real and test ones
				//  Anything that is known like if we would know that the window has to be specific size we set it up 
				//  via 'compileTime_command' type.

				/*
				retriver command SetTitle(const string& title) {

					const size length { 5 + title.length() };
					buffor temp { new bufforType[length] };

					temp[0] = L'\x1b';
					temp[1] = L']';
					temp[2] = L'2';
					temp[3] = L';';

					for (size i = 0; i < title.length(); i++)
						temp[4 + i] = title[i];

					temp[length - 1] = L'\x07';

					return { temp, length };
				}
				*/

				retriver command SetTitle(const bufforType* title, const size& titleLength) {

					const size length { 5 + titleLength };
					buffor temp { new bufforType[length] };

					temp[0] = L'\x1b';
					temp[1] = L']';
					temp[2] = L'2';
					temp[3] = L';';

					for (size i = 0; i < titleLength; ++i)
						temp[4 + i] = title[i];

					temp[length - 1] = L'\x07';

					return { temp, length };
				}

				retriver command SetTextColor(const color& textColor) {

					const size length { 7 +
						textColor.red.length +
						textColor.green.length +
						textColor.blue.length 
					};

					buffor temp { new bufforType[length] };

					// As to why the length
					//  The fisrt characters take the length of 7.
					//  "Decimal" type is structured in a way that it has an extra sign attached at the end. 
					//   therefore 3 characters are the length of 4. So each sufix after the 
					//   color in command is being counted here.
					//  SAMPLE COMMAND "\x1b[38;2;255;255;255m".

					// PREFIX
					temp[0] = L'\x1b';
					temp[1] = L'[';
					temp[2] = L'3';
					temp[3] = L'8';
					temp[4] = L';';
					temp[5] = L'2';
					temp[6] = L';';

					// RED
					for (size i = 0; i < textColor.red.length; ++i)
						temp[7 + i] = textColor.red.characters[i];

					// GREEN
					for (size i = 0; i < textColor.green.length; ++i)
						temp[7 + textColor.red.length + i] = textColor.green.characters[i];

					// BLUE
					for (size i = 0; i < textColor.blue.length; ++i)
						temp[7 + textColor.red.length + textColor.green.length + i] = textColor.blue.characters[i];

					return { temp, length };
				}

				retriver command SetTextBackgroundColor(
					const decimal<bufforType>& red,
					const decimal<bufforType>& green,
					const decimal<bufforType>& blue
				) {
					const size length { 7 + red.length + green.length + blue.length };
					buffor temp { new bufforType[length] };

					// As to why the length
					//  The fisrt characters take the length of 7.
					//  "Decimal" type is structured in a way that it has an extra sign attached at the end. 
					//   therefore 3 characters are the length of 4. So each sufix after the 
					//   color in command is being counted here.
					//  SAMPLE COMMAND "\x1b[48;2;255;255;255m".

					// PREFIX
					temp[0] = L'\x1b';
					temp[1] = L'[';
					temp[2] = L'4';
					temp[3] = L'8';
					temp[4] = L';';
					temp[5] = L'2';
					temp[6] = L';';

					// RED
					for (size i = 0; i < red.length; ++i)
						temp[7 + i] = red.characters[i];

					// GREEN
					for (size i = 0; i < green.length; ++i)
						temp[7 + red.length + i] = green.characters[i];

					// BLUE
					for (size i = 0; i < blue.length; ++i)
						temp[7 + red.length + green.length + i] = blue.characters[i];

					return { temp, length }; // SUCCESS
				}

				retriver command SetCursorPosition(
					const decimal<bufforType>& x,
					const decimal<bufforType>& y
				) {
					const size length { 2 + x.length + y.length };
					buffor temp { new bufforType[length] };

					temp[0] = L'\x1b';
					temp[1] = L'[';

					for (size i = 0; i < y.length; ++i)
						temp[2 + i] = y.characters[i];

					for (size i = 0; i < x.length; ++i)
						temp[2 + y.length + i] = x.characters[i];
					return { temp, length };
				}

				retriver command SetScrollMargin(
					const decimal<bufforType>& marginTop,
					const decimal<bufforType>& marginBottom
				) {
					const size length { 2 + marginTop.length + marginBottom.length };
					buffor temp { new bufforType[length] };

					temp[0] = L'\x1b';
					temp[1] = L'[';

					for (size i = 0; i < marginTop.length; ++i)
						temp[2 + i] = marginTop.characters[i];

					for (size i = 0; i < marginBottom.length; ++i)
						temp[2 + marginTop.length + i] = marginBottom.characters[i];

					return { temp, length };
				}

				namespace unsafe {
					getter size SetTitleLength(const size& titleLength) { return 5 + titleLength; }
					block SetTitle(command* const command, const string& title) {

						const size titleLength { title.length() };

						(*command).SetCurrentElement(L'\x1b');
						(*command).SetCurrentElement(L']');
						(*command).SetCurrentElement(L'2');
						(*command).SetCurrentElement(L';');

						for (size i = 0; i < titleLength; ++i)
							(*command).SetCurrentElement(title[i]);

						(*command).SetCurrentElement(L'\x07');
					}

					getter size SetTextColorLength(const color& textColor) { return 7 + textColor.red.length + textColor.green.length + textColor.blue.length; }
					block SetTextColor(command& command, const color& textColor) {

						const size length { 7 +
							textColor.red.length +
							textColor.green.length +
							textColor.blue.length 
						};

						command.SetCurrentElement(L'\x1b');
						command.SetCurrentElement(L'[');
						command.SetCurrentElement(L'3');
						command.SetCurrentElement(L'8');
						command.SetCurrentElement(L';');
						command.SetCurrentElement(L'2');
						command.SetCurrentElement(L';');

						for (size i = 0; i < textColor.red.length; ++i)
							command.SetCurrentElement(textColor.red.characters[i]);

						for (size i = 0; i < textColor.green.length; ++i)
							command.SetCurrentElement(textColor.green.characters[i]);

						for (size i = 0; i < textColor.blue.length; ++i)
							command.SetCurrentElement(textColor.blue.characters[i]);
					}

					getter size SetTextBackgroundColorLength(const color& textColor) { return 7 + textColor.red.length + textColor.green.length + textColor.blue.length; }
					block SetTextBackgroundColor(command& command, const color& textColor) {

						const size length { 7 +
							textColor.red.length +
							textColor.green.length +
							textColor.blue.length 
						};

						command.SetCurrentElement(L'\x1b');
						command.SetCurrentElement(L'[');
						command.SetCurrentElement(L'4');
						command.SetCurrentElement(L'8');
						command.SetCurrentElement(L';');
						command.SetCurrentElement(L'2');
						command.SetCurrentElement(L';');

						for (size i = 0; i < textColor.red.length; ++i)
							command.SetCurrentElement(textColor.red.characters[i]);

						for (size i = 0; i < textColor.green.length; ++i)
							command.SetCurrentElement(textColor.green.characters[i]);

						for (size i = 0; i < textColor.blue.length; ++i)
							command.SetCurrentElement(textColor.blue.characters[i]);
					}

					getter size SetCursorPositionLength(const decimal<bufforType>& x, const decimal<bufforType>& y) { return 2 + x.length + y.length; }
					block SetCursorPosition(command& command, const decimal<bufforType>& x, const decimal<bufforType>& y) {
						const size length { 2 + x.length + y.length };

						command.SetCurrentElement(L'\x1b');
						command.SetCurrentElement(L'[');

						for (size i = 0; i < y.length; ++i)
							command.SetCurrentElement(y.characters[i]);

						for (size i = 0; i < x.length; ++i)
							command.SetCurrentElement(x.characters[i]);
					}
				}

			}

			uint64 Write(const bufforType* const commandsBuffor, const size& bufforLength) {
				uint32 writtenLength { 0 };

				//if (!WriteConsoleW(outputHandle, commandsBuffor, (uint32)(bufforLength), reinterpret_cast<LPDWORD>(&writtenLength), nullptr))
				//	return ::GetLastError();
				WriteConsoleW(outputHandle, commandsBuffor, (uint32)(bufforLength), reinterpret_cast<LPDWORD>(&writtenLength), nullptr);

				return 0;
			}

			namespace test {

			#define SUCCESS 0 

				uint64 FrameFull(const int16& x, const int16& y, const buffor frame, const size frameLength) {
					uint32 writtenLength { 0 };

					
					//const size length = x * y;
					//buffor data { new bufforType[length] };
					//for (size i = 0; i < x * y; ++i)
					//	data[i] = L'*';

					//const command frame { data, length };

					return Write(frame, frameLength);
				}

				int64 DisplayTextColors() {
					color textColor { { L'0', L';' }, { L'0', L';' }, { L'0', L'm' } };

					// It goes blue -> green -> red.
					for (uint64 i(0); i < 256ull * 256ull * 256ull; ++i) {

						const command setTextColor { construct::SetTextColor(textColor) };
						const command text1 { L'*' };
						const command output { &setTextColor, &text1 };
						Write(output.Pointer(), output.Length());

						++textColor.blue;

						{
							const char green256[] { L'2', L'5', L'6', L';' };
							const char blue256[] { L'2', L'5', L'6', L'm' };

							if (textColor.blue == blue256) {
								textColor.blue = { L'0', L'm' };
								++textColor.green;
							}

							if (textColor.green == green256) {
								textColor.green = { L'0', L';' };
								++textColor.red;
							}
						}
						
					}

					return SUCCESS;
				}

				int64 DisplayTextBackgroundColors_FlamePattern() {
					decimal<winapi::wchar> red { L'0', L';' }, green { L'0', L';' }, blue { L'0', L'm' };
					const command text1 { L' ' };

					for (int64 i { 0 }; i < 256; i++) {

						const command setTextColor { construct::SetTextBackgroundColor(red, green, blue) };
						const command output { &setTextColor, &text1 };
						Write(output.Pointer(), output.Length());

						++red;
					}

					for (int64 i { 0 }; i < 256; i++) {

						const command setTextColor { construct::SetTextBackgroundColor(red, green, blue) };
						const command output { &setTextColor, &text1 };
						Write(output.Pointer(), output.Length());

						++green;
					}

					for (int64 i { 0 }; i < 256; i++) {

						const command setTextColor { construct::SetTextBackgroundColor(red, green, blue) };
						const command output { &setTextColor, &text1 };
						Write(output.Pointer(), output.Length());

						++blue;
					}

					return SUCCESS;
				}

				int64 DisplayTextBackgroundColors() {
					decimal<winapi::wchar> red { L'0', L';' }, green { L'0', L';' }, blue { L'0', L'm' };

					std::cerr << "TEXT FORMATING\n";

					// It goes blue -> green -> red.
					for (uint64 i { 0 }; i < 256ull * 256ull * 256ull; i++) {

						const command setTextBackgroundColor { construct::SetTextBackgroundColor(red, green, blue) };
						const command text1 { L' ' };
						const command output { &setTextBackgroundColor, &text1 };
						Write(output.Pointer(), output.Length());

						++blue;

						{
							const char green256[] { L'2', L'5', L'6', L';' };
							const char blue256[] { L'2', L'5', L'6', L'm' };

							if (blue == blue256) {
								blue = { L'0', L'm' };
								++green;
							}

							if (green == green256) {
								green = { L'0', L';' };
								++red;
							}
						}
						
					}

					return SUCCESS;
				}

				int64 PaintWhiteOnPosition(const uint64 rawPositionX, const uint64 rawPositionY) {
					// First we're converting passed integer to wchar array format.
					//  We're setting the buffor length equal to 5 as the number of rows and cols shouldn't be ever higher then current screen rosorutions + nulltermination.
					array<bufforType, 5> xPositionBuffor, yPositionBuffor;
					winapi::Uint64ToWcharsTerminated<5>(xPositionBuffor, rawPositionX, L'H');
					winapi::Uint64ToWcharsTerminated<5>(yPositionBuffor, rawPositionY, L';');

					{	// Then we're creating the command itself and writing it.
						const decimal<bufforType> xPosition(xPositionBuffor.Pointer(), L'H'),
							yPosition(yPositionBuffor.Pointer(), L';');

						const color textColor { 
							{ L'2', L'5', L'5', L';'}, 
							{ L'2', L'5', L'5', L';'}, 
							{ L'2', L'5', L'5', L'm'} 
						};

						const size comamndLength {
							construct::unsafe::SetCursorPositionLength(xPosition, yPosition) +
							construct::unsafe::SetTextBackgroundColorLength(textColor) + 1
						};

						command message(comamndLength);
						construct::unsafe::SetCursorPosition(message, xPosition, yPosition);
						construct::unsafe::SetTextBackgroundColor(message, textColor);
						message.SetCurrentElement(L' ');

						Write(message.Pointer(), message.Length());
					}
					

					return SUCCESS;
				}

				//int64 DisplayASCII() {
				//
				//	string output;
				//	for (uchar8 character = 0; character < 255; character++)
				//		output += character;
				//
				//	std::cout << output;
				//
				//	return 0; // SUCCESS
				//}
			}

			/*
			namespace construct {

				Command SetTitle(const std::string& title) {

					const size length = 6 + title.length();
					Command temp(length);

					temp.Data()[0] = L'\x1b';
					temp.Data()[1] = L']';
					temp.Data()[2] = L'2';
					temp.Data()[3] = L';';

					for (size i = 0; i < title.length(); i++)
						temp.Data()[4 + i] = title[i];

					temp.Data()[length - 2] = L'\x07';

					//for (auto& character : temp)
					//	std::cout << character << '\n';

					return temp;
				}

				int64* SetTitleSample(const std::string& title) {
					int64* temp = new int64[18];
					return temp;
				}

				//block NullTerminate(const winapi::LPWStr& command, const size& length) { command[length - 1] = L'\0'; }
			}

		}
		*/

		/*
		int64 Clear() {
			// Write the sequence for clearing the display.
			winapi::PCWStr sequence = L"\x1b[2J\x1b[H\x1b[3J";
			winapi::DWord written = 0;

			if (!WriteConsole(console, sequence, (winapi::DWord)wcslen(sequence), &written, nullptr)) {
				// If we fail, try to restore the mode on the way out.
				SetConsoleMode(console, previousMode);
				return ::GetLastError();
			}

			// To also clear the scroll back, emit L"\x1b[3J" as well.
			// 2J only clears the visible window and 3J only clears the scroll back.

			return 0;
		}
		*/
		}

		block GetCurrentFont(font& fontStruct, const consoleInfo& consoleInfo) {
			size length { 0 };

			for (; consoleInfo.fontInfo.FaceName[length] != L'\0'; ++length); ++length;
			fontStruct.faceName.length = length;

			for (size i = 0; i < length; i++)
				fontStruct.faceName.value[i] = consoleInfo.fontInfo.FaceName[i];

			// 2 for's and assignments vs 1 for and additions.

			fontStruct.sysFontTableIndex = consoleInfo.fontInfo.nFont;
			fontStruct.fontFamily = consoleInfo.fontInfo.FontFamily;
			fontStruct.fontWeight = consoleInfo.fontInfo.FontWeight;
			fontStruct.length.x = consoleInfo.fontInfo.dwFontSize.X;
			fontStruct.length.y = consoleInfo.fontInfo.dwFontSize.Y;
		}

		block ChangeFont(const font& newFont) {
			// Why isn't it initialized? 
			//  all i know is that the API works this way and apperently 
			//  winapi::consoleFontInfo is like 98 bytes wheres here we use like 16
			// Making fontInfo with "{}" would cause an addition assignment to 0 for EVERY var inside.

			winapi::consoleFontInfo fontInfo {}; // It does uses more RAM with an initalization... i guess lets just live with that. It's winapi fault.
			fontInfo.cbSize = sizeof fontInfo;

			fontInfo.nFont = newFont.sysFontTableIndex;
			fontInfo.dwFontSize.X = newFont.length.x;
			fontInfo.dwFontSize.Y = newFont.length.y;
			fontInfo.FontFamily = newFont.fontFamily;
			fontInfo.FontWeight = newFont.fontWeight;

			wcscpy_s(fontInfo.FaceName, newFont.faceName.length, newFont.faceName.value);
			if (!SetCurrentConsoleFontEx(outputHandle, false, &fontInfo))
				std::cerr << "Failed To Change the Font.";
		}

		block GetCurrentConsoleSize(winapi::smallRect& windowRect, const consoleInfo& consoleInfo) {
			windowRect = consoleInfo.bufferInfo.srWindow;
		}

		block ChangeConsoleSize(SMALL_RECT windowRect) {
			if (!SetConsoleWindowInfo(outputHandle, true, &windowRect))
				std::cerr << "Failed to change console rows/cols.\t" << GetLastError() << '\n';
		}

		block GetCurrentBufferSize(winapi::coord& bufforSize, const consoleInfo& consoleInfo) {
			bufforSize = consoleInfo.bufferInfo.dwSize;
			//std::cerr << bufferInfo.
			//bufferInfo.dwSize = size;
			//dwCursorPosition;
			//wAttributes;
			//srWindow;
			//dwMaximumWindowSize;
			//wPopupAttributes;
			//bFullscreenSupported;
			//ColorTable[16];
		}

		// Note that buffor therefore function 'size' parameter connot be smaller from the window size.
		block ChangeBufferSize(COORD newSize) {
			if (!SetConsoleScreenBufferSize(outputHandle, newSize))
				std::cerr << "Failed to change console buffer rows/cols.\t" << GetLastError() << '\n';
		}

		block ChangeEqualConsoleSize(const int16& x, const int16& y) {
			namespace co = console::output;
			const winapi::smallRect windowSize { 0, 0, x - 1, y - 1 }; // 0, 0 means something windowsOS just decide what position yourself.
			const winapi::coord bufforSize { x, y };

			// If bufor orginalnie jest mniejszy od nowych rozmiarów okna, to
			// - najpierw zmieniamy rozmiar bufora.
			// If bufor orginalnie jest wiêkszy od nowych rozmiarów okna, to
			// - najpierw zmieniamy rozmiar okna.

			if (co::previousConsoleSetup.bufferSize.X < bufforSize.X ||
				co::previousConsoleSetup.bufferSize.Y < bufforSize.Y) {
				co::ChangeBufferSize(bufforSize);
				co::ChangeConsoleSize(windowSize);
			} else {
				co::ChangeConsoleSize(windowSize);
				co::ChangeBufferSize(bufforSize);
			}
		}

		namespace screen {

			// First buffor displays stuff.
			// Second buffor works as a swaper between First and Third.
			// Third buffor is where we define new objects.
			//void* windowBuffer[3];

			//block DrawFrame(const winapi::handle& outputHandle, const uint32& screenSize) {
			//	
			//}

			getter uint16 GetRows(const winapi::smallRect& windowRect) {
				return windowRect.Bottom - windowRect.Top + 1;
			}

			getter uint16 GetCols(const winapi::smallRect& windowRect) {
				return windowRect.Right - windowRect.Left + 1;
			}

			getter uint32 GetScreenSize(const winapi::smallRect& windowRect) {
				return GetRows(windowRect) * GetCols(windowRect);
			}
		}

		// We intitialize the Console Virtual Terminal Sequences.
		// - Ideally we want to remove the use of it on exit so other program that 
		//  might run in the same console wont bug-out therefore call 'Destroy' on exit!
		block Initialize() {
			const uint32 virtualTerminalProcessing { ENABLE_VIRTUAL_TERMINAL_PROCESSING };				// Allows use of string commands that are passed via simple "WriteConsole" call.
			outputHandle = GetStdHandle(STD_OUTPUT_HANDLE);

			GetConsoleMode(outputHandle, reinterpret_cast<LPDWORD>(&previousConsoleSetup.outputMode));										// Fetch existing console mode so we correctly add a flag and not turn off others.
			SetConsoleMode(outputHandle, previousConsoleSetup.outputMode | virtualTerminalProcessing);			// Set the new mode.

			// I don't need to store the whole thing. Thus i have my own types that hold information
			//  that otherwise global consoleInfo would.
			{
				consoleInfo consoleInfo(outputHandle);

				GetCurrentConsoleSize(previousConsoleSetup.consoleSize, consoleInfo);
				GetCurrentBufferSize(previousConsoleSetup.bufferSize, consoleInfo);
				GetCurrentFont(previousConsoleSetup.font, consoleInfo);
			}
		}

		block Destroy() {
			SetConsoleMode(outputHandle, previousConsoleSetup.outputMode);										// Restore the mode on the way out to be nice to other command-line applications.

			ChangeConsoleSize(previousConsoleSetup.consoleSize);
			ChangeBufferSize(previousConsoleSetup.bufferSize);
			ChangeFont(previousConsoleSetup.font);

			command::Write(command::textFormattingNormal.Pointer(), command::textFormattingNormal.Length());			// To restore text to normal.
			command::Write(command::clear.Pointer(), command::clear.Length());										// to restore font to normal.
		}

	}

	namespace input {

		// The input we control is:
		//  - keyboard events
		//  - mouse events
		//  - windowSize events
		//  - focus events
		//  - menu events

		const winapi::objectHandle inputHandle { GetStdHandle(STD_INPUT_HANDLE) };

		// Now here we're declaring the creation of a thread.
		//  What really ennoys me is that the function starts.
		//  And even tho the console have not yet called the initialize
		//  it is already running... this is very error prone yet it works.
		namespace procedure { void Input(); } thread inputThread(procedure::Input);
		bool isInputThreadEnabled { true };

		uint32 previousInputMode;

		namespace procedure {
			block KeyEvent(winapi::keyEvent& key) {
				std::string output;

				if (key.bKeyDown) output = "Key was pressed\n";
				else output = "Key was released\n";

				//if (key.wVirtualKeyCode == 0x51)
				//	game::isRunning == false;

				std::cerr << output;
			}

			block MouseEvent(winapi::mouseEvent& mouse) {
				std::string output;

				// An interesting part. No idea why.
			#ifndef MOUSE_HWHEELED
			#define MOUSE_HWHEELED 0x0008
			#endif

				std::string temp {
					"Mouse: "
					"X: " + ToString(mouse.dwMousePosition.X) + ' ' +
					"Y: " + ToString(mouse.dwMousePosition.Y) + ' ' +
					"buttons: " + ToString(mouse.dwButtonState) + ' ' +
					"shift: " + ToString(mouse.dwControlKeyState) + ' ' +
					"flags: " + ToString(mouse.dwEventFlags) + '\n'
				};

				switch (mouse.dwEventFlags) {
					case 0:

						switch (mouse.dwButtonState) {
							case 0:
								output += temp + "Mouse release\n";
								break;
							case FROM_LEFT_1ST_BUTTON_PRESSED:
								output += temp + "Mouse left button pressed\n";
								break;
							case RIGHTMOST_BUTTON_PRESSED:
								output += temp + "Mouse right button pressed\n";
								break;
							case FROM_LEFT_2ND_BUTTON_PRESSED:
								output += temp + "2\n";
								break;
							case FROM_LEFT_3RD_BUTTON_PRESSED:
								output += temp + "3\n";
								break;
							case FROM_LEFT_4TH_BUTTON_PRESSED:
								output += temp + "4\n";
								break;
							default:
								output += temp + "Mouse unknown<" + ToString(mouse.dwButtonState) + "> button pressed\n";

						} break;

					case DOUBLE_CLICK:
						output += temp + "Mouse double click\n";
						break;

					case MOUSE_HWHEELED:
						output += temp + "Mouse horizontal wheel\n";
						break;

					case MOUSE_MOVED:
						//output += "Mouse moved";
						//console::output::command::test::PaintWhiteOnPosition((uint64)mouse.dwMousePosition.X + 1, (uint64)mouse.dwMousePosition.Y + 1);
						break;

					case MOUSE_WHEELED:
						output += temp + "Mouse vertical wheel\n";
						break;

					default:
						output += temp + "Mouse unknown<" + ToString(mouse.dwEventFlags) + ">\n";
						break;
				}

				std::cerr << output;
			}

			block ResizeEvent(winapi::resizeEvent& windowSize) {
				std::string output;

				output = "Resize event. Console screen buffer is " +
					ToString(windowSize.dwSize.X) + " columns by " +
					ToString(windowSize.dwSize.Y) + " rows.\n";

				std::cerr << output;
			}

			void Input() {
				array<winapi::inputRecord, 128> eventsBuffer {};
				uint32 eventsRead;

				while (isInputThreadEnabled) {
					ReadConsoleInput(inputHandle, eventsBuffer.Pointer(), (uint32)(eventsBuffer.Length()), reinterpret_cast<LPDWORD>(&eventsRead));
					for (uint32 i { 0 }; i < eventsRead; ++i)
						switch (eventsBuffer[i].EventType) {

							case KEY_EVENT:
								KeyEvent(eventsBuffer[i].Event.KeyEvent);
								break;

							case MOUSE_EVENT:
								MouseEvent(eventsBuffer[i].Event.MouseEvent);
								break;

							case WINDOW_BUFFER_SIZE_EVENT:
								ResizeEvent(eventsBuffer[i].Event.WindowBufferSizeEvent);
								break;

							case FOCUS_EVENT:  // disregard focus events
							case MENU_EVENT:   // disregard menu events
								break;

							default:
								std::cerr << "Unknown Event\n";
								break;
						}
				}
			}
		}

		block Initialize() {

			// QuickEditMode takes control over the mouse so we have to disable it.
			//  Because 'ENABLE_QUICK_EDIT_MODE' is an extended flag.. we have to enable those.
			//  Remeber we're removing it from an exsisting variable so yeah place your code in right place
			//  as it is right here.

			GetConsoleMode(inputHandle, reinterpret_cast<LPDWORD>(&previousInputMode));

			// Allows reading window, keyboard and mouse input.	
			const uint32 inputEvents {
				(previousInputMode | ENABLE_MOUSE_INPUT | ENABLE_EXTENDED_FLAGS) &
				~ENABLE_QUICK_EDIT_MODE 
			};

			SetConsoleMode(inputHandle, inputEvents);

		}

		block Destroy() {
			console::input::isInputThreadEnabled = false;
			inputThread.join();

			SetConsoleMode(inputHandle, previousInputMode | ENABLE_EXTENDED_FLAGS);
		}

	}

	block Initialize() {

		#ifdef LOGGER
		logger::CreateLogger();
		#endif

		output::Initialize();
		input::Initialize();
	}

	block Destroy() {
		input::Destroy();
		output::Destroy();
	}

}