#ifndef VARIABLESWINDOW_H_INCLUDED
#define VARIABLESWINDOW_H_INCLUDED


#include "ImGuiWindow.h"


class VariablesWindow : public ImGuiWindow
{
	private:
		std::string _newVariableFloatName;
		float _newVariableFloatValue;

		std::string _newVariablesStringName;
		std::string _newVariablesStringValue;

		virtual void drawWindow() override;

	public:
		VariablesWindow(SceneManager* sceneManager, std::vector<Bus*>* buses, bool isOpen = true);
};


#endif // VARIABLESWINDOW_H_INCLUDED
