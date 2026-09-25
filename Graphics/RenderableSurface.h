#ifndef RENDERABLESURFACE_H_INCLUDED
#define RENDERABLESURFACE_H_INCLUDED


#include "../GUI/GUIManager.h"

#include "../Scene/Component.h"

#include "../Scripting/Utils/LuaMacros.h"


class RenderableSurface : public Component
{
	VBCPP_COMPONENT(RenderableSurface, CT_RENDERABLE_SURFACE)

	private:
		GUIManager* _guiManager;

		int _width;
		int _height;

		glm::vec4 _backgroundColor;

		Framebuffer* _renderTexture;

		void recreateFramebuffer();

	public:
		RenderableSurface(int width, int height);
		~RenderableSurface();

		LUAF Image* addImage(RTexture* texture);
		LUAF Label* addLabel(RFont* font, std::string text = "");

		LUAF void removeObject(GUIObject* object);
		LUAF void removeAllObjects();

		LUAF int getWidth();
		LUAF int getHeight();

		void setWidth(int width, bool recreateFramebuffer = false);
		void setHeight(int height, bool recreateFramebuffer = false);

		LUAF void setBackgroundColor(const glm::vec4& backgroundColor);
		LUAF const glm::vec4& getBackgroundColor();

		LUAF RTexture* getRenderedTexture();

		void update(float deltaTime) override;

};


#endif // RENDERABLESURFACE_H_INCLUDED
