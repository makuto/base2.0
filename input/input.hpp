#pragma once

// This is an evil dependency. Should probably get rid of it some time
// No, it's fine
#include "../graphics/graphics.hpp"
#include "../timer/timer.hpp"

#include <map>
#include <vector>

// Using a namespace because scoping isn't standard in GCC (or C++)
// See http://stackoverflow.com/questions/5188554/my-enum-is-not-a-class-or-namespace
namespace inputCode
{
// Enums for key codes
// Keyboard codes (for input independence)
// An *almost* direct copy of SFML 2.0 sf::Key (see /usr/local/include/SFML/Window/Keyboard.hpp)
enum keyCode : int
{
	A = 0,      ///< The A key
	B,          ///< The B key
	C,          ///< The C key
	D,          ///< The D key
	E,          ///< The E key
	F,          ///< The F key
	G,          ///< The G key
	H,          ///< The H key
	I,          ///< The I key
	J,          ///< The J key
	K,          ///< The K key
	L,          ///< The L key
	M,          ///< The M key
	N,          ///< The N key
	O,          ///< The O key
	P,          ///< The P key
	Q,          ///< The Q key
	R,          ///< The R key
	S,          ///< The S key
	T,          ///< The T key
	U,          ///< The U key
	V,          ///< The V key
	W,          ///< The W key
	X,          ///< The X key
	Y,          ///< The Y key
	Z,          ///< The Z key
	Num0,       ///< The 0 key
	Num1,       ///< The 1 key
	Num2,       ///< The 2 key
	Num3,       ///< The 3 key
	Num4,       ///< The 4 key
	Num5,       ///< The 5 key
	Num6,       ///< The 6 key
	Num7,       ///< The 7 key
	Num8,       ///< The 8 key
	Num9,       ///< The 9 key
	Escape,     ///< The Escape key
	LControl,   ///< The left Control key
	LShift,     ///< The left Shift key
	LAlt,       ///< The left Alt key
	LSystem,    ///< The left OS specific key: window (Windows and Linux), apple (MacOS X), ...
	RControl,   ///< The right Control key
	RShift,     ///< The right Shift key
	RAlt,       ///< The right Alt key
	RSystem,    ///< The right OS specific key: window (Windows and Linux), apple (MacOS X), ...
	Menu,       ///< The Menu key
	LBracket,   ///< The [ key
	RBracket,   ///< The ] key
	SemiColon,  ///< The ; key
	Comma,      ///< The , key
	Period,     ///< The . key
	Quote,      ///< The ' key
	Slash,      ///< The / key
	BackSlash,  ///< The \ key
	Tilde,      ///< The ~ key
	Equal,      ///< The = key
	Dash,       ///< The - key
	Space,      ///< The Space key
	Return,     ///< The Return key
	BackSpace,  ///< The Backspace key
	Tab,        ///< The Tabulation key
	PageUp,     ///< The Page up key
	PageDown,   ///< The Page down key
	End,        ///< The End key
	Home,       ///< The Home key
	Insert,     ///< The Insert key
	Delete,     ///< The Delete key
	Add,        ///< The + key
	Subtract,   ///< The - key
	Multiply,   ///< The * key
	Divide,     ///< The / key
	Left,       ///< Left arrow
	Right,      ///< Right arrow
	Up,         ///< Up arrow
	Down,       ///< Down arrow
	Numpad0,    ///< The numpad 0 key
	Numpad1,    ///< The numpad 1 key
	Numpad2,    ///< The numpad 2 key
	Numpad3,    ///< The numpad 3 key
	Numpad4,    ///< The numpad 4 key
	Numpad5,    ///< The numpad 5 key
	Numpad6,    ///< The numpad 6 key
	Numpad7,    ///< The numpad 7 key
	Numpad8,    ///< The numpad 8 key
	Numpad9,    ///< The numpad 9 key
	F1,         ///< The F1 key
	F2,         ///< The F2 key
	F3,         ///< The F3 key
	F4,         ///< The F4 key
	F5,         ///< The F5 key
	F6,         ///< The F6 key
	F7,         ///< The F7 key
	F8,         ///< The F8 key
	F9,         ///< The F9 key
	F10,        ///< The F10 key
	F11,        ///< The F11 key
	F12,        ///< The F12 key
	F13,        ///< The F13 key
	F14,        ///< The F14 key
	F15         ///< The F15 key
};
// Mouse codes
enum mouseCode : int
{
	LeftMouse = 1000,
	RightMouse,
	MiddleMouse
};
// Mouse position codes
enum mousePosition : int
{
	MouseX = 2000,
	MouseY
};
}  // namespace inputCode

class inputManager
{
private:
	sf::RenderWindow* sfWin;
	window* win;

	struct InputState
	{
		bool ProcessedTapped;
		float KeyRepeatTime;
	};

	std::map<inputCode::keyCode, InputState> keyTapStates;

	timer repeatTimer;

	float KeyRepeatPauseTime;
	float KeyRepeatTimePerRepeat;
	void initializeKeyRepeat();
	
	bool UpdateRepeat(InputState& state, bool isPressed);
	
public:
	// Use inputManager(window*) over lib-specific functions
	inputManager(sf::RenderWindow*);
	inputManager(window*);
	inputManager();
	void setWindow(sf::RenderWindow*);
	void setWindow(window*);
	int getMouseX();
	int getMouseY();
	// Universal button function. Call if you want to use both mouse
	// and keyboard together (more abstract)
	bool isPressed(int code);
	// Sadly, library dependant. Returns whether or not a key is pressed
	bool isPressed(inputCode::keyCode);
	// Note that this function is now deprecated; use isPressed(inputCode::keyCode) instead
	bool isPressed(sf::Keyboard::Key);

	// Use these functions if you need lib-specific codes (and these are used internally)
	sf::Keyboard::Key convertKeyCode(inputCode::keyCode);
	sf::Mouse::Button convertMouseCode(inputCode::mouseCode);
	// Returns whether or not a mouse key is pressed
	bool isMousePressed(inputCode::mouseCode);
	// Note that this function is now deprecated; use isMousePressed(inputCode::keyCode) instead
	bool isMousePressed(sf::Mouse::Button);

	// Universal input function. Call if you want to use mouse and keyboard
	// buttons or even return mouse position
	int getState(int code);

	// Tapped with key repeat
	bool WasTapped(inputCode::keyCode key, bool enableKeyRepeat);
	// Arrow keys and numpad directional navigation
	void GetInpDirectionalNavigation(int& deltaX, int& deltaY);
	// Top row numbers only
	std::vector<int> GetInpNumbersTapped();
};
