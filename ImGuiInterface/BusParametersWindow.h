#ifndef BUSPARAMETERSWINDOW_H_INCLUDED
#define BUSPARAMETERSWINDOW_H_INCLUDED


#include "ImGuiWindow.h"


class BusParametersWindow : public ImGuiWindow
{
	private:
		const std::vector<Bus*>& _buses;

		int _currentBus;

		virtual void drawWindow() override;

	public:
		BusParametersWindow(SceneManager* sceneManager, const std::vector<Bus*>& buses, bool isOpen = true);
};


#endif // BUSPARAMETERSWINDOW_H_INCLUDED
