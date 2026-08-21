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

		if (currentRouteData.isSet() &&
			availableSchedules->lines.size() > 0 &&
			availableSchedules->lines[currentRouteData.getCurrentLineIndex()].brigades.size() > 0 &&
			availableSchedules->lines[currentRouteData.getCurrentLineIndex()].brigades[currentRouteData.getCurrentBrigadeIndex()].routes.size() > 0)
		{
			ImGuiTableFlags tableFlags = ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_RowBg | ImGuiTableFlags_Borders | ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable;
			if (ImGui::BeginTable("schedules", 3, tableFlags))
			{
				ImGui::TableSetupColumn("Stop id", ImGuiTableColumnFlags_WidthFixed);
				ImGui::TableSetupColumn("Stop name", ImGuiTableColumnFlags_WidthFixed);
				ImGui::TableSetupColumn("Time", ImGuiTableColumnFlags_WidthStretch);
				ImGui::TableHeadersRow();
				for (int i = 0; i < currentRouteData.getCurrentRoute()->stops.size(); ++i)
				{
					BusStopComponent* busStopComponent = currentRouteData.getBusStopOnRoute(i);

					ImGui::TableNextRow();
					{
						ImGui::TableSetColumnIndex(0);
						ImGui::Text("Id: %d", currentRouteData.getBusStopScheduleData(i)->id);

						ImGui::TableSetColumnIndex(1);
						ImGui::Text(busStopComponent != nullptr ? busStopComponent->getName().c_str() : "");

						ImGui::TableSetColumnIndex(2);
						ImGui::Text(currentRouteData.getBusStopScheduleData(i)->time.toString().c_str());
					}
				}
				ImGui::EndTable();
			}

			BusStopComponent* currentBusStop = currentRouteData.getCurrentBusStop();
			BusStopComponent* nextBusStop = currentRouteData.getNextBusStop();
			ImGui::Text("Current bus stop: %s", currentBusStop != nullptr ? currentBusStop->getName().c_str() : "");
			ImGui::Text("Next bus stop: %s", nextBusStop != nullptr ? nextBusStop->getName().c_str() : "");
		}

		bool isRouteFinished = true;// !currentRouteData.isNextBusStopSet();
		if (currentRouteData.isSet() && isRouteFinished)
		{
			ImGui::Text("Route statistics");

			ImGuiTableFlags tableFlags = ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_RowBg | ImGuiTableFlags_Borders | ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable;
			if (ImGui::BeginTable("Route stats", 9, tableFlags))
			{
				ImGui::TableSetupColumn("Stop id", ImGuiTableColumnFlags_WidthStretch);
				ImGui::TableSetupColumn("Stop name", ImGuiTableColumnFlags_WidthStretch);
				ImGui::TableSetupColumn("Is visited", ImGuiTableColumnFlags_WidthStretch);
				ImGui::TableSetupColumn("Arrival time", ImGuiTableColumnFlags_WidthStretch);
				ImGui::TableSetupColumn("Departure time", ImGuiTableColumnFlags_WidthStretch);
				ImGui::TableSetupColumn("Passengers who wanted to get off", ImGuiTableColumnFlags_WidthStretch);
				ImGui::TableSetupColumn("Passengers who got off", ImGuiTableColumnFlags_WidthStretch);
				ImGui::TableSetupColumn("Passengers who wanted to get in", ImGuiTableColumnFlags_WidthStretch);
				ImGui::TableSetupColumn("Passengers who got in", ImGuiTableColumnFlags_WidthStretch);
				ImGui::TableHeadersRow();
				for (int i = 0; i < currentRouteData.getBusStopsStatsData().size(); ++i)
				{
					const CurrentRouteBusStopsStatsData& statsData = currentRouteData.getBusStopsStatsData()[i];
					BusStopComponent* busStopComponent = currentRouteData.getBusStopOnRoute(i);

					ImGui::TableNextRow();
					{
						ImGui::TableSetColumnIndex(0);
						ImGui::Text("Id: %d", currentRouteData.getBusStopScheduleData(i)->id);

						ImGui::TableSetColumnIndex(1);
						ImGui::Text(busStopComponent != nullptr ? busStopComponent->getName().c_str() : "");

						ImGui::TableSetColumnIndex(2);
						ImGui::Text(statsData.isVisited ? "Yes" : "No");

						ImGui::TableSetColumnIndex(3);
						ImGui::Text(statsData.arrivalTime.toString().c_str());

						ImGui::TableSetColumnIndex(4);
						ImGui::Text(statsData.departureTime.toString().c_str());

						ImGui::TableSetColumnIndex(5);
						ImGui::Text(Strings::toString(statsData.numberOfPassengersWhoWantedToGetOff).c_str());

						ImGui::TableSetColumnIndex(6);
						ImGui::Text(Strings::toString(statsData.numberOfPassengersWhoGotOff).c_str());

						ImGui::TableSetColumnIndex(7);
						ImGui::Text(Strings::toString(statsData.numberOfPassengersWhoWantedToGetIn).c_str());

						ImGui::TableSetColumnIndex(8);
						ImGui::Text(Strings::toString(statsData.numberOfPassengersWhoGotIn).c_str());
					}
				}
				ImGui::EndTable();
			}
		}
	}
	ImGui::End();
}
