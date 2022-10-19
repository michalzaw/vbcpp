#ifndef BUSLINEANDDIRECTIONWINDOW_H_INCLUDED
#define BUSLINEANDDIRECTIONWINDOW_H_INCLUDED


#include "ImGuiWindow.h"


class BusLineAndDirectionWindow : public ImGuiWindow
{
	private:
		std::vector<Bus*>* _buses;

		virtual void drawWindow() override;

	public:
		BusLineAndDirectionWindow(SceneManager* sceneManager, std::vector<Bus*>* buses, bool isOpen = true);
};


#endif // BUSLINEANDDIRECTIONWINDOW_H_INCLUDED
