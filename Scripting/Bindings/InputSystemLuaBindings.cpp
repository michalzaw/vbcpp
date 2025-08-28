#include "InputSystemLuaBindings.h"

#include "../Utils/LuaUtils.h"

// Include GLU header with GLFW
#define GLFW_INCLUDE_GLU
#include <GLFW/glfw3.h>

#include "../../Utils/InputSystem.h"
#include "../../Utils/Logger.h"


namespace InputSystemLuaBindings
{
	void bind(sol::state* lua)
	{
		lua->new_usertype<InputSystem>("InputSystem",
			"isKeyDown", &InputSystem::isKeyDown,
			"isKeyPressed", &InputSystem::isKeyPressed,
			"isKeyReleased", &InputSystem::isKeyReleased,
			"isMouseButtonPressed", &InputSystem::isMouseButtonPressed,
			"isMouseButtonReleased", &InputSystem::isMouseButtonReleased,
			"getScrollOffsetX", &InputSystem::getScrollOffsetX,
			"getScrollOffsetY", &InputSystem::getScrollOffsetY,
			"getCursorPosition", &InputSystem::getCursorPosition,
			"setCursorPosition", &InputSystem::setCursorPosition
		);

		sol::table mouseButtonsConstants = lua->create_table();
		mouseButtonsConstants["LEFT_BUTTON"] = GLFW_MOUSE_BUTTON_LEFT;
		mouseButtonsConstants["RIGHT_BUTTON"] = GLFW_MOUSE_BUTTON_RIGHT;
		mouseButtonsConstants["MIDDLE_BUTTON"] = GLFW_MOUSE_BUTTON_MIDDLE;
		LuaUtils::createConstantTable(lua, "MouseButtons", mouseButtonsConstants);

		sol::table keysConstants = lua->create_table();
		keysConstants["KEY_SPACE"]          = GLFW_KEY_SPACE;
		keysConstants["KEY_APOSTROPHE"]     = GLFW_KEY_APOSTROPHE;
		keysConstants["KEY_COMMA"]          = GLFW_KEY_COMMA;
		keysConstants["KEY_MINUS"]          = GLFW_KEY_MINUS;
		keysConstants["KEY_PERIOD"]         = GLFW_KEY_PERIOD;
		keysConstants["KEY_SLASH"]          = GLFW_KEY_SLASH;
		keysConstants["KEY_0"]              = GLFW_KEY_0;
		keysConstants["KEY_1"]              = GLFW_KEY_1;
		keysConstants["KEY_2"]              = GLFW_KEY_2;
		keysConstants["KEY_3"]              = GLFW_KEY_3;
		keysConstants["KEY_4"]              = GLFW_KEY_4;
		keysConstants["KEY_5"]              = GLFW_KEY_5;
		keysConstants["KEY_6"]              = GLFW_KEY_6;
		keysConstants["KEY_7"]              = GLFW_KEY_7;
		keysConstants["KEY_8"]              = GLFW_KEY_8;
		keysConstants["KEY_9"]              = GLFW_KEY_9;
		keysConstants["KEY_SEMICOLON"]      = GLFW_KEY_SEMICOLON;
		keysConstants["KEY_EQUAL"]          = GLFW_KEY_EQUAL;
		keysConstants["KEY_A"]              = GLFW_KEY_A;
		keysConstants["KEY_B"]              = GLFW_KEY_B;
		keysConstants["KEY_C"]              = GLFW_KEY_C;
		keysConstants["KEY_D"]              = GLFW_KEY_D;
		keysConstants["KEY_E"]              = GLFW_KEY_E;
		keysConstants["KEY_F"]              = GLFW_KEY_F;
		keysConstants["KEY_G"]              = GLFW_KEY_G;
		keysConstants["KEY_H"]              = GLFW_KEY_H;
		keysConstants["KEY_I"]              = GLFW_KEY_I;
		keysConstants["KEY_J"]              = GLFW_KEY_J;
		keysConstants["KEY_K"]              = GLFW_KEY_K;
		keysConstants["KEY_L"]              = GLFW_KEY_L;
		keysConstants["KEY_M"]              = GLFW_KEY_M;
		keysConstants["KEY_N"]              = GLFW_KEY_N;
		keysConstants["KEY_O"]              = GLFW_KEY_O;
		keysConstants["KEY_P"]              = GLFW_KEY_P;
		keysConstants["KEY_Q"]              = GLFW_KEY_Q;
		keysConstants["KEY_R"]              = GLFW_KEY_R;
		keysConstants["KEY_S"]              = GLFW_KEY_S;
		keysConstants["KEY_T"]              = GLFW_KEY_T;
		keysConstants["KEY_U"]              = GLFW_KEY_U;
		keysConstants["KEY_V"]              = GLFW_KEY_V;
		keysConstants["KEY_W"]              = GLFW_KEY_W;
		keysConstants["KEY_X"]              = GLFW_KEY_X;
		keysConstants["KEY_Y"]              = GLFW_KEY_Y;
		keysConstants["KEY_Z"]              = GLFW_KEY_Z;
		keysConstants["KEY_LEFT_BRACKET"]   = GLFW_KEY_LEFT_BRACKET;
		keysConstants["KEY_BACKSLASH"]      = GLFW_KEY_BACKSLASH;
		keysConstants["KEY_RIGHT_BRACKET"]  = GLFW_KEY_RIGHT_BRACKET;
		keysConstants["KEY_GRAVE_ACCENT"]   = GLFW_KEY_GRAVE_ACCENT;
		keysConstants["KEY_WORLD_1"]        = GLFW_KEY_WORLD_1;
		keysConstants["KEY_WORLD_2"]        = GLFW_KEY_WORLD_2;

		/* Function keys */
		keysConstants["KEY_ESCAPE"]         = GLFW_KEY_ESCAPE;
		keysConstants["KEY_ENTER"]          = GLFW_KEY_ENTER;
		keysConstants["KEY_TAB"]            = GLFW_KEY_TAB;
		keysConstants["KEY_BACKSPACE"]      = GLFW_KEY_BACKSPACE;
		keysConstants["KEY_INSERT"]         = GLFW_KEY_INSERT;
		keysConstants["KEY_DELETE"]         = GLFW_KEY_DELETE;
		keysConstants["KEY_RIGHT"]          = GLFW_KEY_RIGHT;
		keysConstants["KEY_LEFT"]           = GLFW_KEY_LEFT;
		keysConstants["KEY_DOWN"]           = GLFW_KEY_DOWN;
		keysConstants["KEY_UP"]             = GLFW_KEY_UP;
		keysConstants["KEY_PAGE_UP"]        = GLFW_KEY_PAGE_UP;
		keysConstants["KEY_PAGE_DOWN"]      = GLFW_KEY_PAGE_DOWN;
		keysConstants["KEY_HOME"]           = GLFW_KEY_HOME;
		keysConstants["KEY_END"]            = GLFW_KEY_END;
		keysConstants["KEY_CAPS_LOCK"]      = GLFW_KEY_CAPS_LOCK;
		keysConstants["KEY_SCROLL_LOCK"]    = GLFW_KEY_SCROLL_LOCK;
		keysConstants["KEY_NUM_LOCK"]       = GLFW_KEY_NUM_LOCK;
		keysConstants["KEY_PRINT_SCREEN"]   = GLFW_KEY_PRINT_SCREEN;
		keysConstants["KEY_PAUSE"]          = GLFW_KEY_PAUSE;
		keysConstants["KEY_F1"]             = GLFW_KEY_F1;
		keysConstants["KEY_F2"]             = GLFW_KEY_F2;
		keysConstants["KEY_F3"]             = GLFW_KEY_F3;
		keysConstants["KEY_F4"]             = GLFW_KEY_F4;
		keysConstants["KEY_F5"]             = GLFW_KEY_F5;
		keysConstants["KEY_F6"]             = GLFW_KEY_F6;
		keysConstants["KEY_F7"]             = GLFW_KEY_F7;
		keysConstants["KEY_F8"]             = GLFW_KEY_F8;
		keysConstants["KEY_F9"]             = GLFW_KEY_F9;
		keysConstants["KEY_F10"]            = GLFW_KEY_F10;
		keysConstants["KEY_F11"]            = GLFW_KEY_F11;
		keysConstants["KEY_F12"]            = GLFW_KEY_F12;
		keysConstants["KEY_F13"]            = GLFW_KEY_F13;
		keysConstants["KEY_F14"]            = GLFW_KEY_F14;
		keysConstants["KEY_F15"]            = GLFW_KEY_F15;
		keysConstants["KEY_F16"]            = GLFW_KEY_F16;
		keysConstants["KEY_F17"]            = GLFW_KEY_F17;
		keysConstants["KEY_F18"]            = GLFW_KEY_F18;
		keysConstants["KEY_F19"]            = GLFW_KEY_F19;
		keysConstants["KEY_F20"]            = GLFW_KEY_F20;
		keysConstants["KEY_F21"]            = GLFW_KEY_F21;
		keysConstants["KEY_F22"]            = GLFW_KEY_F22;
		keysConstants["KEY_F23"]            = GLFW_KEY_F23;
		keysConstants["KEY_F24"]            = GLFW_KEY_F24;
		keysConstants["KEY_F25"]            = GLFW_KEY_F25;
		keysConstants["KEY_KP_0"]           = GLFW_KEY_KP_0;
		keysConstants["KEY_KP_1"]           = GLFW_KEY_KP_1;
		keysConstants["KEY_KP_2"]           = GLFW_KEY_KP_2;
		keysConstants["KEY_KP_3"]           = GLFW_KEY_KP_3;
		keysConstants["KEY_KP_4"]           = GLFW_KEY_KP_4;
		keysConstants["KEY_KP_5"]           = GLFW_KEY_KP_5;
		keysConstants["KEY_KP_6"]           = GLFW_KEY_KP_6;
		keysConstants["KEY_KP_7"]           = GLFW_KEY_KP_7;
		keysConstants["KEY_KP_8"]           = GLFW_KEY_KP_8;
		keysConstants["KEY_KP_9"]           = GLFW_KEY_KP_9;
		keysConstants["KEY_KP_DECIMAL"]     = GLFW_KEY_KP_DECIMAL;
		keysConstants["KEY_KP_DIVIDE"]      = GLFW_KEY_KP_DIVIDE;
		keysConstants["KEY_KP_MULTIPLY"]    = GLFW_KEY_KP_MULTIPLY;
		keysConstants["KEY_KP_SUBTRACT"]    = GLFW_KEY_KP_SUBTRACT;
		keysConstants["KEY_KP_ADD"]         = GLFW_KEY_KP_ADD;
		keysConstants["KEY_KP_ENTER"]       = GLFW_KEY_KP_ENTER;
		keysConstants["KEY_KP_EQUAL"]       = GLFW_KEY_KP_EQUAL;
		keysConstants["KEY_LEFT_SHIFT"]     = GLFW_KEY_LEFT_SHIFT;
		keysConstants["KEY_LEFT_CONTROL"]   = GLFW_KEY_LEFT_CONTROL;
		keysConstants["KEY_LEFT_ALT"]       = GLFW_KEY_LEFT_ALT;
		keysConstants["KEY_LEFT_SUPER"]     = GLFW_KEY_LEFT_SUPER;
		keysConstants["KEY_RIGHT_SHIFT"]    = GLFW_KEY_RIGHT_SHIFT;
		keysConstants["KEY_RIGHT_CONTROL"]  = GLFW_KEY_RIGHT_CONTROL;
		keysConstants["KEY_RIGHT_ALT"]      = GLFW_KEY_RIGHT_ALT;
		keysConstants["KEY_RIGHT_SUPER"]    = GLFW_KEY_RIGHT_SUPER;
		keysConstants["KEY_MENU"]           = GLFW_KEY_MENU;

		LuaUtils::createConstantTable(lua, "Keys", keysConstants);
	}
}
