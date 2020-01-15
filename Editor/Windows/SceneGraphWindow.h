#ifndef SCENEGRAPHWINDOW_H_INCLUDED
#define SCENEGRAPHWINDOW_H_INCLUDED


#include "EditorWindow.h"


class SceneGraphWindow : public EditorWindow
{
    private:
		bool _centerGraph;

        virtual void drawWindow() override;

        void inspectSceneObject(SceneObject* object);

    public:
        SceneGraphWindow(SceneManager* sceneManager, SceneObject*& selectedSceneObject);

		void centerGraphView();
};

namespace vbEditor
{
	void SceneGraphWindow(std::list<SceneObject*> sceneRoot);

	void inspectSceneObject(SceneObject* object);

	void centerGraphView();
}


#endif // SCENEGRAPHWINDOW_H_INCLUDED
