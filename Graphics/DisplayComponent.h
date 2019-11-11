#ifndef DISPLAYCOMPONENT_H_INCLUDED
#define DISPLAYCOMPONENT_H_INCLUDED


#include <string>

#include "RDisplayFont.h"

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

	int headSize;
	DisplayTextType type;
};


class DisplayComponent : public Component
{
	private:
		RTexture2D* _matrixTexture;
		RTexture2D* _ledOnTexture;
		RTexture2D* _ledOffTexture;

		RDisplayFont* _font;
		int _displayWidth;
		int _displayHeight;

		DisplayText _displayText;

		std::string _text;
		Framebuffer* _displayRenderTexture;

	public:
		DisplayComponent(RDisplayFont* font, int displayWidth, int displayHeight);
		~DisplayComponent();

		int getCharIndex(char c);

		void generateTexture();
		RTexture2D* generateMatrixTexture();
		void addTextToMatrixTexture(unsigned char* data, int& beginX, int& beginY, const std::string& text, int sizeIndex);
		void calculateTextsSize(int& headSize, int& line1Size, int& line2Size);
		int calculateTextWidth(const std::string& text, int sizeIndex);

		void setText(std::string text);
		std::string getText();

		void update(float deltaTime);

};


#endif // DISPLAYCOMPONENT_H_INCLUDED
