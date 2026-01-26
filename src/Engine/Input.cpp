#include "EngineCore/System/Input.h"

bool Input::keysPressed[] = {};
bool Input::buttonsPressed[] = {};
KeyCode Input::lastKeyPressed = KeyCode::KEY_UNKNOWN;

std::string Input::getKeyString(KeyCode key)
{
	switch (key)
	{
	case KeyCode::KEY_SPACE:
		return "space";
	case KeyCode::KEY_APOSTROPHE:
		return "'";
	case KeyCode::KEY_COMMA:
		return "com";
	case KeyCode::KEY_MINUS:
		return "-";
	case KeyCode::KEY_PERIOD:
		return ".";
	case KeyCode::KEY_SLASH:
		return "/";
	case KeyCode::KEY_0:
		return "0";
	case KeyCode::KEY_1:
		return "1";
	case KeyCode::KEY_2:
		return "2";
	case KeyCode::KEY_3:
		return "3";
	case KeyCode::KEY_4:
		return "4";
	case KeyCode::KEY_5:
		return "5";
	case KeyCode::KEY_6:
		return "6";
	case KeyCode::KEY_7:
		return "7";
	case KeyCode::KEY_8:
		return "8";
	case KeyCode::KEY_9:
		return "9";
	case KeyCode::KEY_SEMICOLON:
		return ";";
	case KeyCode::KEY_EQUAL:
		return "=";
	case KeyCode::KEY_A:
		return "A";
	case KeyCode::KEY_B:
		return "B";
	case KeyCode::KEY_C:
		return "C";
	case KeyCode::KEY_D:
		return "D";
	case KeyCode::KEY_E:
		return "E";
	case KeyCode::KEY_F:
		return "F";
	case KeyCode::KEY_G:
		return "G";
	case KeyCode::KEY_H:
		return "H";
	case KeyCode::KEY_I:
		return "I";
	case KeyCode::KEY_J:
		return "J";
	case KeyCode::KEY_K:
		return "K";
	case KeyCode::KEY_L:
		return "L";
	case KeyCode::KEY_M:
		return "M";
	case KeyCode::KEY_N:
		return "N";
	case KeyCode::KEY_O:
		return "O";
	case KeyCode::KEY_P:
		return "P";
	case KeyCode::KEY_Q:
		return "Q";
	case KeyCode::KEY_R:
		return "R";
	case KeyCode::KEY_S:
		return "S";
	case KeyCode::KEY_T:
		return "T";
	case KeyCode::KEY_U:
		return "U";
	case KeyCode::KEY_V:
		return "V";
	case KeyCode::KEY_W:
		return "W";
	case KeyCode::KEY_X:
		return "X";
	case KeyCode::KEY_Y:
		return "Y";
	case KeyCode::KEY_Z:
		return "Z";
	case KeyCode::KEY_LEFT_BRACKET:
		return "[";
	case KeyCode::KEY_BACKSLASH:
		return "\\";
	case KeyCode::KEY_RIGHT_BRACKET:
		return "]";
	case KeyCode::KEY_GRAVE_ACCENT:
		return "`";
	case KeyCode::KEY_WORLD_1:
		break;
	case KeyCode::KEY_WORLD_2:
		break;
	case KeyCode::KEY_ESCAPE:
		return "esc";
	case KeyCode::KEY_ENTER:
		return "enter";
	case KeyCode::KEY_TAB:
		return "tab";
	case KeyCode::KEY_BACKSPACE:
		return "backspace";
	case KeyCode::KEY_INSERT:
		return "ins";
	case KeyCode::KEY_DELETE:
		return "del";
	case KeyCode::KEY_RIGHT:
		return "right";
	case KeyCode::KEY_LEFT:
		return "left";
	case KeyCode::KEY_DOWN:
		return "down";
	case KeyCode::KEY_UP:
		return "up";
	case KeyCode::KEY_PAGE_UP:
		return "pgup";
	case KeyCode::KEY_PAGE_DOWN:
		return "pgdown";
	case KeyCode::KEY_HOME:
		return "home";
	case KeyCode::KEY_END:
		return "end";
	case KeyCode::KEY_CAPS_LOCK:
		return "caps";
	case KeyCode::KEY_SCROLL_LOCK:
		return "scroll";
	case KeyCode::KEY_NUM_LOCK:
		return "num";
	case KeyCode::KEY_PRINT_SCREEN:
		return "print";
	case KeyCode::KEY_PAUSE:
		return "pause";
	case KeyCode::KEY_F1:
		return "F1";
	case KeyCode::KEY_F2:
		return "F2";
	case KeyCode::KEY_F3:
		return "F3";
	case KeyCode::KEY_F4:
		return "F4";
	case KeyCode::KEY_F5:
		return "F5";
	case KeyCode::KEY_F6:
		return "F6";
	case KeyCode::KEY_F7:
		return "F7";
	case KeyCode::KEY_F8:
		return "F8";
	case KeyCode::KEY_F9:
		return "F9";
	case KeyCode::KEY_F10:
		return "F10";
	case KeyCode::KEY_F11:
		return "F11";
	case KeyCode::KEY_F12:
		return "F12";
	case KeyCode::KEY_F13:
		return "F13";
	case KeyCode::KEY_F14:
		return "F14";
	case KeyCode::KEY_F15:
		return "F15";
	case KeyCode::KEY_F16:
		return "F16";
	case KeyCode::KEY_F17:
		return "F17";
	case KeyCode::KEY_F18:
		return "F18";
	case KeyCode::KEY_F19:
		return "F19";
	case KeyCode::KEY_F20:
		return "F20";
	case KeyCode::KEY_F21:
		return "F21";
	case KeyCode::KEY_F22:
		return "F22";
	case KeyCode::KEY_F23:
		return "F23";
	case KeyCode::KEY_F24:
		return "F24";
	case KeyCode::KEY_F25:
		return "F25";
	case KeyCode::KEY_KP_0:
		return "kp0";
	case KeyCode::KEY_KP_1:
		return "kp1";
	case KeyCode::KEY_KP_2:
		return "kp2";
	case KeyCode::KEY_KP_3:
		return "kp3";
	case KeyCode::KEY_KP_4:
		return "kp4";
	case KeyCode::KEY_KP_5:
		return "kp5";
	case KeyCode::KEY_KP_6:
		return "kp6";
	case KeyCode::KEY_KP_7:
		return "kp7";
	case KeyCode::KEY_KP_8:
		return "kp8";
	case KeyCode::KEY_KP_9:
		return "kp9";
	case KeyCode::KEY_KP_DECIMAL:
		return "/";
	case KeyCode::KEY_KP_DIVIDE:
		return "/";
	case KeyCode::KEY_KP_MULTIPLY:
		return "+";
	case KeyCode::KEY_KP_SUBTRACT:
		return "-";
	case KeyCode::KEY_KP_ADD:
		return "-";
	case KeyCode::KEY_KP_ENTER:
		return "kpenter";
	case KeyCode::KEY_KP_EQUAL:
		return "=";
	case KeyCode::KEY_LEFT_SHIFT:
		return "left shift";
	case KeyCode::KEY_LEFT_CONTROL:
		return "left ctrl";
	case KeyCode::KEY_LEFT_ALT:
		return "left alt";
	case KeyCode::KEY_LEFT_SUPER:
		return "left super";
	case KeyCode::KEY_RIGHT_SHIFT:
		return "right shift";
	case KeyCode::KEY_RIGHT_CONTROL:
		return "right ctrl";
	case KeyCode::KEY_RIGHT_ALT:
		return "right alt";
	case KeyCode::KEY_RIGHT_SUPER:
		return "tight super";
	case KeyCode::KEY_MENU:
		return "menu";
	case KeyCode::MOUSE_BUTTON_1:
		return "m-1";
	case KeyCode::MOUSE_BUTTON_2:
		return "m-2";
	case KeyCode::MOUSE_BUTTON_3:
		return "m-3";
	case KeyCode::MOUSE_BUTTON_4:
		return "m-4";
	case KeyCode::MOUSE_BUTTON_5:
		return "m-5";
	case KeyCode::MOUSE_BUTTON_6:
		return "m-6";
	case KeyCode::MOUSE_BUTTON_7:
		return "m-7";
	case KeyCode::MOUSE_BUTTON_8:
		return "m-8";
  default:
    return "unknown";
	}
}

bool Input::isKeyPressed(KeyCode key)
{
	return keysPressed[static_cast<size_t>(key)];
}

void Input::pressKey(KeyCode key)
{
	keysPressed[static_cast<size_t>(key)] = true;
	lastKeyPressed = key;
}

void Input::releaseKey(KeyCode key)
{
	keysPressed[static_cast<size_t>(key)] = false;
}

bool Input::isMouseButtonPressed(MouseButton key)
{
	return buttonsPressed[static_cast<size_t>(key)];
}

void Input::pressMouseButton(MouseButton key)
{
	buttonsPressed[static_cast<size_t>(key)] = true;
	lastKeyPressed = (KeyCode)key;
}

void Input::releaseMouseButton(MouseButton key)
{
	buttonsPressed[static_cast<size_t>(key)] = false;
}

KeyCode Input::getLastKeyPressed()
{
	return lastKeyPressed;
}
