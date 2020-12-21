#ifndef INPUTSYSTEM_H_INCLUDED
#define INPUTSYSTEM_H_INCLUDED


#include <vector>

class Window;


class InputSystem
{
	private:
		bool _isInitialized;
		Window* _window;

		std::vector<bool> _keys;
		std::vector<bool> _keysPressed;
		std::vector<bool> _keysReleased;

		std::vector<bool> _mouseButtonPressed;
		std::vector<bool> _mouseButtonReleased;

		double _scrollOffsetX;
		double _scrollOffsetY;

		InputSystem();

	public:
		~InputSystem();

		static InputSystem& getInstance();

		void init(Window* window);
		void update();

		void keyCallback(int key, int scancode, int action, int mods);
		void mouseButtonCallback(int button, int action, int mods);
		void scrollCallback(double xOffset, double yOffset);

		bool isKeyDown(int key);
		bool isKeyPressed(int key);
		bool isKeyReleased(int key);

		bool isMouseButtonPressed(int mouseButton);
		bool isMouseButtonReleased(int mouseButton);

		double getScrollOffsetX();
		double getScrollOffsetY();

		void getCursorPosition(double* x, double* y);
		void setCursorPosition(double x, double y);
};


#endif // INPUTSYSTEM_H_INCLUDED
