#include "Editor/Editor.h"

#include "Utils/Logger.h"


int main()
{
	//Editor editor;

	//editor.run();

	Logger::init(LL_DEBUG, true, true, "logEditor.txt");

	vbEditor::createWindow();
	vbEditor::initializeEngineSubsystems();

	vbEditor::run();

	vbEditor::shutDown();

	Logger::destroy();

	return 0;
}