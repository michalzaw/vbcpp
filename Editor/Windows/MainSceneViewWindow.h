#ifndef MAINSCENEVIEWWINDOW_H_INCLUDED
#define MAINSCENEVIEWWINDOW_H_INCLUDED


#include "../../ImGuiInterface/ImGuiWindow.h"

#include "../../Graphics/ShaderStorageBuffer.h"


class RShader;
class RTexture;


class MainSceneViewWindow final : public ImGuiWindow
{
	private:
		RTexture* _texture;

		glm::uvec2 _availableViewSize;
		bool _isWindowHovered;

		glm::vec2 _cursorPositionOnSceneView;
		UintRect _sceneViewport;

		void calculateCursorPositionOnSceneView();
		void calculateViewport();

		void showGizmo();

		virtual void drawWindow() override;

	public:
		explicit MainSceneViewWindow(bool isOpen = false);

		inline const glm::uvec2& getAvailableViewSize() { return _availableViewSize; }
		inline bool isWindowHovered() { return  _isWindowHovered; }

		inline const glm::vec2 getCursorPositionOnSceneView() { return _cursorPositionOnSceneView; }

		inline const UintRect& getSceneViewport() { return _sceneViewport; }

};


#endif // MAINSCENEVIEWWINDOW_H_INCLUDED
