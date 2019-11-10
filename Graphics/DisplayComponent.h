#ifndef DISPLAYCOMPONENT_H_INCLUDED
#define DISPLAYCOMPONENT_H_INCLUDED


#include <string>

#include "../Scene/Component.h"

#include "../GUI/Label.h"


class DisplayComponent : public Component
{
	private:
		RTexture2D* _matrixTexture;
		RTexture2D* _ledOnTexture;
		RTexture2D* _ledOffTexture;

		int _displayWidth;
		int _displayHeight;

		std::string _text;
		Framebuffer* _displayRenderTexture;

	public:
		DisplayComponent(int displayWidth, int displayHeight);
		~DisplayComponent();

		void generateTexture();
		RTexture2D* generateMatrixTexture();

		void setText(std::string text);
		std::string getText();

		void update(float deltaTime);

};


#endif // DISPLAYCOMPONENT_H_INCLUDED
