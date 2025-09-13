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


BusStopComponent* SchedulesWindow::findBusStopById(int id)
{
	TransitSystem* transitSystem = _sceneManager->getTransitSystem();
	for (int i = 0; i < transitSystem->getBusStopsCount(); ++i)
	{
		BusStopComponent* busStop = transitSystem->getBusStop(i);
		if (busStop->getId() == id)
		{
			return busStop;
		}
	}

	return nullptr;
}


void SchedulesWindow::drawWindow()
{
	Schedule* availableSchedules = _sceneManager->getTransitSystem()->getSchedule();

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

		if (availableSchedules->lines.size() > 0 && availableSchedules->lines[_selectedLineIndex].brigades.size() > 0 && availableSchedules->lines[_selectedLineIndex].brigades[_selectedBrigadeIndex].routes.size() > 0)
		{
			const ScheduleRoute& currentRoute = availableSchedules->lines[_selectedLineIndex].brigades[_selectedBrigadeIndex].routes[_selectedRouteIndex];

			ImGuiTableFlags tableFlags = ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_RowBg | ImGuiTableFlags_Borders | ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable;
			if (ImGui::BeginTable("schedules", 3, tableFlags))
			{
				ImGui::TableSetupColumn("Stop id", ImGuiTableColumnFlags_WidthFixed);
				ImGui::TableSetupColumn("Stop name", ImGuiTableColumnFlags_WidthFixed);
				ImGui::TableSetupColumn("Time", ImGuiTableColumnFlags_WidthStretch);
				ImGui::TableHeadersRow();
				for (int i = 0; i < currentRoute.stops.size(); ++i)
				{
					BusStopComponent* busStopComponent = findBusStopById(currentRoute.stops[i].id);

					ImGui::TableNextRow();
					{
						ImGui::TableSetColumnIndex(0);
						ImGui::Text("Id: %d", currentRoute.stops[i].id);

						ImGui::TableSetColumnIndex(1);
						ImGui::Text(busStopComponent != nullptr ? busStopComponent->getName().c_str() : "");

						ImGui::TableSetColumnIndex(2);
						ImGui::Text(currentRoute.stops[i].time.toString().c_str());
					}
				}
				ImGui::EndTable();
			}
		}
	}
	ImGui::End();
}
