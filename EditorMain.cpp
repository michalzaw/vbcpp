#include "Editor/Editor.h"

#include "Utils/Logger2.h"


int main()
{
	//Editor editor;

	//editor.run();

	Logger2::init(LL_DEBUG, true, true, "logEditor.txt");

	vbEditor::createWindow();
	vbEditor::initializeEngineSubsystems();

	vbEditor::run();

	vbEditor::shutDown();

	Logger2::destroy();

	return 0;
}