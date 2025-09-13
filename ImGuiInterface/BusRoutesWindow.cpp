#include "BusRoutesWindow.h"

#include "../Game/GameLogicSystem.h"

#include "../Game/Transit/BusRoutes.h"

#include "../../Utils/ImGuiUtils.h"


BusRoutesWindow::BusRoutesWindow(SceneManager* sceneManager, std::vector<Bus*>* buses, bool isOpen)
	: ImGuiWindow(sceneManager, isOpen),
	_buses(buses)
{

}


DisplayTextType from(BusRouteDisplayType type)
{
	return static_cast<DisplayTextType>(type + 1);
}


void BusRoutesWindow::drawWindow()
{
	BusRoutes* availabelRoutes = _sceneManager->getTransitSystem()->getRoutes();

	if (ImGui::Begin("Routes", &_isOpen))
	{
		static std::string selectedLineValue = " ";
		static std::string selectedRouteValue = " ";

		std::vector<std::string> lines;
		for (const auto& pair : availabelRoutes->lines) {
			lines.push_back(pair.first);
		}

		std::string linesComboData;
		int selectedLineIndex;
		ImGuiUtils::convertVectorToComboData(lines, selectedLineValue, linesComboData, selectedLineIndex);

		if (ImGui::Combo("Line", &selectedLineIndex, linesComboData.c_str()))
		{
			if (selectedLineIndex > 0)
			{
				selectedLineValue = lines[selectedLineIndex - 1];
				selectedRouteValue = " ";
			}
			else
			{
				selectedLineValue = " ";
				selectedRouteValue = " ";
			}
		}

		std::vector<std::string> routes;
		if (selectedLineValue != " ")
		{
			for (const auto& pair : availabelRoutes->lines[selectedLineValue].routes) {
				routes.push_back(pair.first);
			}
		}

		std::string routesComboData;
		int selectedRouteIndex;
		ImGuiUtils::convertVectorToComboData(routes, selectedRouteValue, routesComboData, selectedRouteIndex);

		if (ImGui::Combo("Route", &selectedRouteIndex, routesComboData.c_str()))
		{
			if (selectedRouteIndex > 0)
			{
				selectedRouteValue = routes[selectedRouteIndex - 1];
			}
			else
			{
				selectedRouteValue = " ";
			}
		}

		if (selectedLineValue != " " && selectedRouteValue != " ")
		{
			BusLine& selectedLine = availabelRoutes->lines[selectedLineValue];
			BusRoute& selectedRoute = selectedLine.routes[selectedRouteValue];

			ImGui::Text("Line: %s", selectedLine.number.c_str());
			ImGui::Text("Route: %s", selectedRoute.direction.c_str());

			if (ImGui::Button("Ok", ImVec2(120, 0)))
			{
				Bus* bus = (*_buses)[0];

				bus->getDisplayText().head = selectedRoute.head;
				bus->getDisplayText().line1 = selectedRoute.line1;
				bus->getDisplayText().line2 = selectedRoute.line2;
				bus->getDisplayText().type = from(selectedRoute.type);
				bus->updateDisplays();
			}
		}
	}
	ImGui::End();
}
