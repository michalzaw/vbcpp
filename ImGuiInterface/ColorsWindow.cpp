#include "ColorsWindow.h"


ColorsWindow::ColorsWindow(SceneManager* sceneManager, bool isOpen)
	: ImGuiWindow(sceneManager, isOpen)
{

}


void ColorsWindow::drawWindow()
{
	if (ImGui::Begin("Colors", &_isOpen))
	{
		ImGui::Text("Tree colors");
		ImGui::ColorEdit3("Color1##1", (float*)& Renderer::getInstance().color1, ImGuiColorEditFlags_NoOptions);
		ImGui::ColorEdit3("Color2##2", (float*)& Renderer::getInstance().color2, ImGuiColorEditFlags_NoOptions);
		ImGui::ColorEdit3("Color3##3", (float*)& Renderer::getInstance().color3, ImGuiColorEditFlags_NoOptions);
		ImGui::ColorEdit3("Color4##4", (float*)& Renderer::getInstance().color4, ImGuiColorEditFlags_NoOptions);

		ImGui::Text("Grass color");

		SceneObject* grassObject = _sceneManager->getSceneObject("grass");
		if (grassObject != nullptr)
		{
			Grass* grass = static_cast<Grass*>(grassObject->getComponent(CT_GRASS));
			glm::vec4 color = grass->getGrassColor();

			ImGui::ColorEdit3("MyColor##1", (float*)& color, ImGuiColorEditFlags_NoOptions);

			grass->setGrassColor(color);
		}
	}
	ImGui::End();
}
