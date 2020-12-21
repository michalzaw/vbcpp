#include "InputSystem.h"

#include <memory>

#include "../Window/Window.h"


static std::unique_ptr<InputSystem> inputSystemInstance;


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

	//glfwSetKeyCallback(_window->getWindow(), ::keyCallback);
	//glfwSetMouseButtonCallback(_window->getWindow(), ::mouseButtonCallback);
	//glfwSetScrollCallback(_window->getWindow(), ::scrollCallback);

	_isInitialized = true;
}


void InputSystem::update()
{
	//glfwPollEvents();
}


void InputSystem::keyCallback(int key, int scancode, int action, int mods)
{

}


void InputSystem::mouseButtonCallback(int button, int action, int mods)
{

}


void InputSystem::scrollCallback(double xOffset, double yOffset)
{

}


bool InputSystem::isKeyDown(int key)
{
	return glfwGetKey(_window->getWindow(), key) == GLFW_PRESS;
}


void InputSystem::getCursorPosition(double* x, double* y)
{
	glfwGetCursorPos(_window->getWindow(), x, y);
}


void InputSystem::setCursorPosition(double x, double y)
{
	glfwSetCursorPos(_window->getWindow(), x, y);
}
