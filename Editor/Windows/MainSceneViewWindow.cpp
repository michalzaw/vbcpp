#include "MainSceneViewWindow.h"

#include <algorithm>
#include <ImGuizmo.h>
#include "glm/gtc/type_ptr.hpp"

#include "../Editor.h"
#include "../GuizmoParams.h"
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

	extern GuizmoParams _guizmoParams;
}


MainSceneViewWindow::MainSceneViewWindow(bool isOpen/* = false*/)
	: ImGuiWindow(nullptr, isOpen),
	_availableViewSize(2, 2),
	_isWindowHovered(false),
	_guizmoToolbarMode(GuizmoToolbarMode::BOTTOM_BAR)
{
	_translationButtonTexture = ResourceManager::getInstance().loadTexture("Icons/materialSymols/translateIcon.png");
	_rotationButtonTexture = ResourceManager::getInstance().loadTexture("Icons/materialSymols/rotateIcon.png");
	_scaleButtonTexture = ResourceManager::getInstance().loadTexture("Icons/materialSymols/scaleIcon.png");
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


void MainSceneViewWindow::showImGuizmoToolbarAsWindow()
{
	int margin = 5;
	int expectedWidth = 641;
	int width = expectedWidth;
	if (_sceneViewport.size.x < expectedWidth + 2 * margin)
	{
		width = _sceneViewport.size.x - 2 * margin;
	}

	const ImU32 flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoSavedSettings;
	ImGui::SetNextWindowSize(ImVec2(width, 38));
	//ImGui::SetNextWindowPos(ImVec2(200, io.DisplaySize.y - 40));

	int x = _sceneViewport.position.x + _sceneViewport.size.x - width - margin;
	ImGui::SetNextWindowPos(ImVec2(x, _sceneViewport.position.y + margin));

	//if (ImGui::BeginChild("Tools", ImVec2(800, 40), false, flags))
	if (ImGui::Begin("Tools", nullptr, flags))
	{
		showImGuizmoToolbar();
	}
	ImGui::End();
	//ImGui::EndChild();
}


void MainSceneViewWindow::showImGuizmoToolbarAsBottomBar()
{
	_availableViewSize.y -= 30;

	ImGui::Separator();

	showImGuizmoToolbar();
}


void MainSceneViewWindow::showImGuizmoToolbar()
{
		ImGuiStyle& style = ImGui::GetStyle();

		{
			ImVec4 selectedButtonColor = vbEditor::_guizmoParams.currentOperation == ImGuizmo::TRANSLATE ? style.Colors[ImGuiCol_ButtonActive] : style.Colors[ImGuiCol_Button];
			ImGui::PushStyleColor(ImGuiCol_Button, selectedButtonColor);
			if (ImGui::ImageButton((ImTextureID)_translationButtonTexture->getID(), ImVec2(16, 16)))
			{
				vbEditor::_guizmoParams.currentOperation = ImGuizmo::TRANSLATE;
			}
			if (ImGui::IsItemHovered())
			{
				ImGui::SetTooltip("Translation");
			}
			ImGui::PopStyleColor();
		}

		ImGui::SameLine();

		{
			ImVec4 selectedButtonColor = vbEditor::_guizmoParams.currentOperation == ImGuizmo::ROTATE ? style.Colors[ImGuiCol_ButtonActive] : style.Colors[ImGuiCol_Button];
			ImGui::PushStyleColor(ImGuiCol_Button, selectedButtonColor);
			if (ImGui::ImageButton((ImTextureID)_rotationButtonTexture->getID(), ImVec2(16, 16)))
			{
				vbEditor::_guizmoParams.currentOperation = ImGuizmo::ROTATE;
			}
			if (ImGui::IsItemHovered())
			{
				ImGui::SetTooltip("Rotation");
			}
			ImGui::PopStyleColor();
		}

		ImGui::SameLine();

		{
			ImVec4 selectedButtonColor = vbEditor::_guizmoParams.currentOperation == ImGuizmo::SCALE ? style.Colors[ImGuiCol_ButtonActive] : style.Colors[ImGuiCol_Button];
			ImGui::PushStyleColor(ImGuiCol_Button, selectedButtonColor);
			if (ImGui::ImageButton((ImTextureID)_scaleButtonTexture->getID(), ImVec2(16, 16)))
			{
				vbEditor::_guizmoParams.currentOperation = ImGuizmo::SCALE;
			}
			if (ImGui::IsItemHovered())
			{
				ImGui::SetTooltip("Scale");
			}
			ImGui::PopStyleColor();
		}

		ImGui::SameLine(0, 30);
		ImGui::Checkbox("Translate Snap", &vbEditor::_guizmoParams.translationSnap);

		ImGui::SameLine();

		{
			ImGui::PushID("TranslateSnapCombo");
			const char* const items[3] = { "1\0", "5\0", "10\0" };
			ImGui::SetNextItemWidth(50);
			if (ImGui::Combo("", &vbEditor::_guizmoParams.currentTranslationSnapValueIndex, items, 3))
			{

			}
			ImGui::PopID();
		}

		ImGui::SameLine(0, 30);
		ImGui::Checkbox("Rotate Snap", &vbEditor::_guizmoParams.rotationSnap);

		ImGui::SameLine();

		{
			ImGui::PushID("RotateSnapCombo");
			const char* const items[3] = { "1\0", "5\0", "10\0" };
			ImGui::SetNextItemWidth(50);
			if (ImGui::Combo("", &vbEditor::_guizmoParams.currentRotationSnapValueIndex, items, 3))
			{

			}
			ImGui::PopID();
		}

		ImGui::SameLine(0, 30);
		ImGui::Checkbox("Scale Snap", &vbEditor::_guizmoParams.scaleSnap);

		ImGui::SameLine();

		{
			ImGui::PushID("ScaleSnapCombo");
			const char* const items[3] = { "1\0", "5\0", "10\0" };
			ImGui::SetNextItemWidth(50);
			if (ImGui::Combo("", &vbEditor::_guizmoParams.currentScaleSnapValueIndex, items, 3))
			{

			}
			ImGui::PopID();
		}
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

		if (_guizmoToolbarMode == GuizmoToolbarMode::FLOATING_TOOLBAR)
		{
			showImGuizmoToolbarAsWindow();
		}
		else
		{
			showImGuizmoToolbarAsBottomBar();
		}

		showTools();

		_isWindowHovered = ImGui::IsWindowHovered() && !ImGuizmo::IsUsing() && !RoadManipulator::IsUsing();
	}
	ImGui::End();
}
