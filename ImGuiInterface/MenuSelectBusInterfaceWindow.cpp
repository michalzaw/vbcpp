#include "MenuSelectBusInterfaceWindow.h"

#include "../Bus/BusPreviewLoader.h"
#include "../Bus/BusRepaintLoader.h"


MenuSelectBusInterfaceWindow::MenuSelectBusInterfaceWindow(std::unordered_map<std::string, std::string>* selectedBusConfigurationVariables, std::string& selectedBusRepaintName, SceneManager* sceneManager, bool isOpen)
	: ImGuiWindow(sceneManager, isOpen),
	_busPreview(nullptr), _selectedBusConfigurationVariables(selectedBusConfigurationVariables), _selectedBusRepaintName(selectedBusRepaintName)
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
	drawBusRepaints();
	drawStartButton();


	ImGui::End();
}


void MenuSelectBusInterfaceWindow::drawBusConfigurations()
{
	const ImGuiIO& io = ImGui::GetIO();

	const ImU32 flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoBringToFrontOnFocus;
	ImGui::SetNextWindowSize(ImVec2(400, 200));
	ImGui::SetNextWindowPos(ImVec2(io.DisplaySize.x - 450, io.DisplaySize.y - 250));

	//ImGui::PushStyleColor(ImGuiCol_WindowBg, 0);
	//ImGui::PushStyleColor(ImGuiCol_Border, 0);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);

	if (ImGui::Begin("BusConfigurations", nullptr, flags))
	{
		//ImGui::SetWindowFontScale(1.5);

		if (_busPreview != nullptr)
		{
			static int selectedConfiguration = 0;
			std::string configurationNames = "";
			for (const auto& configurationName : _busPreview->predefinedConfigurations)
			{
				configurationNames += configurationName.first + '\0';
			}

			ImGui::PushID("Configuration");
			{
				ImGui::SetNextItemWidth(400 - (2 * 8));
				if (ImGui::Combo("", &selectedConfiguration, configurationNames.c_str()))
				{
					changeBusConfiguration(selectedConfiguration);
				}
			}
			ImGui::PopID();

			ImGui::Separator();

			for (int i = 0; i < _busPreview->availableVariables.size(); ++i)
			{
				const GameVariable& variable = _busPreview->availableVariables[i];

				ImGui::PushID(variable.name.c_str());
				{
					ImGui::Text(variable.displayName.c_str());
					if (ImGui::IsItemHovered())
					{
						ImGui::SetTooltip(variable.description.c_str());
					}

					ImGui::SameLine(200);

					int selectedVariableValue = 0;
					std::string variableValues = "";

					int j = 0;
					for (const auto& variableValue : variable.values)
					{
						variableValues += variableValue + '\0';

						if (variableValue == (*_selectedBusConfigurationVariables)[variable.name])
						{
							selectedVariableValue = j;
						}

						++j;
					}

					ImGui::SetNextItemWidth(400 - 200 - 8);
					if (ImGui::Combo("", &selectedVariableValue, variableValues.c_str()))
					{
						(*_selectedBusConfigurationVariables)[variable.name] = _busPreview->availableVariables[i].values[selectedVariableValue];

						_busPreview->setConfiguration(*_selectedBusConfigurationVariables);
						_busPreview->bus->getSceneObject()->setRotation(0.0f, degToRad(0.0f), 0.0f);
					}
				}
				ImGui::PopID();
			}
			
		}
	}
	ImGui::End();

	ImGui::PopStyleVar();
	//ImGui::PopStyleColor(2);
}


void MenuSelectBusInterfaceWindow::drawBusRepaints()
{
	const ImGuiIO& io = ImGui::GetIO();

	const ImU32 flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoBringToFrontOnFocus;
	ImGui::SetNextWindowSize(ImVec2(io.DisplaySize.x - 2 * 50, 88));
	ImGui::SetNextWindowPos(ImVec2(50, io.DisplaySize.y - 88 - 50));

	ImGui::PushStyleColor(ImGuiCol_WindowBg, 0);
	ImGui::PushStyleColor(ImGuiCol_Border, 0);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);

	if (ImGui::Begin("BusRepaints", nullptr, flags))
	{
		//ImGui::SetWindowFontScale(1.5);

		if (_busPreview != nullptr)
		{
			for (const BusRepaintDescription& repaintDescription : _busPreview->availableRepaints)
			{
				ImGui::PushID(repaintDescription.name.c_str());

				if (ImGui::ImageButton((ImTextureID)repaintDescription.logo->getID(), ImVec2(64, 64)))
				{
					std::vector<RMaterialsCollection*> altMaterialsCollections;
					BusRepaintLoader::loadBusRepaint(_busPreview->busName, repaintDescription.repainDirectorytName, altMaterialsCollections);
					for (RMaterialsCollection* materialsCollection : altMaterialsCollections)
					{
						_busPreview->bus->replaceMaterialsByName(materialsCollection->getMaterials());
					}

					_selectedBusRepaintName = repaintDescription.repainDirectorytName;
				}

				if (ImGui::IsItemHovered())
				{
					ImGui::SetTooltip(repaintDescription.name.c_str());
				}

				ImGui::PopID();

				ImGui::SameLine();
			}
		}
	}
	ImGui::End();

	ImGui::PopStyleVar();
	ImGui::PopStyleColor(2);
}


void MenuSelectBusInterfaceWindow::drawStartButton()
{
	const ImGuiIO& io = ImGui::GetIO();

	const ImU32 flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoBringToFrontOnFocus;
	//ImGui::SetNextWindowSize(ImVec2(io.DisplaySize.x - 2 * 50, 88));
	ImGui::SetNextWindowPos(ImVec2((io.DisplaySize.x - 100) / 2.0f, io.DisplaySize.y - 50 - 25));

	ImGui::PushStyleColor(ImGuiCol_WindowBg, 0);
	ImGui::PushStyleColor(ImGuiCol_Border, 0);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);

	if (ImGui::Begin("StartBuuton", nullptr, flags))
	{
		ImGui::Button("Start", ImVec2(100, 25));
	}
	ImGui::End();

	ImGui::PopStyleVar();
	ImGui::PopStyleColor(2);
}


void MenuSelectBusInterfaceWindow::changeBusConfiguration(int index)
{
	int i = 0;
	for (auto& configuration : _busPreview->predefinedConfigurations)
	{
		if (i == index)
		{
			(*_selectedBusConfigurationVariables) = configuration.second;
		}

		++i;
	}

	_busPreview->setConfiguration(*_selectedBusConfigurationVariables);
	_busPreview->bus->getSceneObject()->setRotation(0.0f, degToRad(0.0f), 0.0f);
}


void MenuSelectBusInterfaceWindow::setCurrentBusPreview(BusPreview* busPreview)
{
	_busPreview = busPreview;
}
