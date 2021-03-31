#ifndef DISPLAYCOMPONENT_H_INCLUDED
#define DISPLAYCOMPONENT_H_INCLUDED


#include <string>

#include "RDisplayFont.h"
#include "RShader.h"

#include "../Scene/Component.h"

#include "../Utils/RTexture2D.h"


enum DisplayTextType
{
	ONLY_HEAD,
	ONE_LINE,
	TWO_LINE,
	TWO_LINE_FIRST_BIG,
	TWO_LINE_SECOND_BIG
};


struct DisplayText
{
	std::string head;
	std::string line1;
	std::string line2;

	DisplayTextType type;
};


class DisplayComponent : public Component
{
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

		Framebuffer* _displayRenderTexture;

		int getCharIndex(char c);

		void generateMatrixTexture();
		void addTextToMatrixTexture(unsigned char* data, int& beginX, int& beginY, const std::string& text, int sizeIndex);
		void calculateTextsSize(int& headSize, int& line1Size, int& line2Size);
		int calculateTextWidth(const std::string& text, int sizeIndex);

		void generateTexture();

	public:
		DisplayComponent(RDisplayFont* font, int displayWidth, int displayHeight, glm::vec3 textColor = DEFAULT_TEXT_COLOR);//2 * vec3(1, 0.4, 0.1) * vec3(0.96, 0.67, 0.22);
		~DisplayComponent();

		void setText(DisplayText& text);
		DisplayText& getText();

		void init();

		static constexpr glm::vec3 DEFAULT_TEXT_COLOR = glm::vec3(1.92, 0.536, 0.044);

};


#endif // DISPLAYCOMPONENT_H_INCLUDED
