#ifndef OBJECTDEFINITIONSWINDOW_H_INCLUDED
#define OBJECTDEFINITIONSWINDOW_H_INCLUDED


#include "EditorWindow.h"
#include "../EditorContext.h"


class ObjectDefinitionsWindow : public EditorWindow
{
	private:
		std::shared_ptr<EditorContext> _editorContext;

		int _listBoxObjectsSelectedItem;

		virtual void drawWindow() override;

	public:
		ObjectDefinitionsWindow(SceneManager* sceneManager, SceneObject*& selectedSceneObject, std::shared_ptr<EditorContext> editorContext);
};


#endif // OBJECTDEFINITIONSWINDOW_H_INCLUDED
