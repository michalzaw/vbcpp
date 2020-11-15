#include "PhysicsDebuggerWindow.h"


PhysicsDebuggerWindow::PhysicsDebuggerWindow(SceneManager* sceneManager, std::vector<Bus*>* buses, bool isOpen)
	: ImGuiWindow(sceneManager, buses, isOpen)
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
		bool physicsDebuggerEnabled = _sceneManager->getPhysicsManager()->getDebugRenderingState();
		if (ImGui::Checkbox("Enabled", &physicsDebuggerEnabled))
		{
			_sceneManager->getPhysicsManager()->setDebugRenderingState(physicsDebuggerEnabled);
		}

		ImGui::Separator();
		ImGui::Text("Rendering mode flags");

		btIDebugDraw* debugRenderer = _sceneManager->getPhysicsManager()->getDebugRenderer();
		int debugMode = debugRenderer->getDebugMode();

		drawDebugModeCheckBox(debugMode, "DBG_NoDebug", btIDebugDraw::DBG_NoDebug);
		drawDebugModeCheckBox(debugMode, "DBG_DrawWireframe", btIDebugDraw::DBG_DrawWireframe);
		drawDebugModeCheckBox(debugMode, "DBG_DrawAabb", btIDebugDraw::DBG_DrawAabb);
		drawDebugModeCheckBox(debugMode, "DBG_DrawFeaturesText", btIDebugDraw::DBG_DrawFeaturesText);
		drawDebugModeCheckBox(debugMode, "DBG_DrawContactPoints", btIDebugDraw::DBG_DrawContactPoints);
		drawDebugModeCheckBox(debugMode, "DBG_NoDeactivation", btIDebugDraw::DBG_NoDeactivation);
		drawDebugModeCheckBox(debugMode, "DBG_NoHelpText", btIDebugDraw::DBG_NoHelpText);
		drawDebugModeCheckBox(debugMode, "DBG_DrawText", btIDebugDraw::DBG_DrawText);
		drawDebugModeCheckBox(debugMode, "DBG_ProfileTimings", btIDebugDraw::DBG_ProfileTimings);
		drawDebugModeCheckBox(debugMode, "DBG_EnableSatComparison", btIDebugDraw::DBG_EnableSatComparison);
		drawDebugModeCheckBox(debugMode, "DBG_DisableBulletLCP", btIDebugDraw::DBG_DisableBulletLCP);
		drawDebugModeCheckBox(debugMode, "DBG_EnableCCD", btIDebugDraw::DBG_EnableCCD);
		drawDebugModeCheckBox(debugMode, "DBG_DrawConstraints", btIDebugDraw::DBG_DrawConstraints);
		drawDebugModeCheckBox(debugMode, "DBG_DrawConstraintLimits", btIDebugDraw::DBG_DrawConstraintLimits);
		drawDebugModeCheckBox(debugMode, "DBG_FastWireframe", btIDebugDraw::DBG_FastWireframe);
		drawDebugModeCheckBox(debugMode, "DBG_DrawNormals", btIDebugDraw::DBG_DrawNormals);
		drawDebugModeCheckBox(debugMode, "DBG_DrawFrames", btIDebugDraw::DBG_DrawFrames);

		debugRenderer->setDebugMode(debugMode);
	}
	ImGui::End();
}
