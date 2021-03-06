#include "InputSystem.h"

#include <memory>

#include "../Window/Window.h"


static std::unique_ptr<InputSystem> inputSystemInstance;


void inputSystemKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	inputSystemInstance->keyCallback(key, scancode, action, mods);
}


void inputSystemMouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	inputSystemInstance->mouseButtonCallback(button, action, mods);
}


void inputSystemScrollCallback(GLFWwindow* window, double xOffset, double yOffset)
{
	inputSystemInstance->scrollCallback(xOffset, yOffset);
}


InputSystem::InputSystem()
	: _isInitialized(false), _window(nullptr)
{

}


InputSystem::~InputSystem()
{

}


InputSystem& InputSystem::getInstance()
{
	if (!inputSystemInstance)
		inputSystemInstance = std::unique_ptr<InputSystem>(new InputSystem);

	return *inputSystemInstance;
}


void InputSystem::init(Window* window)
{
	_window = window;

	glfwSetKeyCallback(_window->getWindow(), inputSystemKeyCallback);
	glfwSetMouseButtonCallback(_window->getWindow(), inputSystemMouseButtonCallback);
	glfwSetScrollCallback(_window->getWindow(), inputSystemScrollCallback);

	_keys.resize(GLFW_KEY_LAST + 1);
	_keysPressed.resize(GLFW_KEY_LAST + 1);
	_keysReleased.resize(GLFW_KEY_LAST + 1);
	
	_mouseButtonPressed.resize(GLFW_MOUSE_BUTTON_LAST + 1);
	_mouseButtonReleased.resize(GLFW_MOUSE_BUTTON_LAST + 1);

	_isInitialized = true;
}


void InputSystem::update()
{
	std::fill(_keysPressed.begin(), _keysPressed.end(), false);
	std::fill(_keysReleased.begin(), _keysReleased.end(), false);

	std::fill(_mouseButtonPressed.begin(), _mouseButtonPressed.end(), false);
	std::fill(_mouseButtonReleased.begin(), _mouseButtonReleased.end(), false);

	_scrollOffsetX = _scrollOffsetY = 0.0;

	glfwPollEvents();
}


void InputSystem::keyCallback(int key, int scancode, int action, int mods)
{
	if (action == GLFW_PRESS)
	{
		_keys[key] = true;
		_keysPressed[key] = true;
	}
	else if (action == GLFW_RELEASE)
	{
		_keys[key] = false;
		_keysReleased[key] = true;
	}
}


void InputSystem::mouseButtonCallback(int button, int action, int mods)
{
	if (action == GLFW_PRESS)
	{
		_mouseButtonPressed[button] = true;
	}
	else if (action == GLFW_RELEASE)
	{
		_mouseButtonReleased[button] = true;
	}
}


void InputSystem::scrollCallback(double xOffset, double yOffset)
{
	_scrollOffsetX = xOffset;
	_scrollOffsetY = yOffset;
}


bool InputSystem::isKeyDown(int key)
{
	return _keys[key];
}


bool InputSystem::isKeyPressed(int key)
{
	return _keysPressed[key];
}


bool InputSystem::isKeyReleased(int key)
{
	return _keysReleased[key];
}


bool InputSystem::isMouseButtonPressed(int mouseButton)
{
	return _mouseButtonPressed[mouseButton];
}


bool InputSystem::isMouseButtonReleased(int mouseButton)
{
	return _mouseButtonReleased[mouseButton];
}


double InputSystem::getScrollOffsetX()
{
	return _scrollOffsetX;
}


double InputSystem::getScrollOffsetY()
{
	return _scrollOffsetY;
}


void InputSystem::getCursorPosition(double* x, double* y)
{
	glfwGetCursorPos(_window->getWindow(), x, y);
}


void InputSystem::setCursorPosition(double x, double y)
{
	glfwSetCursorPos(_window->getWindow(), x, y);
}
