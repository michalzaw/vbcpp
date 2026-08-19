#include "SchedulesWindow.h"

#include "../Game/Transit/TransitSystem.h"
#include "../Game/GameLogicSystem.h"

#include "../Game/Transit/Schedule.h"

#include "../../Utils/ImGuiUtils.h"


SchedulesWindow::SchedulesWindow(SceneManager* sceneManager, std::vector<Bus*>* buses, bool isOpen)
	: ImGuiWindow(sceneManager, isOpen),
	_buses(buses),
	_selectedLineIndex(0), _selectedBrigadeIndex(0), _selectedRouteIndex(0)
{

}


void SchedulesWindow::drawWindow()
{
	TransitSystem* transitSytem = _sceneManager->getTransitSystem();
	Schedule* availableSchedules = transitSytem->getSchedule();

	if (ImGui::Begin("Schedule", &_isOpen))
	{
		std::string linesComboData;
		if (availableSchedules->lines.size() > 0)
		{
			ImGuiUtils::convertVectorToComboData(availableSchedules->lines, linesComboData, false);
		}

		if (ImGui::Combo("Line", &_selectedLineIndex, linesComboData.c_str()))
		{
			_selectedBrigadeIndex = 0;
			_selectedRouteIndex = 0;
		}

		std::string brigadesComboData;
		if (availableSchedules->lines.size() > 0 && availableSchedules->lines[_selectedLineIndex].brigades.size() > 0)
		{
			ImGuiUtils::convertVectorToComboData(availableSchedules->lines[_selectedLineIndex].brigades, brigadesComboData, false);
		}

		if (ImGui::Combo("Brigade", &_selectedBrigadeIndex, brigadesComboData.c_str()))
		{
			_selectedRouteIndex = 0;
		}

		std::string routesComboData;
		if (availableSchedules->lines.size() > 0 && availableSchedules->lines[_selectedLineIndex].brigades.size() > 0 && availableSchedules->lines[_selectedLineIndex].brigades[_selectedBrigadeIndex].routes.size() > 0)
		{
			ImGuiUtils::convertVectorToComboData(availableSchedules->lines[_selectedLineIndex].brigades[_selectedBrigadeIndex].routes, routesComboData, false);
		}

		if (ImGui::Combo("Route", &_selectedRouteIndex, routesComboData.c_str()))
		{

		}

		if (ImGui::Button("OK"))
		{
			_sceneManager->getTransitSystem()->setCurrentRoute(_selectedLineIndex, _selectedBrigadeIndex, _selectedRouteIndex);
		}

		ImGui::Separator();

		const CurrentRouteData& currentRouteData = _sceneManager->getTransitSystem()->getCurrentRouteData();

		if (currentRouteData.isSet() && availableSchedules->lines.size() > 0 && availableSchedules->lines[currentRouteData.currentLineIndex].brigades.size() > 0 && availableSchedules->lines[currentRouteData.currentLineIndex].brigades[currentRouteData.currentBrigadeIndex].routes.size() > 0)
		{
			ImGuiTableFlags tableFlags = ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_RowBg | ImGuiTableFlags_Borders | ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable;
			if (ImGui::BeginTable("schedules", 3, tableFlags))
			{
				ImGui::TableSetupColumn("Stop id", ImGuiTableColumnFlags_WidthFixed);
				ImGui::TableSetupColumn("Stop name", ImGuiTableColumnFlags_WidthFixed);
				ImGui::TableSetupColumn("Time", ImGuiTableColumnFlags_WidthStretch);
				ImGui::TableHeadersRow();
				for (int i = 0; i < currentRouteData.currentRoute->stops.size(); ++i)
				{
					BusStopComponent* busStopComponent = transitSytem->findBusStopById(currentRouteData.currentRoute->stops[i].id);

					ImGui::TableNextRow();
					{
						ImGui::TableSetColumnIndex(0);
						ImGui::Text("Id: %d", currentRouteData.currentRoute->stops[i].id);

						ImGui::TableSetColumnIndex(1);
						ImGui::Text(busStopComponent != nullptr ? busStopComponent->getName().c_str() : "");

						ImGui::TableSetColumnIndex(2);
						ImGui::Text(currentRouteData.currentRoute->stops[i].time.toString().c_str());
					}
				}
				ImGui::EndTable();
			}


			ImGui::Text("Current bus stop: %s", currentRouteData.currentBusStopIndex >= 0 ? transitSytem->findBusStopById(currentRouteData.currentRoute->stops[currentRouteData.currentBusStopIndex].id)->getName() : "");
			ImGui::Text("Next bus stop: %s", currentRouteData.nextBusStopIndex >= 0 ? transitSytem->findBusStopById(currentRouteData.currentRoute->stops[currentRouteData.nextBusStopIndex].id)->getName() : "");
		}
	}
	ImGui::End();
}
