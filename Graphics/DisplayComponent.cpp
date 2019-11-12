#include "DisplayComponent.h"

#include "../Scene/SceneObject.h"

#include "../Graphics/RenderObject.h"
#include "../Graphics/Renderer.h"


DisplayComponent::DisplayComponent(RDisplayFont* font, int displayWidth, int displayHeight)
	: Component(CT_DISPLAY),
	_font(font), _displayWidth(displayWidth), _displayHeight(displayHeight)
{
	/*_displayText.head = "0";
	_displayText.headSize = 4;
	_displayText.line1 = "katowice";
	_displayText.line2 = "dworzec pkp";
	_displayText.type = TWO_LINE;*/

	//_matrixTexture = ResourceManager::getInstance().loadTexture("tab1.png");
	_matrixTextureData = new unsigned char[_displayWidth * _displayHeight * 4];
	for (int i = 0; i < _displayWidth * _displayHeight * 4; ++i)
	{
		_matrixTextureData[i] = 0;
	}
	_matrixTexture = new RTexture2D("", _matrixTextureData, TF_RGBA, glm::uvec2(_displayWidth, _displayHeight));
	_matrixTexture->setFiltering(TFM_NEAREST, TFM_NEAREST);

	_ledOnTexture = ResourceManager::getInstance().loadTexture("doron.bmp");
	_ledOffTexture = ResourceManager::getInstance().loadTexture("doroff.bmp");

	int ledWidth = _ledOnTexture->getSize().x;
	int ledHeight = _ledOnTexture->getSize().y;

	_displayRenderTexture = OGLDriver::getInstance().createFramebuffer();
	_displayRenderTexture->addTexture(TF_RGBA, _displayWidth * ledWidth, _displayHeight * ledHeight);
	_displayRenderTexture->setViewport(UintRect(0, 0, _displayWidth * ledWidth, _displayHeight * ledHeight));
	_displayRenderTexture->getTexture(0)->setFiltering(TFM_TRILINEAR, TFM_LINEAR);
}


DisplayComponent::~DisplayComponent()
{
	delete[] _matrixTextureData;
	delete _matrixTexture;
}


int DisplayComponent::getCharIndex(char c)
{
	if (c >= '0' && c <= '9')
	{
		return (int)(c - '0');
	}
	else
	{
		return (int)(c - 'a') + 10;
	}
}


void DisplayComponent::generateMatrixTexture()
{
	for (int i = 0; i < _displayWidth * _displayHeight * 4; ++i)
	{
		_matrixTextureData[i] = 0;
	}

	/*--------------------------------------------------------
	//std::string text = "101bis plac mlynarz";
	std::string text = "0 katowice dworzec";
	addTextToMatrixTexture(data, 0, 0, text, 4);
	--------------------------------------------------------*/

	int headSize = 0;
	int line1Size = 0;
	int line2Size = 0;
	calculateTextsSize(headSize, line1Size, line2Size);

	int beginX = 0;
	int beginY = 0;

	// head
	addTextToMatrixTexture(_matrixTextureData, beginX, beginY, _displayText.head, headSize);

	int headEnd = beginX;

	// line1 and line2
	if (_displayText.type != ONLY_HEAD)
	{
		int textWidth = calculateTextWidth(_displayText.line1, line1Size);
		if (textWidth <= _displayWidth - headEnd)
		{
			beginX = headEnd + (_displayWidth - headEnd) / 2 - textWidth / 2;
		}

		addTextToMatrixTexture(_matrixTextureData, beginX, beginY, _displayText.line1, line1Size);
	}

	if (_displayText.type == TWO_LINE || _displayText.type == TWO_LINE_FIRST_BIG || _displayText.type == TWO_LINE_SECOND_BIG)
	{
		int textWidth = calculateTextWidth(_displayText.line2, line2Size);
		if (textWidth <= _displayWidth - headEnd)
		{
			beginX = headEnd + (_displayWidth - headEnd) / 2 - textWidth / 2;
		}

		beginY += _font->getAvailableSizes()[line1Size] + 1;
		addTextToMatrixTexture(_matrixTextureData, beginX, beginY, _displayText.line2, line2Size);
	}
	
	_matrixTexture->setTexSubImage(_matrixTextureData, 0, 0, _displayWidth, _displayHeight);
}


void DisplayComponent::addTextToMatrixTexture(unsigned char* data, int& beginX, int& beginY, const std::string& text, int sizeIndex)
{
	RDisplayFontSize* fontSize = _font->getFontInSize(sizeIndex);

	for (int i = 0; i < text.size(); ++i)
	{

		if (text[i] == ' ')
		{
			beginX += 2;
			continue;
		}

		int charIndex = getCharIndex(text[i]);
		
		int charWidth = 0;
		for (int row = 0; row < fontSize->height; ++row)
		{
			for (int x = 0; x < fontSize->charsWidth[charIndex]; ++x)
			{
				int b = 1 << x;
				if (fontSize->chars[charIndex][row] & b)
				{
					int index = ((_displayHeight - (beginY + row) - 1) * _displayWidth + beginX + x) * 4;
					data[index] = 255;
					data[index + 1] = 255;
					data[index + 2] = 255;
					data[index + 3] = 255;
				}
			}
		}
		beginX += fontSize->charsWidth[charIndex] + 1;
	}
}


void DisplayComponent::calculateTextsSize(int& headSize, int& line1Size, int& line2Size)
{
	for (int i = RDisplayFont::NUMBER_OF_SIZES - 1; i >= 0; --i)
	{
		if (_font->getAvailableSizes()[i] <= _displayHeight)
		{
			headSize = i;
			break;
		}
	}

	if (_displayText.type == ONE_LINE)
	{
		line1Size = headSize;
	}
	else if (_displayText.type == TWO_LINE)
	{
		int s = (_displayHeight - 1) / 2;

		for (int i = RDisplayFont::NUMBER_OF_SIZES - 1; i >= 0; --i)
		{
			if (_font->getAvailableSizes()[i] <= s)
			{
				line1Size = line2Size = i;
				break;
			}
		}
	}
	else if (_displayText.type == TWO_LINE_FIRST_BIG)
	{
		line1Size = 2;
		line2Size = 0;
	}
	else if (_displayText.type == TWO_LINE_SECOND_BIG)
	{
		line1Size = 0;
		line2Size = 2;
	}
}


int DisplayComponent::calculateTextWidth(const std::string& text, int sizeIndex)
{
	int width = 0;

	RDisplayFontSize* fontSize = _font->getFontInSize(sizeIndex);

	for (int i = 0; i < text.size(); ++i)
	{
		if (text[i] == ' ')
		{
			width += 2;
			continue;
		}

		int charIndex = getCharIndex(text[i]);

		width += fontSize->charsWidth[charIndex] + 1;
	}

	return width;
}


void DisplayComponent::generateTexture()
{
	float quadVertices[] =
	{
		-1.0f, -1.0f,
		1.0f, -1.0f,
		-1.0f, 1.0f,
		1.0f, 1.0f
	};
	VBO* quadVBO = OGLDriver::getInstance().createVBO(4 * 2 * sizeof(float));
	quadVBO->addVertexData(quadVertices, 4 * 2);

	RShader* shader = ResourceManager::getInstance().loadShader("Shaders/quad.vert", "Shaders/tabGenerator.frag");



	_displayRenderTexture->bind();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	quadVBO->bind();

	shader->enable();


	shader->bindTexture(shader->getUniformLocation("matrixTexture"), _matrixTexture);
	shader->bindTexture(shader->getUniformLocation("ledOnTexture"), _ledOnTexture);
	shader->bindTexture(shader->getUniformLocation("ledOffTexture"), _ledOffTexture);



	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), (void*)0);

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	glDisableVertexAttribArray(0);


	OGLDriver::getInstance().getDefaultFramebuffer()->bind();

	_displayRenderTexture->getTexture(0)->generateMipmap();
}


void DisplayComponent::setText(DisplayText& text)
{
	_displayText = text;

	generateMatrixTexture();

	generateTexture();
}


void DisplayComponent::setText(std::string text)
{
	_text = text;
}


std::string DisplayComponent::getText()
{
	return _text;
}


void DisplayComponent::update(float deltaTime)
{
	RenderObject* renderObject = static_cast<RenderObject*>(_object->getComponent(CT_RENDER_OBJECT));

	renderObject->getMaterial(0)->diffuseTexture = _displayRenderTexture->getTexture(0);
}
