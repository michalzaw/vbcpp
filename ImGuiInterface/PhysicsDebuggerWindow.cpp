#include "PhysicsDebuggerWindow.h"

#include "../Graphics/PhysicsDebugRenderer.h"


PhysicsDebuggerWindow::PhysicsDebuggerWindow(SceneManager* sceneManager, bool isOpen)
	: ImGuiWindow(sceneManager, isOpen)
{

}


void PhysicsDebuggerWindow::drawDebugModeCheckBox(int& debugModes, const char* name, btIDebugDraw::DebugDrawModes mode)
{
	bool checked = debugModes & mode;

	if (ImGui::Checkbox(name, &checked))
	{
		if (checked)
		{
			debugModes = debugModes | mode;
		}
		else
		{
			debugModes = debugModes & ~mode;
		}
	}
}


void PhysicsDebuggerWindow::drawWindow()
{
	if (ImGui::Begin("Physics debbuger", &_isOpen))
	{
		PhysicsDebugRenderer* debugRenderer = static_cast<PhysicsDebugRenderer*>(_sceneManager->getPhysicsManager()->getDebugRenderer());

		bool physicsDebuggerEnabled = _sceneManager->getPhysicsManager()->getDebugRenderingState();
		if (ImGui::Checkbox("Debug renderer enabled", &physicsDebuggerEnabled))
		{
			_sceneManager->getPhysicsManager()->setDebugRenderingState(physicsDebuggerEnabled);
		}

		bool drawOnClearScreen = debugRenderer->getClearRenderTargetBeforeRenderingFlag();
		if (ImGui::Checkbox("Draw on clear screen", &drawOnClearScreen))
		{
			debugRenderer->setClearRenderTargetBeforeRenderingFlag(drawOnClearScreen);
		}

		ImGui::Separator();
		ImGui::Text("Rendering mode flags");

		int debugMode = debugRenderer->getDebugMode();

		drawDebugModeCheckBox(debugMode, "DBG_DrawWireframe", btIDebugDraw::DBG_DrawWireframe);
		drawDebugModeCheckBox(debugMode, "DBG_DrawAabb", btIDebugDraw::DBG_DrawAabb);
		drawDebugModeCheckBox(debugMode, "DBG_DrawContactPoints", btIDebugDraw::DBG_DrawContactPoints);
		drawDebugModeCheckBox(debugMode, "DBG_NoDeactivation", btIDebugDraw::DBG_NoDeactivation);
		drawDebugModeCheckBox(debugMode, "DBG_DrawConstraints", btIDebugDraw::DBG_DrawConstraints);
		drawDebugModeCheckBox(debugMode, "DBG_DrawConstraintLimits", btIDebugDraw::DBG_DrawConstraintLimits);
		drawDebugModeCheckBox(debugMode, "DBG_DrawNormals", btIDebugDraw::DBG_DrawNormals);
		drawDebugModeCheckBox(debugMode, "DBG_DrawFrames", btIDebugDraw::DBG_DrawFrames);

		debugRenderer->setDebugMode(debugMode);

		ImGui::Separator();

		bool isPhysicsEnabled = _sceneManager->getPhysicsManager()->isRunning();
		if (ImGui::Checkbox("Physics enabled", &isPhysicsEnabled))
		{
			if (isPhysicsEnabled)
			{
				_sceneManager->getPhysicsManager()->play();
			}
			else
			{
				_sceneManager->getPhysicsManager()->stop();
			}
		}
	}
	ImGui::End();
}
