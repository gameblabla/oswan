#include <windows.h>

static LPCTSTR keyName[] = {
	TEXT("???"),
	TEXT("[ESC]"),
	TEXT("1"),
	TEXT("2"),
	TEXT("3"),
	TEXT("4"),
	TEXT("5"),
	TEXT("6"),
	TEXT("7"),
	TEXT("8"),
	TEXT("9"),
	TEXT("0"),
	TEXT("-"),              // - on main keyboard
	TEXT("^"),
	TEXT("[BS]"),           // backspace
	TEXT("[TAB]"),
	TEXT("Q"),
	TEXT("W"),
	TEXT("E"),
	TEXT("R"),
	TEXT("T"),
	TEXT("Y"),
	TEXT("U"),
	TEXT("I"),
	TEXT("O"),
	TEXT("P"),
	TEXT("["),
	TEXT("]"),
	TEXT("[ENTER]"),        // Enter on main keyboard
	TEXT("[LCTRL]"),
	TEXT("A"),
	TEXT("S"),
	TEXT("D"),
	TEXT("F"),
	TEXT("G"),
	TEXT("H"),
	TEXT("J"),
	TEXT("K"),
	TEXT("L"),
	TEXT(";"),
	TEXT(":"),
	TEXT("]"),              // accent grave
	TEXT("[LSHIFT]"),
	TEXT("\\"),
	TEXT("Z"),
	TEXT("X"),
	TEXT("C"),
	TEXT("V"),
	TEXT("B"),
	TEXT("N"),
	TEXT("M"),
	TEXT(","),
	TEXT("."),              // . on main keyboard
	TEXT("/"),              // / on main keyboard
	TEXT("[RSHIFT]"),
	TEXT("[*]"),            // * on numeric keypad
	TEXT("[LALT]"),         // left Alt
	TEXT("[SPACE]"),
	TEXT("[CAPS]"),
	TEXT("[F1]"),
	TEXT("[F2]"),
	TEXT("[F3]"),
	TEXT("[F4]"),
	TEXT("[F5]"),
	TEXT("[F6]"),
	TEXT("[F7]"),
	TEXT("[F8]"),
	TEXT("[F9]"),
	TEXT("[F10]"),
	TEXT("[NUMLOCK]"),
	TEXT("[SCRLLK]"),       // Scroll Lock
	TEXT("[7]"),
	TEXT("[8]"),
	TEXT("[9]"),
	TEXT("[-]"),            // - on numeric keypad
	TEXT("[4]"),
	TEXT("[5]"),
	TEXT("[6]"),
	TEXT("[+]"),            // + on numeric keypad
	TEXT("[1]"),
	TEXT("[2]"),
	TEXT("[3]"),
	TEXT("[0]"),
	TEXT("[.]"),            // . on numeric keypad
	TEXT("???"),
	TEXT("???"),
	TEXT("???"),            // < > | on UK/Germany keyboards
	TEXT("[F11]"),
	TEXT("[F12]"),
	TEXT("???"),
	TEXT("???"),
	TEXT("???"),
	TEXT("???"),
	TEXT("???"),
	TEXT("???"),
	TEXT("???"),
	TEXT("???"),
	TEXT("???"),
	TEXT("???"),
	TEXT("???"),
	TEXT("[F13]"),          // (NEC PC98)
	TEXT("[F14]"),          // (NEC PC98)
	TEXT("[F15]"),          // (NEC PC98)
	TEXT("???"),
	TEXT("???"),
	TEXT("???"),
	TEXT("???"),
	TEXT("???"),
	TEXT("???"),
	TEXT("???"),
	TEXT("???"),
	TEXT("???"),
	TEXT("[KANA]"),         // (Japanese keyboard)
	TEXT("???"),
	TEXT("???"),
	TEXT("???"),            // / ? on Portugese (Brazilian) keyboards
	TEXT("???"),
	TEXT("???"),
	TEXT("???"),
	TEXT("???"),
	TEXT("???"),
	TEXT("[HENKAN]"),       // (Japanese keyboard)
	TEXT("???"),
	TEXT("[MUHENKAN]"),     // (Japanese keyboard)
	TEXT("???"),
	TEXT("\\"),             // (Japanese keyboard)
	TEXT("???"),            // Numpad . on Portugese (Brazilian) keyboards
	TEXT("???"),
	TEXT("???"),
	TEXT("???"),
	TEXT("???"),
	TEXT("???"),
	TEXT("???"),
	TEXT("???"),
	TEXT("???"),
	TEXT("???"),
	TEXT("???"),
	TEXT("???"),
	TEXT("???"),
	TEXT("???"),
	TEXT("???"),
	TEXT("[=]"),            // = on numeric keypad (NEC PC98)
	TEXT("???"),
	TEXT("???"),
	TEXT("^"),              // Previous Track (DIK_CIRCUMFLEX on Japanese keyboard)
	TEXT("@"),              // (NEC PC98)
	TEXT(":"),              // (NEC PC98)
	TEXT("[UNDERLINE]"),    // (NEC PC98)
	TEXT("[KANJI]"),        // (Japanese keyboard)
	TEXT("[STOP]"),         // (NEC PC98)
	TEXT("[AX]"),           // (Japan AX)
	TEXT("[UNLABELED]"),    // (J3100)
	TEXT("???"),
	TEXT("[JIKOUHO]"),      // Next Track
	TEXT("???"),
	TEXT("???"),
	TEXT("[enter]"),        // Enter on numeric keypad
	TEXT("[RCTRL]"),
	TEXT("???"),
	TEXT("???"),
	TEXT("[MUTE]"),         // Mute
	TEXT("[CALCULATOR]"),   // Calculator
	TEXT("[PLAYPAUSE]"),    // Play / Pause
	TEXT("???"),
	TEXT("[MEDIASTOP]"),    // Media Stop
	TEXT("???"),
	TEXT("???"),
	TEXT("???"),
	TEXT("???"),
	TEXT("???"),
	TEXT("???"),
	TEXT("???"),
	TEXT("???"),
	TEXT("???"),
	TEXT("[VOLUME-]"),      // Volume -
	TEXT("???"),
	TEXT("[VOLUME+]"),      // Volume +
	TEXT("???"),
	TEXT("[WEBHOME]"),      // Web home
	TEXT("[),]"),            // ), on numeric keypad (NEC PC98)
	TEXT("???"),
	TEXT("[/]"),            // / on numeric keypad
	TEXT("???"),
	TEXT("[SYSRQ]"),
	TEXT("[RALT]"),         // right Alt
	TEXT("???"),
	TEXT("???"),
	TEXT("???"),
	TEXT("???"),
	TEXT("???"),
	TEXT("???"),
	TEXT("???"),
	TEXT("???"),
	TEXT("???"),
	TEXT("???"),
	TEXT("???"),
	TEXT("???"),
	TEXT("[PAUSE]"),        // Pause
	TEXT("???"),
	TEXT("[HOME]"),         // Home on arrow keypad
	TEXT("[UP]"),           // UpArrow on arrow keypad
	TEXT("[PgUP]"),         // PgUp on arrow keypad
	TEXT("???"),
	TEXT("[LEFT]"),         // LeftArrow on arrow keypad
	TEXT("???"),
	TEXT("[RIGHT]"),        // RightArrow on arrow keypad
	TEXT("???"),
	TEXT("[END]"),          // End on arrow keypad
	TEXT("[DOWN]"),         // DownArrow on arrow keypad
	TEXT("[PgDn]"),         // PgDn on arrow keypad
	TEXT("[INSERT]"),       // Insert on arrow keypad
	TEXT("[DELETE]"),       // Delete on arrow keypad
	TEXT("???"),
	TEXT("???"),
	TEXT("???"),
	TEXT("???"),
	TEXT("???"),
	TEXT("???"),
	TEXT("???"),
	TEXT("[LWIN]"),         // Left Windows key
	TEXT("[RWIN]"),         // Right Windows key
	TEXT("[APPS]"),         // AppMenu key
	TEXT("[POWER]"),        // System Power
	TEXT("[SLEEP]"),        // System Sleep
	TEXT("???"),
	TEXT("???"),
	TEXT("???"),
	TEXT("[WAKE]"),         // System Wake
	TEXT("???"),
	TEXT("[WEBSEARCH]"),    // Web Search
	TEXT("[WEBFAVORITES]"), // Web Favorites
	TEXT("[WEBREFRESH]"),   // Web Refresh
	TEXT("[WEBSTOP]"),      // Web Stop
	TEXT("[WEBFORWARD]"),   // Web Forward
	TEXT("[WEBBACK]"),      // Web Back
	TEXT("[MYCOMPUTER]"),   // My Computer
	TEXT("[MAIL]"),         // Mail
	TEXT("[MEDIASELECT]")   // Media Select
};
