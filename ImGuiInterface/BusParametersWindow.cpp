#include "BusParametersWindow.h"

#include "../Bus/BusRaycast.h"


BusParametersWindow::BusParametersWindow(SceneManager* sceneManager, const std::vector<Bus*>& buses, bool isOpen/* = false*/)
	: ImGuiWindow(sceneManager, isOpen),
	_buses(buses),
	_currentBus(0)
{

}


void BusParametersWindow::drawWindow()
{
	if (ImGui::Begin("Bus parameters", &_isOpen))
	{
		std::string busesComboItems{};
		for (int i = 0; i < _buses.size(); ++i)
		{
			busesComboItems += _buses[i]->getSceneObject()->getName() + '\0';
		}

		ImGui::Combo("Bus", &_currentBus, busesComboItems.c_str());

		if (_buses.size() == 0)
		{
			return;
		}

		BusRaycast* bus = dynamic_cast<BusRaycast*>(_buses[_currentBus]);
		if (bus != nullptr)
		{
			ImGui::Text("Wheels");
			ImGui::Separator();

			for (int i = 0; i < bus->getWheelsCount(); ++i)
			{
				ImGui::PushID(i);

				BusRayCastWheel* wheel = bus->getWheel(i);
				btWheelInfo& wheelInfo = wheel->wheel->getWheelInfo();

				ImGui::Text("Wheel %d", i);

				if (ImGui::DragFloat("Suspension Rest Length", &wheelInfo.m_suspensionRestLength1, 0.01f));

				if (ImGui::DragFloat("Suspension Stiffness", &wheelInfo.m_suspensionStiffness))
				{
					wheelInfo.m_wheelsDampingCompression = wheel->dampingCompression * 2 * sqrt(wheelInfo.m_suspensionStiffness);
					wheelInfo.m_wheelsDampingRelaxation = wheel->dampingRelaxation * 2 * sqrt(wheelInfo.m_suspensionStiffness);
				}

				if (ImGui::DragFloat("Damping Compression", &wheel->dampingCompression))
				{
					wheelInfo.m_wheelsDampingCompression = wheel->dampingCompression * 2 * sqrt(wheelInfo.m_suspensionStiffness);
				}

				if (ImGui::DragFloat("Damping Relaxation", &wheel->dampingRelaxation))
				{
					wheelInfo.m_wheelsDampingRelaxation = wheel->dampingRelaxation * 2 * sqrt(wheelInfo.m_suspensionStiffness);
				}

				if (ImGui::DragFloat("Friction Slip", &wheelInfo.m_frictionSlip));
				if (ImGui::DragFloat("Roll Influence", &wheelInfo.m_rollInfluence));

				ImGui::Separator();

				ImGui::PopID();
			}

			for (int i = 0; i < bus->getWheelsCount(); ++i)
			{
				ImGui::PushID(i);

				BusRayCastWheel* wheel = bus->getWheel(i);
				btWheelInfo& wheelInfo = wheel->wheel->getWheelInfo();

				ImGui::Text("Wheel %d", i);

				ImGui::Checkbox("Steering", &wheel->steering);
				ImGui::Checkbox("Tag", &wheel->tag);
				ImGui::Checkbox("Powered", &wheel->powered);
				ImGui::Checkbox("Handbrake", &wheel->handbrake);

				ImGui::Separator();

				ImGui::PopID();
			}
		}
	}
	ImGui::End();
}

