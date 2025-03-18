#include "MainSceneViewWindow.h"

#include <algorithm>
#include <ImGuizmo.h>
#include "glm/gtc/type_ptr.hpp"

#include "../../Graphics/OGLDriver.h"


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


void MainSceneViewWindow::showGizmo()
{
	if (vbEditor::_selectedSceneObject == nullptr)
	{
		return;
	}

	glm::mat4 modelMatrix = vbEditor::_selectedSceneObject->getLocalTransformMatrix();
	glm::mat4 viewMatrix = vbEditor::_camera->getViewMatrix();

	static ImGuizmo::OPERATION mCurrentGizmoOperation(ImGuizmo::TRANSLATE);
	static ImGuizmo::MODE mCurrentGizmoMode(ImGuizmo::LOCAL);

	//ImGuiIO& io = ImGui::GetIO();
	const UintRect& viewport = _sceneViewport;
	ImGuizmo::SetRect(viewport.position.x, viewport.position.y, viewport.size.x, viewport.size.y);
	ImGuizmo::Manipulate(glm::value_ptr(viewMatrix), glm::value_ptr(vbEditor::_camera->getProjectionMatrix()),
		mCurrentGizmoOperation, mCurrentGizmoMode,
		glm::value_ptr(modelMatrix),
		NULL,
		NULL,
		NULL,
		NULL
	);
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

		_isWindowHovered = ImGui::IsWindowHovered();

		_texture->setClampMode(TCM_REPEAT);
		ImGui::Image((ImTextureID)_texture->getID(), ImVec2(_texture->getSize().x, _texture->getSize().y) , ImVec2(0, 1), ImVec2(1, 0));
		_texture->setClampMode(TCM_CLAMP_TO_EDGE);

		ImGuizmo::SetDrawlist(ImGui::GetWindowDrawList());

		showGizmo();

	}
	ImGui::End();
}
