#ifndef DISPLAYCOMPONENT_H_INCLUDED
#define DISPLAYCOMPONENT_H_INCLUDED


#include <string>

#include "DisplayText.h"
#include "RDisplayFont.h"
#include "RShader.h"

#include "../Scene/Component.h"

#include "../Utils/RTexture2D.h"


class Framebuffer;


class DisplayComponent : public Component
{
	VBCPP_COMPONENT(DisplayComponent, CT_DISPLAY)

	private:
		RShader* _tabGeneratorShader;

		unsigned char* _matrixTextureData;

		RTexture2D* _matrixTexture;
		RTexture2D* _ledOffTexture;

		glm::vec3 _emissiveColor;

		RDisplayFont* _font;
		int _displayWidth;
		int _displayHeight;

		DisplayText _displayText;
		bool _isTextChanged;

		Framebuffer* _displayRenderTexture;

		int getCharIndex(char c);

		void generateMatrixTexture();
		void addTextToMatrixTexture(unsigned char* data, int& beginX, int& beginY, const std::string& text, int sizeIndex);
		void calculateTextsSize(int& headSize, int& line1Size, int& line2Size);
		int calculateTextWidth(const std::string& text, int sizeIndex);

		void generateTexture();

	public:
		DisplayComponent(RDisplayFont* font, int displayWidth, int displayHeight, glm::vec3 textColor);//2 * vec3(1, 0.4, 0.1) * vec3(0.96, 0.67, 0.22);
		~DisplayComponent();

		LUAF void setText(DisplayText& text);
		LUAF DisplayText& getText();

		void init();

		void update(float deltaTime) override;

};


#endif // DISPLAYCOMPONENT_H_INCLUDED
