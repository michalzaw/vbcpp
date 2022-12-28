#include "SceneGraphWindow.h"

#include "../Editor.h"


SceneGraphWindow::SceneGraphWindow(SceneManager* sceneManager, SceneObject*& selectedSceneObject)
    : EditorWindow(sceneManager, selectedSceneObject),
	_centerGraph(false)
{

}


void SceneGraphWindow::drawWindow()
{
    glm::uvec2 windowSize(Renderer::getInstance().getWindowDimensions());

    ImGui::SetNextWindowSize(ImVec2(200, windowSize.y - 18), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSizeConstraints(ImVec2(100, windowSize.y - 18), ImVec2(500, windowSize.y - 18));
    ImGui::SetNextWindowPos(ImVec2(0, 18));

    ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse;
    if (ImGui::Begin("Scene Graph", nullptr, windowFlags))
    {
        std::list<SceneObject*>& sceneObjects = _sceneManager->getSceneObjects();
        for (std::list<SceneObject*>::iterator i = sceneObjects.begin(); i != sceneObjects.end(); ++i)
        {
            if ((*i)->getParent() == nullptr)
                inspectSceneObject(*i);
        }
    }
    ImGui::End();
}


void SceneGraphWindow::inspectSceneObject(SceneObject* object)
{
	if (startsWith(object->getName(), "editor#"))
		return;

    ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;

    if (object->getChildren().empty())
        node_flags = node_flags | ImGuiTreeNodeFlags_Leaf;
	if (object == _selectedSceneObject)
		node_flags = node_flags | ImGuiTreeNodeFlags_Selected;

	if (object == _selectedSceneObject && _centerGraph)
	{
		_centerGraph = false;
		ImGui::SetScrollHereY();
	}

    ImGui::PushID(object);
    bool node_open = ImGui::TreeNodeEx((void*)(intptr_t)object, node_flags, object->getName().c_str());

    if (ImGui::IsItemClicked())
        _selectedSceneObject = object;
    if (node_open)
    {
        for (SceneObject* child : object->getChildren())
        {
            ImGui::PushID(child);

            inspectSceneObject(child);

            ImGui::PopID();
        }
        ImGui::TreePop();
    }
    ImGui::PopID();
}


void SceneGraphWindow::centerGraphView()
{
	_centerGraph = true;
}



namespace vbEditor {

	extern SceneObject* _selectedSceneObject;
	extern std::vector<RoadObject*> _selectedRoads;


	static int selectionMask = -1;
	int nodeNumber, nodeClicked = -1;
	bool _centerGraph = false;


	void SceneGraphWindow(std::list<SceneObject*> sceneRoot)
	{
		nodeNumber = 0;

		ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoCollapse;
		if (ImGui::Begin("Scene Graph", nullptr, windowFlags))
		{
			for (std::list<SceneObject*>::iterator i = sceneRoot.begin(); i != sceneRoot.end(); ++i)
			{
				if ((*i)->getParent() == nullptr)
				{
					inspectSceneObject(*i);
				}
			}
		}
		if (nodeClicked != -1)
		{
			selectionMask = nodeClicked;
		}

		ImGui::End();
	}


	bool hasVisibleChildren(SceneObject* object)
	{
		if (object->getChildren().empty())
		{
			return false;
		}
		else
		{
			for (SceneObject* child : object->getChildren())
			{
				if (!(child->getFlags() & SOF_NOT_SELECTABLE))
				{
					return true;
				}
			}
		}
		return false;
	}


	void inspectSceneObject(SceneObject* object)
	{
		if (object->getFlags() & SOF_NOT_SELECTABLE)
			return;

		ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;

		if (!hasVisibleChildren(object))
			node_flags = node_flags | ImGuiTreeNodeFlags_Leaf;


		if (object == _selectedSceneObject)
		{
			node_flags |= ImGuiTreeNodeFlags_Selected;

			if (_centerGraph)
			{
				_centerGraph = false;
				ImGui::SetScrollHereY();
			}
		}

		//ImGui::PushID(object);
		bool node_open = ImGui::TreeNodeEx((void*)(intptr_t)object, node_flags, object->getName().c_str());

		if (ImGui::IsItemHovered())
		{
			ImGui::SetTooltip("Id: %d", object->getId());
		}

		if (ImGui::IsItemClicked())
		{
			vbEditor::setSelectedSceneObject(object);
			_centerGraph = false;
			nodeClicked = nodeNumber;
		}
		if (ImGui::IsItemClicked(1))
		{
			RoadObject* roadObject = dynamic_cast<RoadObject*>(object->getComponent(CT_ROAD_OBJECT));
			if (roadObject != nullptr)
			{
				if (!isVectorContains(_selectedRoads, roadObject))
				{
					_selectedRoads.push_back(roadObject);
				}
				else
				{
					auto element = std::find(_selectedRoads.begin(), _selectedRoads.end(), roadObject);
					_selectedRoads.erase(element);
				}
			}
		}

		if (node_open)
		{
			std::list<SceneObject*> children = object->getChildren();
			for (std::list<SceneObject*>::iterator i = children.begin(); i != children.end(); ++i)
			{
				SceneObject* child = *i;

				//ImGui::PushID(child);

				inspectSceneObject(child);

				//ImGui::PopID();
			}
			ImGui::TreePop();
		}
		//ImGui::PopID();

		nodeNumber++;
	}

	void centerGraphView()
	{
		_centerGraph = true;
	}

} // namespace