#ifndef BUSLINEANDDIRECTIONWINDOW_H_INCLUDED
#define BUSLINEANDDIRECTIONWINDOW_H_INCLUDED


#include "ImGuiWindow.h"


class BusLineAndDirectionWindow : public ImGuiWindow
{
	private:
		virtual void drawWindow() override;

	public:
		BusLineAndDirectionWindow(SceneManager* sceneManager, bool isOpen = true);
};


#endif // BUSLINEANDDIRECTIONWINDOW_H_INCLUDED
