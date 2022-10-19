#ifndef PHYSICSDEBUGGRERWINDOW_H_INCLUDED
#define PHYSICSDEBUGGRERWINDOW_H_INCLUDED


#include "ImGuiWindow.h"


class PhysicsDebuggerWindow : public ImGuiWindow
{
	private:
		void drawDebugModeCheckBox(int& debugModes, const char* name, btIDebugDraw::DebugDrawModes mode);

		virtual void drawWindow() override;

	public:
		PhysicsDebuggerWindow(SceneManager* sceneManager, bool isOpen = true);

};


#endif // PHYSICSDEBUGGRERWINDOW_H_INCLUDED
