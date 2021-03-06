#include "input.hpp"
#include <iostream>

void inputManager::initializeKeyRepeat()
{
	repeatTimer.start();
	KeyRepeatPauseTime = .500f;
	KeyRepeatTimePerRepeat = .1f;
}

inputManager::inputManager(sf::RenderWindow* newWin)
{
	sfWin = newWin;
	initializeKeyRepeat();
}

inputManager::inputManager(window* newWin)
{
	sfWin = newWin->getBase();
	win = newWin;
	initializeKeyRepeat();
}

inputManager::inputManager()
{
	sfWin = NULL;
	initializeKeyRepeat();
}

void inputManager::setWindow(sf::RenderWindow* newWin)
{
	sfWin = newWin;
}

void inputManager::setWindow(window* newWin)
{
	sfWin = newWin->getBase();
	win = newWin;
}

int inputManager::getMouseX()
{
	return sf::Mouse::getPosition(*sfWin).x;
}

int inputManager::getMouseY()
{
	return sf::Mouse::getPosition(*sfWin).y;
}

// Universal button function. Call if you want to use both mouse
// and keyboard together (more abstract)
bool inputManager::isPressed(int code)
{
	if (win && !win->isFocused())
		return false;
	// Mouse code (they start at 1000)
	if (code >= 1000)
	{
		inputCode::mouseCode mouseButton = static_cast<inputCode::mouseCode>(code);
		return isMousePressed(mouseButton);
	}
	// Keyboard code (they start at 0)
	inputCode::keyCode keyButton = static_cast<inputCode::keyCode>(code);
	return isPressed(keyButton);
}

bool inputManager::isPressed(sf::Keyboard::Key key)
{
	return sf::Keyboard::isKeyPressed(key);
}

bool inputManager::isPressed(inputCode::keyCode key)
{
	if (win && !win->isFocused())
		return false;
	return sf::Keyboard::isKeyPressed(convertKeyCode(key));
}

bool inputManager::isMousePressed(sf::Mouse::Button button)
{
	return sf::Mouse::isButtonPressed(button);
}

bool inputManager::isMousePressed(inputCode::mouseCode button)
{
	if (win && !win->isFocused())
		return false;
	return sf::Mouse::isButtonPressed(convertMouseCode(button));
}

sf::Keyboard::Key inputManager::convertKeyCode(inputCode::keyCode key)
{
	sf::Keyboard::Key newKey = sf::Keyboard::Unknown;
	//:(
	switch (key)
	{
		case inputCode::A:
			newKey = sf::Keyboard::A;
			break;
		case inputCode::B:
			newKey = sf::Keyboard::B;
			break;
		case inputCode::C:
			newKey = sf::Keyboard::C;
			break;
		case inputCode::D:
			newKey = sf::Keyboard::D;
			break;
		case inputCode::E:
			newKey = sf::Keyboard::E;
			break;
		case inputCode::F:
			newKey = sf::Keyboard::F;
			break;
		case inputCode::G:
			newKey = sf::Keyboard::G;
			break;
		case inputCode::H:
			newKey = sf::Keyboard::H;
			break;
		case inputCode::I:
			newKey = sf::Keyboard::I;
			break;  ///< The I key
		case inputCode::J:
			newKey = sf::Keyboard::J;
			break;  ///< The J key
		case inputCode::K:
			newKey = sf::Keyboard::K;
			break;  ///< The K key
		case inputCode::L:
			newKey = sf::Keyboard::L;
			break;  ///< The L key
		case inputCode::M:
			newKey = sf::Keyboard::M;
			break;  ///< The M key
		case inputCode::N:
			newKey = sf::Keyboard::N;
			break;  ///< The N key
		case inputCode::O:
			newKey = sf::Keyboard::O;
			break;  ///< The O key
		case inputCode::P:
			newKey = sf::Keyboard::P;
			break;  ///< The P key
		case inputCode::Q:
			newKey = sf::Keyboard::Q;
			break;  ///< The Q key
		case inputCode::R:
			newKey = sf::Keyboard::R;
			break;  ///< The R key
		case inputCode::S:
			newKey = sf::Keyboard::S;
			break;  ///< The S key
		case inputCode::T:
			newKey = sf::Keyboard::T;
			break;  ///< The T key
		case inputCode::U:
			newKey = sf::Keyboard::U;
			break;  ///< The U key
		case inputCode::V:
			newKey = sf::Keyboard::V;
			break;  ///< The V key
		case inputCode::W:
			newKey = sf::Keyboard::W;
			break;  ///< The W key
		case inputCode::X:
			newKey = sf::Keyboard::X;
			break;  ///< The X key
		case inputCode::Y:
			newKey = sf::Keyboard::Y;
			break;  ///< The Y key
		case inputCode::Z:
			newKey = sf::Keyboard::Z;
			break;  ///< The Z key
		case inputCode::Num0:
			newKey = sf::Keyboard::Num0;
			break;  ///< The 0 key
		case inputCode::Num1:
			newKey = sf::Keyboard::Num1;
			break;  ///< The 1 key
		case inputCode::Num2:
			newKey = sf::Keyboard::Num2;
			break;  ///< The 2 key
		case inputCode::Num3:
			newKey = sf::Keyboard::Num3;
			break;  ///< The 3 key
		case inputCode::Num4:
			newKey = sf::Keyboard::Num4;
			break;  ///< The 4 key
		case inputCode::Num5:
			newKey = sf::Keyboard::Num5;
			break;  ///< The 5 key
		case inputCode::Num6:
			newKey = sf::Keyboard::Num6;
			break;  ///< The 6 key
		case inputCode::Num7:
			newKey = sf::Keyboard::Num7;
			break;  ///< The 7 key
		case inputCode::Num8:
			newKey = sf::Keyboard::Num8;
			break;  ///< The 8 key
		case inputCode::Num9:
			newKey = sf::Keyboard::Num9;
			break;  ///< The 9 key
		case inputCode::Escape:
			newKey = sf::Keyboard::Escape;
			break;  ///< The Escape key
		case inputCode::LControl:
			newKey = sf::Keyboard::LControl;
			break;  ///< The left Control key
		case inputCode::LShift:
			newKey = sf::Keyboard::LShift;
			break;  ///< The left Shift key
		case inputCode::LAlt:
			newKey = sf::Keyboard::LAlt;
			break;  ///< The left Alt key
		case inputCode::LSystem:
			newKey = sf::Keyboard::LSystem;
			break;  ///< The left OS specific key: window (Windows and Linux): apple (MacOS X): ...
		case inputCode::RControl:
			newKey = sf::Keyboard::RControl;
			break;  ///< The right Control key
		case inputCode::RShift:
			newKey = sf::Keyboard::RShift;
			break;  ///< The right Shift key
		case inputCode::RAlt:
			newKey = sf::Keyboard::RAlt;
			break;  ///< The right Alt key
		case inputCode::RSystem:
			newKey = sf::Keyboard::RSystem;
			break;  ///< The right OS specific key: window (Windows and Linux): apple (MacOS X): ...
		case inputCode::Menu:
			newKey = sf::Keyboard::Menu;
			break;  ///< The Menu key
		case inputCode::LBracket:
			newKey = sf::Keyboard::LBracket;
			break;  ///< The [ key
		case inputCode::RBracket:
			newKey = sf::Keyboard::RBracket;
			break;  ///< The ] key
		case inputCode::SemiColon:
			newKey = sf::Keyboard::SemiColon;
			break;  ///< The ; key
		case inputCode::Comma:
			newKey = sf::Keyboard::Comma;
			break;  ///< The : key
		case inputCode::Period:
			newKey = sf::Keyboard::Period;
			break;  ///< The . key
		case inputCode::Quote:
			newKey = sf::Keyboard::Quote;
			break;  ///< The ' key
		case inputCode::Slash:
			newKey = sf::Keyboard::Slash;
			break;  ///< The / key
		case inputCode::BackSlash:
			newKey = sf::Keyboard::BackSlash;
			break;  ///< The \ key
		case inputCode::Tilde:
			newKey = sf::Keyboard::Tilde;
			break;  ///< The ~ key
		case inputCode::Equal:
			newKey = sf::Keyboard::Equal;
			break;  ///< The = key
		case inputCode::Dash:
			newKey = sf::Keyboard::Dash;
			break;  ///< The - key
		case inputCode::Space:
			newKey = sf::Keyboard::Space;
			break;  ///< The Space key
		case inputCode::Return:
			newKey = sf::Keyboard::Return;
			break;  ///< The Return key
		case inputCode::BackSpace:
			newKey = sf::Keyboard::BackSpace;
			break;  ///< The Backspace key
		case inputCode::Tab:
			newKey = sf::Keyboard::Tab;
			break;  ///< The Tabulation key
		case inputCode::PageUp:
			newKey = sf::Keyboard::PageUp;
			break;  ///< The Page up key
		case inputCode::PageDown:
			newKey = sf::Keyboard::PageDown;
			break;  ///< The Page down key
		case inputCode::End:
			newKey = sf::Keyboard::End;
			break;  ///< The End key
		case inputCode::Home:
			newKey = sf::Keyboard::Home;
			break;
		case inputCode::Insert:
			newKey = sf::Keyboard::Insert;
			break;  ///< The Insert key
		case inputCode::Delete:
			newKey = sf::Keyboard::Delete;
			break;  ///< The Delete key
		case inputCode::Add:
			newKey = sf::Keyboard::Add;
			break;  ///< The + key
		case inputCode::Subtract:
			newKey = sf::Keyboard::Subtract;
			break;  ///< The - key
		case inputCode::Multiply:
			newKey = sf::Keyboard::Multiply;
			break;  ///< The * key
		case inputCode::Divide:
			newKey = sf::Keyboard::Divide;
			break;  ///< The / key
		case inputCode::Left:
			newKey = sf::Keyboard::Left;
			break;  ///< Left arrow
		case inputCode::Right:
			newKey = sf::Keyboard::Right;
			break;  ///< Right arrow
		case inputCode::Up:
			newKey = sf::Keyboard::Up;
			break;  ///< Up arrow
		case inputCode::Down:
			newKey = sf::Keyboard::Down;
			break;  ///< Down arrow
		case inputCode::Numpad0:
			newKey = sf::Keyboard::Numpad0;
			break;  ///< The numpad 0 key
		case inputCode::Numpad1:
			newKey = sf::Keyboard::Numpad1;
			break;  ///< The numpad 1 key
		case inputCode::Numpad2:
			newKey = sf::Keyboard::Numpad2;
			break;  ///< The numpad 2 key
		case inputCode::Numpad3:
			newKey = sf::Keyboard::Numpad3;
			break;  ///< The numpad 3 key
		case inputCode::Numpad4:
			newKey = sf::Keyboard::Numpad4;
			break;  ///< The numpad 4 key
		case inputCode::Numpad5:
			newKey = sf::Keyboard::Numpad5;
			break;  ///< The numpad 5 key
		case inputCode::Numpad6:
			newKey = sf::Keyboard::Numpad6;
			break;  ///< The numpad 6 key
		case inputCode::Numpad7:
			newKey = sf::Keyboard::Numpad7;
			break;  ///< The numpad 7 key
		case inputCode::Numpad8:
			newKey = sf::Keyboard::Numpad8;
			break;  ///< The numpad 8 key
		case inputCode::Numpad9:
			newKey = sf::Keyboard::Numpad9;
			break;  ///< The numpad 9 key
		case inputCode::F1:
			newKey = sf::Keyboard::F1;
			break;  ///< The F1 key
		case inputCode::F2:
			newKey = sf::Keyboard::F2;
			break;  ///< The F2 key
		case inputCode::F3:
			newKey = sf::Keyboard::F3;
			break;  ///< The F3 key
		case inputCode::F4:
			newKey = sf::Keyboard::F4;
			break;  ///< The F4 key
		case inputCode::F5:
			newKey = sf::Keyboard::F5;
			break;  ///< The F5 key
		case inputCode::F6:
			newKey = sf::Keyboard::F6;
			break;  ///< The F6 key
		case inputCode::F7:
			newKey = sf::Keyboard::F7;
			break;  ///< The F7 key
		case inputCode::F8:
			newKey = sf::Keyboard::F8;
			break;  ///< The F8 key
		case inputCode::F9:
			newKey = sf::Keyboard::F9;
			break;  ///< The F9 key
		case inputCode::F10:
			newKey = sf::Keyboard::F10;
			break;  ///< The F10 key
		case inputCode::F11:
			newKey = sf::Keyboard::F11;
			break;  ///< The F11 key
		case inputCode::F12:
			newKey = sf::Keyboard::F12;
			break;  ///< The F12 key
		case inputCode::F13:
			newKey = sf::Keyboard::F13;
			break;  ///< The F13 key
		case inputCode::F14:
			newKey = sf::Keyboard::F14;
			break;  ///< The F14 key
		case inputCode::F15:
			newKey = sf::Keyboard::F15;
			break;
		default:
			break;
	};
	return newKey;
};

sf::Mouse::Button inputManager::convertMouseCode(inputCode::mouseCode button)
{
	// Default left click
	sf::Mouse::Button newButton = sf::Mouse::Left;
	switch (button)
	{
		case inputCode::LeftMouse:
			newButton = sf::Mouse::Left;
			break;
		case inputCode::RightMouse:
			newButton = sf::Mouse::Right;
			break;
		case inputCode::MiddleMouse:
			newButton = sf::Mouse::Middle;
			break;
		default:
			break;
	}
	return newButton;
}

// Universal input function. Call if you want to use mouse and keyboard
// buttons or even return mouse position
int inputManager::getState(int code)
{
	if (code >= 2000)  // Mouse position code
	{
		switch (code)
		{
			case inputCode::mousePosition::MouseX:
				return getMouseX();
				break;
			case inputCode::mousePosition::MouseY:
				return getMouseY();
				break;
			default:
				break;
		}
	}
	else  // Button code
	{
		return isPressed(code);
	}
	// Input code not recognized
	std::cout << "WARNING: inputManager.getState(): Input code " << code
	          << " not recognized! Returning 0\n";
	return 0;
}

bool inputManager::UpdateRepeat(InputState& state, bool currentlyPressed)
{
	float currentTime = repeatTimer.getTime();
	if (currentlyPressed)
	{
		if (state.KeyRepeatTime)
		{
			if (state.KeyRepeatTime <= currentTime)
			{
				state.KeyRepeatTime = currentTime + KeyRepeatTimePerRepeat;
				return true;
			}
			else
				return false;
		}
		else
		{
			state.KeyRepeatTime = currentTime + KeyRepeatPauseTime;
			return false;
		}
	}

	state.KeyRepeatTime = 0.f;

	return false;
}

bool inputManager::WasTapped(inputCode::keyCode key, bool enableKeyRepeat)
{
	bool currentlyPressed = isPressed(key);

	std::map<inputCode::keyCode, InputState>::iterator findIt = keyTapStates.find(key);
	if (findIt != keyTapStates.end())
	{
		bool shouldRepeat = UpdateRepeat(findIt->second, currentlyPressed);
		if (currentlyPressed)
		{
			if (findIt->second.ProcessedTapped)
			{
				if (shouldRepeat && enableKeyRepeat)
					return true;
				// Wasn't tapped this frame
				return false;
			}
			else
			{
				findIt->second.ProcessedTapped = true;
				return true;
			}
		}
		else
		{
			findIt->second.ProcessedTapped = false;
			findIt->second.KeyRepeatTime = 0.f;
		}
	}
	else
	{
		keyTapStates[key] = {true, 0.f};
		return currentlyPressed;
	}

	return false;
}

void inputManager::GetInpDirectionalNavigation(int& deltaX, int& deltaY)
{
	if (WasTapped(inputCode::Up, true) || WasTapped(inputCode::Numpad8, true))
		deltaY = -1;
	if (WasTapped(inputCode::Down, true) || WasTapped(inputCode::Numpad2, true))
		deltaY = 1;
	if (WasTapped(inputCode::Left, true) || WasTapped(inputCode::Numpad4, true))
		deltaX = -1;
	if (WasTapped(inputCode::Right, true) || WasTapped(inputCode::Numpad6, true))
		deltaX = 1;
	if (WasTapped(inputCode::Numpad9, true))
	{
		deltaX = 1;
		deltaY = -1;
	}
	if (WasTapped(inputCode::Numpad3, true))
	{
		deltaX = 1;
		deltaY = 1;
	}
	if (WasTapped(inputCode::Numpad1, true))
	{
		deltaX = -1;
		deltaY = 1;
	}
	if (WasTapped(inputCode::Numpad7, true))
	{
		deltaX = -1;
		deltaY = -1;
	}
}

std::vector<int> inputManager::GetInpNumbersTapped()
{
	std::vector<int> numbersTapped;
	if (WasTapped(inputCode::Num0, false))
		numbersTapped.push_back(0);
	if (WasTapped(inputCode::Num1, false))
		numbersTapped.push_back(1);
	if (WasTapped(inputCode::Num2, false))
		numbersTapped.push_back(2);
	if (WasTapped(inputCode::Num3, false))
		numbersTapped.push_back(3);
	if (WasTapped(inputCode::Num4, false))
		numbersTapped.push_back(4);
	if (WasTapped(inputCode::Num5, false))
		numbersTapped.push_back(5);
	if (WasTapped(inputCode::Num6, false))
		numbersTapped.push_back(6);
	if (WasTapped(inputCode::Num7, false))
		numbersTapped.push_back(7);
	if (WasTapped(inputCode::Num8, false))
		numbersTapped.push_back(8);
	if (WasTapped(inputCode::Num9, false))
		numbersTapped.push_back(9);

	return numbersTapped;
}
