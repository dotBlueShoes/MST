#pragma once
#include "Window.hpp"
#include "Theme.hpp"

#include <uxtheme.h>
#include <vsstyle.h>
#pragma comment(lib, "uxtheme.lib")

// To draw border around an element!
// https://stackoverflow.com/questions/16159127/win32-how-to-draw-a-rectangle-around-a-text-string

//  KNOWN EXSISTING MENU ITEM FLAGS
// ODS_SELECTED		0b0000.0000.0000.0000.0000.0000.0000.0001	0x0001 	The menu item's status is selected.
// ODS_GRAYED		0b0000.0000.0000.0000.0000.0000.0000.0010	0x0002	The item is to be grayed.This bit is used only in a menu.
// ODS_DISABLED		0b0000.0000.0000.0000.0000.0000.0000.0100	0x0004	The item is to be drawn as disabled.
// ODS_CHECKED		0b0000.0000.0000.0000.0000.0000.0000.1000	0x0008	The menu item is to be checked. This bit is used only in a menu.
// ODS_FOCUS		0b0000.0000.0000.0000.0000.0000.0001.0000	0x0010	The item has the keyboard focus.
// ODS_DEFAULT		0b0000.0000.0000.0000.0000.0000.0010.0000	0x0020	The item is the default item.
// ODS_HOTLIGHT		0b0000.0000.0000.0000.0000.0000.0100.0000	0x0040	The item is being hot - tracked, that is, the item will be highlighted when the mouse is on the item.
// ODS_INACTIVE		0b0000.0000.0000.0000.0000.0000.1000.0000	0x0080	The item is inactiveand the window associated with the menu is inactive.
// ODS_NOACCEL		0b0000.0000.0000.0000.0000.0001.0000.0000	0x0100	The control is drawn without the keyboard accelerator cues.
// ODS_NOFOCUSRECT	0b0000.0000.0000.0000.0000.0010.0000.0000	0x0200	The control is drawn without focus indicator cues.
// ODS_COMBOBOXEDIT	0b0000.0000.0000.0000.0001.0000.0000.0000	0x1000	The drawing takes place in the selection field(edit control) of an owner - drawn combo box.

namespace mst::winapi::window::event::uahmenubar {

	#define WM_UAHDESTROYWINDOW    0x0090	// Handled by DefWindowProc
	#define WM_UAHDRAWMENU         0x0091	// messageL is UAHMENU
	#define WM_UAHDRAWMENUITEM     0x0092	// messageL is UAHDRAWMENUITEM
	#define WM_UAHINITMENU         0x0093	// handled by DefWindowProc
	#define WM_UAHMEASUREMENUITEM  0x0094	// messageL is UAHMEASUREMENUITEM
	#define WM_UAHNCPAINTMENUPOPUP 0x0095	// handled by DefWindowProc

	// On top of these the handling of drawing a menu requires the following defined calls:
	//  WM_THEMECHANGED
	//  WM_NCPAINT
	//  WM_NCACTIVATE

	enum class Event : uint32 {
		MeasureItem = WM_UAHMEASUREMENUITEM,
		PaintPopup = WM_UAHNCPAINTMENUPOPUP,
		DrawItem = WM_UAHDRAWMENUITEM,
		Destroy = WM_UAHDESTROYWINDOW,
		Intitialize = WM_UAHINITMENU,
		Draw = WM_UAHDRAWMENU
	};

	// describes the sizes of the menu bar or menu item
	typedef union tagUAHMENUITEMMETRICS {
		// cx appears to be 14 / 0xE less than rcItem's width!
		// cy 0x14 seems stable, i wonder if it is 4 less than rcItem's height which is always 24 atm
		struct {
			DWORD cx;
			DWORD cy;
		} rgsizeBar[2];
		struct {
			DWORD cx;
			DWORD cy;
		} rgsizePopup[4];
	} UAHMENUITEMMETRICS;

	// not really used in our case but part of the other structures
	typedef struct tagUAHMENUPOPUPMETRICS {
		DWORD rgcx[4];
		DWORD fUpdateMaxWidths : 2; // from kernel symbols, padded to full dword
	} UAHMENUPOPUPMETRICS;

	// hmenu is the main window menu; hdc is the context to draw in
	typedef struct tagUAHMENU {
		HMENU hmenu;
		HDC hdc;
		DWORD dwFlags; // no idea what these mean, in my testing it's either 0x00000a00 or sometimes 0x00000a10
	} UAHMENU;

	// menu items are always referred to by iPosition here
	typedef struct tagUAHMENUITEM {
		int iPosition; // 0-based position of menu item in menubar
		UAHMENUITEMMETRICS umim;
		UAHMENUPOPUPMETRICS umpm;
	} UAHMENUITEM;

	// the DRAWITEMSTRUCT contains the states of the menu items, as well as
	// the position index of the item in the menu, which is duplicated in
	// the UAHMENUITEM's iPosition as well
	typedef struct UAHDRAWMENUITEM {
		DRAWITEMSTRUCT dis; // itemID looks uninitialized
		UAHMENU um;
		UAHMENUITEM umi;
	} UAHDRAWMENUITEM;

	// the MEASUREITEMSTRUCT is intended to be filled with the size of the item
	// height appears to be ignored, but width can be modified
	typedef struct tagUAHMEASUREMENUITEM {
		MEASUREITEMSTRUCT mis;
		UAHMENU um;
		UAHMENUITEM umi;
	} UAHMEASUREMENUITEM;

	// No memory leaks here.
	HTHEME menuTheme = nullptr; 

	proceeded DrawMenu(const windowHandle& window, const UAHMENU& menu, const theme::solidBrush& brush) {
		MENUBARINFO menuBarInfo = { sizeof menuBarInfo };
		rect windowArea;

		GetMenuBarInfo(window, OBJID_MENU, 0, &menuBarInfo);
		GetWindowRect(window, &windowArea);
		{ // Creation of the actual rectangle representing our menu.
			rect& menuArea = menuBarInfo.rcBar;
			OffsetRect(&menuArea, -windowArea.left, -windowArea.top);

			FillRect(menu.hdc, &menuArea, brush.Get());
		}

		return proceeded::True;
	}

	proceeded MeasureMenuItem(UAHMEASUREMENUITEM& menuItemMeasure, const uint32& height, const uint32& width, const fontHandle& font = nullptr) {
		// We can modify it here to make it 1/3rd wider and higher for example.
		menuItemMeasure.mis.itemHeight = height;
		menuItemMeasure.mis.itemWidth = width;
		// Changing font via hdc
		// https://stackoverflow.com/questions/46185358/changing-font-of-a-text-in-win32
		return proceeded::True;
	}

	proceeded DrawMenuItem(
		const windowHandle& window, 
		UAHDRAWMENUITEM& menuItem,
		const theme::solidBrush& brush,
		const theme::solidBrush& brushSelected,
		const theme::solidBrush& brushHovered,
		const color& textColor
	) {
		mst::array_n::array<wchar, 256> menuText { L'\0' }; // Get the menu item string

		MENUITEMINFOW menuItemInfo { sizeof menuItemInfo, MIIM_STRING };
		uint32 dwFlags { DT_CENTER | DT_SINGLELINE | DT_VCENTER }; // Get the item state for drawing
		//uint32 iBackgroundStateID { 0 }, iTextStateID { 0 };

		menuItemInfo.dwTypeData = (LPWSTR)menuText.Pointer();
		menuItemInfo.cch = ((uint32)menuText.Length() / 2) - 1;

		GetMenuItemInfoW(menuItem.um.hmenu, menuItem.umi.iPosition, TRUE, &menuItemInfo);

		switch (menuItem.dis.itemState) {
			// Default, Inactive
			case 0x0020:
			case 0x0120:
			case 0x0080:
			case 0x0180:
				FillRect(menuItem.um.hdc, &menuItem.dis.rcItem, brush.Get());
				// Here would be the call to make the border underneeth reappear. as if wndproc can be dyn changed i would even propose a different function for such.
				//iBackgroundStateID = MPI_NORMAL;
				//iTextStateID = MPI_NORMAL;
				break;

			// Hot
			case 0x0040:
			case 0x0140:
				FillRect(menuItem.um.hdc, &menuItem.dis.rcItem, brushHovered.Get());
				//iBackgroundStateID = MPI_HOT;
				//iTextStateID = MPI_HOT;
				break;

			// Hot & Inactive
			case 0x00c0:
			case 0x01c0:
				FillRect(menuItem.um.hdc, &menuItem.dis.rcItem, brushHovered.Get());
				//iBackgroundStateID = MPI_HOT;
				//iTextStateID = MPI_HOT;
				break;

			// Select
			case 0x0001:
			case 0x0101:
				FillRect(menuItem.um.hdc, &menuItem.dis.rcItem, brushSelected.Get());
				// MENU_POPUPITEM has no state for this, though MENU_BARITEM does
				//iBackgroundStateID = MPI_HOT;
				//iTextStateID = MPI_HOT;
				break;

			// Disabled & Grayed
			case 0x0002:
			case 0x0102:
			case 0x0004:
			case 0x0104: {
				//iBackgroundStateID = MPI_DISABLED;
				//iTextStateID = MPI_DISABLED;
				if (!menuTheme) menuTheme = OpenThemeData(window, L"Menu");
				DTTOPTS opts { sizeof opts, DTT_TEXTCOLOR, RGB(0x40, 0x40, 0x40) };
				DrawThemeTextEx(menuTheme, menuItem.um.hdc, MENU_BARITEM, MBI_NORMAL, menuText.Pointer(), menuItemInfo.cch, dwFlags, &menuItem.dis.rcItem, &opts);
				return proceeded::True;
			} 

			// Other
			case 0x0100:
				FillRect(menuItem.um.hdc, &menuItem.dis.rcItem, brush.Get());
				dwFlags |= DT_HIDEPREFIX;
				break;

			default:
				MessageBoxExW(window, L"Unspecified menu item state.", L"Error", MB_OK, 0);
				break;

			
		}

		if (!menuTheme) menuTheme = OpenThemeData(window, L"Menu");
		DTTOPTS opts { sizeof opts, DTT_TEXTCOLOR, textColor };
		DrawThemeTextEx(menuTheme, menuItem.um.hdc, MENU_BARITEM, MBI_NORMAL, menuText.Pointer(), menuItemInfo.cch, dwFlags, &menuItem.dis.rcItem, &opts);
		return proceeded::True;
	}

	int64 DrawBottomLine(
		const windowHandle& window, 
		const displayContextHandle& displayContext, 
		const brushHandle& brush
	) {
		MENUBARINFO menuBarInfo { sizeof menuBarInfo };
		if (!GetMenuBarInfo(window, OBJID_MENU, 0, &menuBarInfo)) return 1; // Error Handling!

		{
			rect clientArea { 0 }, windowArea { 0 };
			GetClientRect(window, &clientArea);
			MapWindowPoints(window, nullptr, (point*)&clientArea, 2);
			GetWindowRect(window, &windowArea);

			// Creation of the actual rectangle representing our menu underline.
			OffsetRect(&clientArea, -windowArea.left, -windowArea.top);
			{
				rect& menuBarArea = clientArea;
				menuBarArea.bottom = menuBarArea.top;
				menuBarArea.top -= 1; // The height up.

				FillRect(displayContext, &menuBarArea, brush);
				ReleaseDC(window, displayContext);
			}
		} 
		return 0;
	}

	//void UAHDrawMenuNCBottomLine(HWND hWnd) {
	//    MENUBARINFO mbi = { sizeof(mbi) };
	//    if (!GetMenuBarInfo(hWnd, OBJID_MENU, 0, &mbi)) {
	//        return;
	//    }
	//    RECT rcClient = { 0 };
	//    GetClientRect(hWnd, &rcClient);
	//    MapWindowPoints(hWnd, nullptr, (POINT*)&rcClient, 2);
	//    RECT rcWindow = { 0 };
	//    GetWindowRect(hWnd, &rcWindow);
	//    OffsetRect(&rcClient, -rcWindow.left, -rcWindow.top);
	//    // the rcBar is offset by the window rect
	//    RECT rcAnnoyingLine = rcClient;
	//    rcAnnoyingLine.bottom = rcAnnoyingLine.top;
	//    rcAnnoyingLine.top--;
	//    HDC hdc = GetWindowDC(hWnd);
	//    FillRect(hdc, &rcAnnoyingLine, backgroundBrush);
	//    ReleaseDC(hWnd, hdc);
	//}
	//// processes messages related to UAH / custom menubar drawing.
	//// return true if handled, false to continue with normal processing in your wndproc
	//bool UAHWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam, LRESULT* lr, theme::theme theme, const theme::solidBrush& background, const theme::solidBrush& backgroundSelected, const theme::solidBrush& backgroundHovered) {
	//    switch (message) {
	//        case WM_UAHDRAWMENU: {
	//            UAHMENU* pUDM = (UAHMENU*)lParam;
	//            RECT rc = { 0 };
	//            // get the menubar rect
	//            {
	//                MENUBARINFO mbi = { sizeof mbi };
	//                GetMenuBarInfo(hWnd, OBJID_MENU, 0, &mbi);
	//                RECT rcWindow;
	//                GetWindowRect(hWnd, &rcWindow);
	//                // the rcBar is offset by the window rect
	//                rc = mbi.rcBar;
	//                OffsetRect(&rc, -rcWindow.left, -rcWindow.top);
	//            }
	//            FillRect((*pUDM).hdc, &rc, backgroundBrush);
	//            return true;
	//        }
	//        case WM_UAHDRAWMENUITEM: {
	//            UAHDRAWMENUITEM* pUDMI = (UAHDRAWMENUITEM*)lParam;
	//            // ugly colors for illustration purposes
	//            static HBRUSH g_brItemBackground = CreateSolidBrush(theme.backgroundSecondary); //CreateSolidBrush(RGB(0x00, 0x00, 0x00));
	//            static HBRUSH g_brItemBackgroundHot = CreateSolidBrush(RGB(0x4d, 0x4d, 0x4d));
	//            static HBRUSH g_brItemBackgroundSelected = CreateSolidBrush(RGB(0x67, 0x67, 0x67));
	//            HBRUSH* pbrBackground = &g_brItemBackground;
	//            // get the menu item string
	//            wchar_t menuString[256] = { 0 };
	//            MENUITEMINFO mii = { sizeof(mii), MIIM_STRING };
	//            {
	//                mii.dwTypeData = menuString;
	//                mii.cch = (sizeof(menuString) / 2) - 1;
	//                GetMenuItemInfo((*pUDMI).um.hmenu, (*pUDMI).umi.iPosition, TRUE, &mii);
	//            }
	//            // get the item state for drawing
	//            DWORD dwFlags = DT_CENTER | DT_SINGLELINE | DT_VCENTER;
	//            int iTextStateID = 0;
	//            int iBackgroundStateID = 0;
	//            {
	//                if (((*pUDMI).dis.itemState & ODS_INACTIVE) | ((*pUDMI).dis.itemState & ODS_DEFAULT)) {
	//                    // normal display
	//                    iTextStateID = MPI_NORMAL;
	//                    iBackgroundStateID = MPI_NORMAL;
	//                }
	//                if ((*pUDMI).dis.itemState & ODS_HOTLIGHT) {
	//                    // hot tracking
	//                    iTextStateID = MPI_HOT;
	//                    iBackgroundStateID = MPI_HOT;
	//                    pbrBackground = &g_brItemBackgroundHot;
	//                }
	//                if ((*pUDMI).dis.itemState & ODS_SELECTED) {
	//                    // clicked -- MENU_POPUPITEM has no state for this, though MENU_BARITEM does
	//                    iTextStateID = MPI_HOT;
	//                    iBackgroundStateID = MPI_HOT;
	//                    pbrBackground = &g_brItemBackgroundSelected;
	//                }
	//                if (((*pUDMI).dis.itemState & ODS_GRAYED) || ((*pUDMI).dis.itemState & ODS_DISABLED)) {
	//                    // disabled / grey text
	//                    iTextStateID = MPI_DISABLED;
	//                    iBackgroundStateID = MPI_DISABLED;
	//                }
	//                if ((*pUDMI).dis.itemState & ODS_NOACCEL) {
	//                    dwFlags |= DT_HIDEPREFIX;
	//                }
	//            }
	//            if (!menuTheme) {
	//                menuTheme = OpenThemeData(hWnd, L"Menu");
	//            }
	//            DTTOPTS opts = { sizeof opts, DTT_TEXTCOLOR, iTextStateID != MPI_DISABLED ? theme.textPrimary : RGB(0x40, 0x40, 0x40) }; //  : RGB(0x40, 0x40, 0x40)
	//            FillRect((*pUDMI).um.hdc, &(*pUDMI).dis.rcItem, *pbrBackground);
	//            DrawThemeTextEx(menuTheme, (*pUDMI).um.hdc, MENU_BARITEM, MBI_NORMAL, menuString, mii.cch, dwFlags, &(*pUDMI).dis.rcItem, &opts);
	//            return true;
	//        }
	//        case WM_UAHMEASUREMENUITEM: {
	//            UAHMEASUREMENUITEM* pMmi = (UAHMEASUREMENUITEM*)lParam;
	//            // allow the default window procedure to handle the message
	//            // since we don't really care about changing the width
	//            *lr = DefWindowProc(hWnd, message, wParam, lParam);
	//            // but we can modify it here to make it 1/3rd wider for example
	//            (*pMmi).mis.itemWidth = ((*pMmi).mis.itemWidth * 4) / 3;
	//            return true;
	//        }
	//        case WM_THEMECHANGED: {
	//            if (menuTheme) {
	//                CloseThemeData(menuTheme);
	//                menuTheme = nullptr;
	//            }
	//            // continue processing in main wndproc
	//            return false;
	//        }
	//        case WM_NCPAINT:
	//        case WM_NCACTIVATE:
	//            *lr = DefWindowProc(hWnd, message, wParam, lParam);
	//            UAHDrawMenuNCBottomLine(hWnd);
	//            return true;
	//            break;
	//        default:
	//            return false;
	//    }
	//}
}