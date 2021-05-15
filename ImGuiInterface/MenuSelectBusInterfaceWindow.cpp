#include "MenuSelectBusInterfaceWindow.h"

#include "../Bus/BusPreviewLoader.h"


MenuSelectBusInterfaceWindow::MenuSelectBusInterfaceWindow(SceneManager* sceneManager, bool isOpen)
	: ImGuiWindow(sceneManager, isOpen),
	_busPreview(nullptr)
{

}


void MenuSelectBusInterfaceWindow::drawWindow()
{

	ImGui::ShowDemoWindow();

	ImGuiIO& io = ImGui::GetIO();

	const ImU32 flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoBringToFrontOnFocus;
	ImGui::SetNextWindowSize(io.DisplaySize);
	ImGui::SetNextWindowPos(ImVec2(0, 0));

	ImGui::PushStyleColor(ImGuiCol_WindowBg, 0);
	ImGui::PushStyleColor(ImGuiCol_Border, 0);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);

	ImGui::Begin("GUI", NULL, flags);

	/*ImGui::Text("Lorem ipsum");
	//ImGui::Image((ImTextureID)ResourceManager::getInstance().loadTexture("Data/logo.jpg")->getID(), ImVec2(200, 200));

	ImGui::GetWindowDrawList()->AddImage((ImTextureID)ResourceManager::getInstance().loadTexture("Data/logo.jpg")->getID(), ImVec2(200, 200), ImVec2(400, 400));

	ImGui::GetWindowDrawList()->AddCircleFilled(ImVec2(500, 500), 20.0f, IM_COL32(255, 255, 255, 255));


	ImGui::SetNextWindowSize(ImVec2(200, 300));
	ImGui::SetNextWindowPos(ImVec2(0, io.DisplaySize.y - 300));
	ImGui::Begin("GUI2", NULL, flags);

	ImGui::SetWindowFontScale(1.5);
	ImGui::Text("Lorem ipsum\nHello world\n1\n22\n333\n\n Dlugi tekst");

	ImGui::End();*/


	ImGui::PopStyleVar();
	ImGui::PopStyleColor(2);

	drawBusConfigurations();


	ImGui::End();
}


void MenuSelectBusInterfaceWindow::drawBusConfigurations()
{
	const ImGuiIO& io = ImGui::GetIO();

	const ImU32 flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoBringToFrontOnFocus;
	ImGui::SetNextWindowSize(ImVec2(300, 200));
	ImGui::SetNextWindowPos(ImVec2(io.DisplaySize.x - 350, io.DisplaySize.y - 250));
	if (ImGui::Begin("BusConfigurations", nullptr, flags))
	{
		ImGui::SetWindowFontScale(1.5);

		if (_busPreview != nullptr)
		{
			static int selected = 0;

			int i = 0;
			for (const auto& busConfiguration : _busPreview->predefinedConfigurations)
			{
				ImGui::RadioButton(busConfiguration.first.c_str(), &selected, i);
				++i;
			}
			
		}

		//ImGui::Text("Lorem ipsum\nHello world\n1\n22\n333\n\n Dlugi tekst");
	}
	ImGui::End();
}


void MenuSelectBusInterfaceWindow::setCurrentBusPreview(BusPreview* busPreview)
{
	_busPreview = busPreview;
}
