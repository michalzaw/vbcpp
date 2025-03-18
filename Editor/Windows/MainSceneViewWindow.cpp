#include "MainSceneViewWindow.h"

#include <algorithm>
#include <ImGuizmo.h>
#include "glm/gtc/type_ptr.hpp"

#include "../Editor.h"
#include "../Tools/AxisTool.h"
#include "../Tools/RoadManipulator.h"

#include "../../Graphics/BezierCurve.h"
#include "../../Graphics/OGLDriver.h"
#include "../../Graphics/RoadObject.h"
#include "../../Graphics/ShapePolygonComponent.h"


namespace vbEditor
{
	extern CameraFPS* _camera;
	extern SceneObject* _selectedSceneObject;
}


MainSceneViewWindow::MainSceneViewWindow(bool isOpen/* = false*/)
	: ImGuiWindow(nullptr, isOpen),
	_availableViewSize(2, 2),
	_isWindowHovered(false)
{
	//_texture = Renderer::getInstance()._postProcessingFramebuffers[0]->getTexture(0);
	//_texture->setClampMode(TCM_CLAMP_TO_EDGE);
	//_texture->setFiltering(TFM_LINEAR, TFM_LINEAR);
}


void MainSceneViewWindow::calculateCursorPositionOnSceneView()
{
	ImVec2 windowPos = ImGui::GetWindowPos();
	ImVec2 vMin = ImGui::GetWindowContentRegionMin();
	ImVec2 cursourPos = ImGui::GetMousePos();

	_cursorPositionOnSceneView = glm::vec2(cursourPos.x - windowPos.x - vMin.x, _texture->getSize().y - (cursourPos.y - windowPos.y - vMin.y));
}


void MainSceneViewWindow::calculateViewport()
{
	ImVec2 windowPos = ImGui::GetWindowPos();
	ImVec2 vMin = ImGui::GetWindowContentRegionMin();

	_sceneViewport.position.x = windowPos.x + vMin.x;
	_sceneViewport.position.y = windowPos.y + vMin.y;
	_sceneViewport.size.x = _texture->getSize().x;
	_sceneViewport.size.y = _texture->getSize().y;
}


void MainSceneViewWindow::showTools()
{
	ImDrawList* drawList = ImGui::GetWindowDrawList();
	if (vbEditor::_selectedSceneObject)
	{
		ImGuizmo::SetDrawlist(drawList);
		RoadManipulator::SetDrawlist(drawList);

		RoadObject* roadComponent = vbEditor::_selectedSceneObject->getComponentWithCasting<RoadObject>(CT_ROAD_OBJECT);
		ShapePolygonComponent* shapePolygonComponent = vbEditor::_selectedSceneObject->getComponentWithCasting<ShapePolygonComponent>(CT_SHAPE_POLYGON);
		BezierCurve* bezierCurveComponent = vbEditor::_selectedSceneObject->getComponentWithCasting<BezierCurve>(CT_BEZIER_CURVE);

		if (roadComponent && roadComponent->getRoadType() != RoadType::BEZIER_CURVES)
			vbEditor::showRoadTools();
		else if (shapePolygonComponent)
			vbEditor::showPolygonEditTool();
		else if (bezierCurveComponent)
			vbEditor::showBezierCurveTool();
		else
			vbEditor::ShowTransformGizmo();
	}

	AxisTool::SetDrawlist(drawList);
	vbEditor::showAxisTool();
}


void MainSceneViewWindow::drawWindow()
{
	_texture = Renderer::getInstance().getOutFramebuffer()->getTexture(0);

	if (ImGui::Begin("Scene", &_isOpen/*, ImGuiWindowFlags_NoMouseInputs*/))
	{
		calculateCursorPositionOnSceneView();
		calculateViewport();

		ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
		_availableViewSize = glm::uvec2(std::max((unsigned int) viewportPanelSize.x, 2u), std::max((unsigned int) viewportPanelSize.y, 2u));

		_texture->setClampMode(TCM_REPEAT);
		ImGui::Image((ImTextureID)_texture->getID(), ImVec2(_texture->getSize().x, _texture->getSize().y) , ImVec2(0, 1), ImVec2(1, 0));
		_texture->setClampMode(TCM_CLAMP_TO_EDGE);

		showTools();

		_isWindowHovered = ImGui::IsWindowHovered() && !ImGuizmo::IsUsing() && !RoadManipulator::IsUsing();
	}
	ImGui::End();
}
