#include "BusLineAndDirectionWindow.h"

#include "../Game/GameLogicSystem.h"


BusLineAndDirectionWindow::BusLineAndDirectionWindow(SceneManager* sceneManager, std::vector<Bus*>* buses, bool isOpen)
	: ImGuiWindow(sceneManager, isOpen),
	_buses(buses)
{

}


void BusLineAndDirectionWindow::drawWindow()
{
	if (ImGui::Begin("Line and direction", &_isOpen))
	{
		Bus* bus = (*_buses)[0];

		char lineBuffer[1024];
		char direction1Buffer[1024];
		char direction2Buffer[1024];

		strncpy(lineBuffer, bus->getDisplayText().head.c_str(), sizeof lineBuffer);
		strncpy(direction1Buffer, bus->getDisplayText().line1.c_str(), sizeof direction1Buffer);
		strncpy(direction2Buffer, bus->getDisplayText().line2.c_str(), sizeof direction2Buffer);

		lineBuffer[sizeof lineBuffer - 1] = '\0';
		direction1Buffer[sizeof direction1Buffer - 1] = '\0';
		direction2Buffer[sizeof direction2Buffer - 1] = '\0';

		if (ImGui::InputText("Line", lineBuffer, IM_ARRAYSIZE(lineBuffer)))
		{
			bus->getDisplayText().head = lineBuffer;
		}
		if (ImGui::InputText("Direction line 1", direction1Buffer, IM_ARRAYSIZE(direction1Buffer)))
		{
			bus->getDisplayText().line1 = direction1Buffer;
		}
		if (ImGui::InputText("Direction line 2", direction2Buffer, IM_ARRAYSIZE(direction2Buffer)))
		{
			bus->getDisplayText().line2 = direction2Buffer;
		}

		int e = bus->getDisplayText().type;
		ImGui::RadioButton("one line", &e, 1);
		ImGui::RadioButton("two lines", &e, 2);
		ImGui::RadioButton("two lines, first big", &e, 3);
		ImGui::RadioButton("two lines, second big", &e, 4);
		bus->getDisplayText().type = (DisplayTextType)e;

		if (ImGui::Button("Ok", ImVec2(120, 0)))
		{
			bus->updateDisplays();
		}
	}
	ImGui::End();
}
