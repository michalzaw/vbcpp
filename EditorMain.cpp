#include "Editor/Editor.h"

int main()
{
	//Editor editor;

	//editor.run();

	vbEditor::createWindow();
	vbEditor::initializeEngineSubsystems();

	vbEditor::run();

	vbEditor::shutDown();

	return 0;
}