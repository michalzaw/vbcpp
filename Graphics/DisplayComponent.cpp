#include "DisplayComponent.h"

#include "../Scene/SceneObject.h"

#include "../Graphics/RenderObject.h"
#include "../Graphics/Renderer.h"
#include "RDisplayFont.h"

DisplayComponent::DisplayComponent(int displayWidth, int displayHeight)
	: Component(CT_DISPLAY),
	_displayWidth(displayWidth), _displayHeight(displayHeight)
{
	//_matrixTexture = ResourceManager::getInstance().loadTexture("tab1.png");
	_ledOnTexture = ResourceManager::getInstance().loadTexture("doron.bmp");
	_ledOffTexture = ResourceManager::getInstance().loadTexture("doroff.bmp");
	_matrixTexture = generateMatrixTexture();

	_matrixTexture->setFiltering(TFM_NEAREST, TFM_NEAREST);

	int ledWidth = _ledOnTexture->getSize().x;
	int ledHeight = _ledOnTexture->getSize().y;

	_displayRenderTexture = OGLDriver::getInstance().createFramebuffer();
	_displayRenderTexture->addTexture(TF_RGBA, _displayWidth * ledWidth, _displayHeight * ledHeight);
	_displayRenderTexture->setViewport(UintRect(0, 0, _displayWidth * ledWidth, _displayHeight * ledHeight));

	generateTexture();
}


DisplayComponent::~DisplayComponent()
{

}


RTexture2D* DisplayComponent::generateMatrixTexture()
{
	/*const int HEIGHT = 16;
	int chars[48][HEIGHT];

	std::ifstream fstream("matryca16.csv");

	std::string line;
	int charIndex = 0;
	while (std::getline(fstream, line))
	{
		std::string cell;

		std::stringstream lineStream(line);

		int i = 0;
		while (std::getline(lineStream, cell, ';'))
		{
			chars[charIndex][i] = toInt(cell);
			++i;
		}

		++charIndex;
	}

	int charsWidth[48];
	for (int i = 0; i < 47; ++i)
	{
		int maxNumberInRow = 0;
		for (int j = 0; j < HEIGHT; ++j)
		{
			if (chars[i][j] > maxNumberInRow)
			{
				maxNumberInRow = chars[i][j];
			}

			std::cout << chars[i][j] << " ";
		}

		charsWidth[i] = ceil(log2(maxNumberInRow));
		std::cout << maxNumberInRow << " " << charsWidth[i] << std::endl;
		//system("pause");

	}*/

	RDisplayFont* font = new RDisplayFont("Displays/RG");
	RDisplayFontSize* fontSize = font->getFontInSize(4);




	std::string text = "101bis plac mlynarz";

	int beginX = 0;
	int beginY = 0;

	unsigned char* data = new unsigned char[_displayWidth * _displayHeight * 4];
	for (int i = 0; i < _displayWidth * _displayHeight * 4; ++i)
	{
		data[i] = 0;
	}

	//for (int i = 40; i < 48; ++i)
	for (int i = 0; i < text.size(); ++i)
	{
		int charIndex = 0;

		if (text[i] == ' ')
		{
			beginX += 2;
			continue;
		}
		else if (text[i] >= '0' && text[i] <= '9')
		{
			charIndex = (int)(text[i] - '0');
		}
		else
		{
			charIndex = (int)(text[i] - 'a') + 10;//i;//
		}

		int charWidth = 0;
		for (int row = 0; row < fontSize->height; ++row)
		{
			for (int x = 0; x < fontSize->charsWidth[charIndex]; ++x)
			{
				std::cout << text.size() << " " << fontSize->height << " " << fontSize->charsWidth[charIndex] << std::endl;
				//system("pause");

				int b = 1 << x;
				if (fontSize->chars[charIndex][row] & b)
				{
					int index = ((_displayHeight - (beginY + row) - 1) * _displayWidth + beginX + x) * 4;
					//int index = (beginX + x) * _displayHeight + beginY + row;
					data[index] = 255;
					data[index + 1] = 255;
					data[index + 2] = 255;
					data[index + 3] = 255;
				}
				//else
				{

				}
			}
		}
		beginX += fontSize->charsWidth[charIndex] + 1;
	}


	return new RTexture2D("", data, TF_RGBA, glm::uvec2(_displayWidth, _displayHeight));
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


	_displayRenderTexture->getTexture(0)->setFiltering(TFM_TRILINEAR, TFM_LINEAR);
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
