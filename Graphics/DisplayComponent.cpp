#include "DisplayComponent.h"

#include "../Scene/SceneObject.h"

#include "../Graphics/RenderObject.h"
#include "../Graphics/Renderer.h"


DisplayComponent::DisplayComponent(RDisplayFont* font, int displayWidth, int displayHeight)
	: Component(CT_DISPLAY),
	_font(font), _displayWidth(displayWidth), _displayHeight(displayHeight)
{
	_tabGeneratorShader = ResourceManager::getInstance().loadShader("Shaders/quad.vert", "Shaders/tabGenerator.frag");

	_matrixTextureData = new unsigned char[_displayWidth * _displayHeight * 4];
	for (int i = 0; i < _displayWidth * _displayHeight * 4; ++i)
	{
		_matrixTextureData[i] = 0;
	}
	_matrixTexture = new RTexture2D("", _matrixTextureData, TF_RGBA, glm::uvec2(_displayWidth, _displayHeight));
	_matrixTexture->setFiltering(TFM_NEAREST, TFM_NEAREST);

	_ledOnTexture = ResourceManager::getInstance().loadTexture("doron.bmp");
	_ledOffTexture = ResourceManager::getInstance().loadTexture("doroff.bmp");

	int pointWidth = _ledOnTexture->getSize().x;
	int pointHeight = _ledOnTexture->getSize().y;

	_displayRenderTexture = OGLDriver::getInstance().createFramebuffer();
	_displayRenderTexture->addTexture(TF_RGBA, _displayWidth * pointWidth, _displayHeight * pointHeight);
	_displayRenderTexture->setViewport(UintRect(0, 0, _displayWidth * pointWidth, _displayHeight * pointHeight));
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
	else if (c >= 'a' && c <= 'z')
	{
		return (int)(c - 'a') + 10;
	}
	else if (c == '-')
	{
		return 36;
	}
	else if (c == '.')
	{
		return 37;
	}
	else if (c == '"')
	{
		return 38;
	}
	else
	{
		return 37;
	}
}


void DisplayComponent::generateMatrixTexture()
{
	for (int i = 0; i < _displayWidth * _displayHeight * 4; ++i)
	{
		_matrixTextureData[i] = 0;
	}

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
		else
		{
			beginX = headEnd;
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
		else
		{
			beginX = headEnd;
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
					int pointX = beginX + x;
					int pointY = (_displayHeight - (beginY + row) - 1);
					if (pointX < _displayWidth && pointY < _displayHeight)
					{
						int index = (pointY * _displayWidth + pointX) * 4;
						data[index] = 255;
						data[index + 1] = 255;
						data[index + 2] = 255;
						data[index + 3] = 255;
					}
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
	_displayRenderTexture->bind();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	VBO* quadVBO = Renderer::getInstance().getQuadVbo();
	quadVBO->bind();

	_tabGeneratorShader->enable();
	_tabGeneratorShader->bindTexture(_tabGeneratorShader->getUniformLocation("matrixTexture"), _matrixTexture);
	_tabGeneratorShader->bindTexture(_tabGeneratorShader->getUniformLocation("ledOnTexture"), _ledOnTexture);
	_tabGeneratorShader->bindTexture(_tabGeneratorShader->getUniformLocation("ledOffTexture"), _ledOffTexture);
	_tabGeneratorShader->setUniform(_tabGeneratorShader->getUniformLocation("displayWidth"), _displayWidth);
	_tabGeneratorShader->setUniform(_tabGeneratorShader->getUniformLocation("displayHeight"), _displayHeight);


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


DisplayText& DisplayComponent::getText()
{
	return _displayText;
}


void DisplayComponent::init()
{
	RenderObject* renderObject = static_cast<RenderObject*>(_object->getComponent(CT_RENDER_OBJECT));

	renderObject->getMaterial(0)->diffuseTexture = _displayRenderTexture->getTexture(0);
}
