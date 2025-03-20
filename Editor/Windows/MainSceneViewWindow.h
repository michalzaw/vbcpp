#ifndef MAINSCENEVIEWWINDOW_H_INCLUDED
#define MAINSCENEVIEWWINDOW_H_INCLUDED


#include "../../ImGuiInterface/ImGuiWindow.h"

#include "../../Graphics/ShaderStorageBuffer.h"


class RTexture;


enum class GuizmoToolbarMode
{
	FLOATING_TOOLBAR,
	BOTTOM_BAR
};


class MainSceneViewWindow final : public ImGuiWindow
{
	private:
		RTexture* _texture;

		glm::uvec2 _availableViewSize;
		bool _isWindowHovered;

		glm::vec2 _cursorPositionOnSceneView;
		UintRect _sceneViewport;

		RTexture* _translationButtonTexture;
		RTexture* _rotationButtonTexture;
		RTexture* _scaleButtonTexture;

		GuizmoToolbarMode _guizmoToolbarMode;

		void calculateCursorPositionOnSceneView();
		void calculateViewport();

		void showImGuizmoToolbarAsWindow();
		void showImGuizmoToolbarAsBottomBar();
		void showImGuizmoToolbar();
		void showTools();

		virtual void drawWindow() override;

	public:
		explicit MainSceneViewWindow(bool isOpen = false);

		inline const glm::uvec2& getAvailableViewSize() { return _availableViewSize; }
		inline bool isWindowHovered() { return  _isWindowHovered; }

		inline const glm::vec2 getCursorPositionOnSceneView() { return _cursorPositionOnSceneView; }

		inline const UintRect& getSceneViewport() { return _sceneViewport; }

		inline GuizmoToolbarMode getGuizmoToolbarMode() { return _guizmoToolbarMode; }
		inline void setGuizmoToolbarMode(GuizmoToolbarMode mode) { _guizmoToolbarMode = mode; }

};


#endif // MAINSCENEVIEWWINDOW_H_INCLUDED
