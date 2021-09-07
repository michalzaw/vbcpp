#include "Picker.h"

#include "../Utils/ResourceManager.h"


Picker::Picker(RFont* font, const std::vector<std::string>& options, unsigned int width, unsigned int height)
	: _width(width), _height(height), _margin(5.0f, 5.0f), _backgroundColor(1.0f, 1.0f, 1.0f, 0.0f),
	_options(options), _selectedOption(0)
{
	_imageBackground = new Image(ResourceManager::getInstance().loadOneColorTexture(_backgroundColor));

	_buttonArrowBack = new Button(ResourceManager::getInstance().loadTexture("Data/arrow_back2.png"), ResourceManager::getInstance().loadTexture("Data/arrow_back2_2.png"));
	_buttonArrowForward = new Button(ResourceManager::getInstance().loadTexture("Data/arrow_forward2.png"), ResourceManager::getInstance().loadTexture("Data/arrow_forward2_2.png"));

	_label = new Label(font);
	_label->setText(_options[_selectedOption]);
	_label->setColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));

	setAllElementsPositions();

	_buttonArrowBack->setOnClickCallback([this]()
		{
			--_selectedOption;
			if (_selectedOption < 0)
			{
				_selectedOption = _options.size() - 1;
			}

			_label->setText(_options[_selectedOption]);

			setAllElementsPositions();
		});

	_buttonArrowForward->setOnClickCallback([this]()
		{

			++_selectedOption;
			if (_selectedOption >= _options.size())
			{
				_selectedOption = 0;
			}

			_label->setText(_options[_selectedOption]);

			setAllElementsPositions();
		});
}


Picker::~Picker()
{
	delete _buttonArrowBack;
	delete _buttonArrowForward;
	delete _label;
}


void Picker::setAllElementsPositions()
{
	_buttonArrowBack->setPosition(_position.x + _margin.x, _position.y + _margin.y);
	_buttonArrowForward->setPosition(_position.x + _width - _buttonArrowForward->getRealSize().x - _margin.x, _position.y + _margin.y);

	_label->setPosition(_position.x + (_width - _label->getWidth()) / 2.0f, _position.y + _margin.y);

	_imageBackground->setPosition(_position.x, _position.y);

	_imageBackground->setScale((_width) / _imageBackground->getTexture()->getSize().x,
							   (_height) / _imageBackground->getTexture()->getSize().y);
}


void Picker::setPosition(glm::vec3 position)
{
	GUIObject::setPosition(position);

	setAllElementsPositions();
}


void Picker::setScale(glm::vec2 scale)
{
	GUIObject::setScale(scale);

	_buttonArrowBack->setScale(scale);
	_buttonArrowForward->setScale(scale);
	_label->setScale(scale);

	setAllElementsPositions();
}


void Picker::setRotation(float angle)
{
	GUIObject::setRotation(angle);

	_buttonArrowBack->setRotation(angle);
	_buttonArrowForward->setRotation(angle);
	_label->setRotation(angle);
}


void Picker::move(glm::vec3 delta)
{
	GUIObject::move(delta);

	setAllElementsPositions();
}


void Picker::scale(glm::vec2 delta)
{
	GUIObject::scale(delta);

	_buttonArrowBack->scale(delta);
	_buttonArrowForward->scale(delta);
	_label->scale(delta);

	setAllElementsPositions();
}


void Picker::rotate(float angle)
{
	GUIObject::rotate(angle);

	_buttonArrowBack->rotate(angle);
	_buttonArrowForward->rotate(angle);
	_label->rotate(angle);
}


void Picker::setTextScale(float scale)
{
	_label->setScale(scale, scale);
}


void Picker::setWidth(unsigned int width)
{
	_width = width;

	setAllElementsPositions();
}


unsigned int Picker::getWidth()
{
	return _width;
}


void Picker::setHeight(unsigned int height)
{
	_height = height;

	setAllElementsPositions();
}


unsigned int Picker::getHeight()
{
	return _height;
}


void Picker::setMargin(glm::vec2 margin)
{
	_margin = margin;

	setAllElementsPositions();
}


glm::vec2 Picker::getMargin()
{
	return _margin;
}


void Picker::setBackgroundColor(glm::vec4 color)
{
	if (_backgroundColor != color)
	{
		_backgroundColor = color;
		_imageBackground->setTexture(ResourceManager::getInstance().loadOneColorTexture(_backgroundColor));
	}
}


glm::vec4 Picker::getBackgroundColor()
{
	return _backgroundColor;
}


void Picker::update(float deltaTime)
{
	_imageBackground->update(deltaTime);
	_buttonArrowBack->update(deltaTime);
	_buttonArrowForward->update(deltaTime);
	_label->update(deltaTime);
}


void Picker::addDataToRenderList(GUIRenderList* renderList)
{
	_imageBackground->addDataToRenderList(renderList);
	_buttonArrowBack->addDataToRenderList(renderList);
	_buttonArrowForward->addDataToRenderList(renderList);
	_label->addDataToRenderList(renderList);
}
