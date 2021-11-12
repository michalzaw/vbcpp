#include "Button.h"

#include <GLFW/glfw3.h>

#include "../Utils/InputSystem.h"


Button::Button(RTexture* texture, RTexture* textureHovered, RFont* font, const std::string& text)
	: _label(nullptr), _width(texture->getSize().x), _height(texture->getSize().y)
{
	_image = new Image(texture);

	if (font != nullptr)
	{
		_label = new Label(font);
		_label->setText(text);
	}

	_texture = texture;
	_textureHovered = textureHovered != nullptr ? textureHovered : texture;

	setAllElementsPositions();
}


Button::Button(RTexture* texture, const UintRect& textureRect, RFont* font, const std::string& text)
{
	_image = new Image(texture, textureRect);

	if (_label != nullptr)
	{
		_label = new Label(font);
		_label->setText(text);
	}

	setAllElementsPositions();
}


Button::~Button()
{
	delete _image;
	delete _label;
}


void Button::setAllElementsPositions()
{
	_image->setPosition(_position);
	_image->setScale(static_cast<float>(_width) / _image->getTexture()->getSize().x,
					 static_cast<float>(_height) / _image->getTexture()->getSize().y);

	if (_label != nullptr)
	{
		_label->setPosition(_position.x + (_width - _label->getWidth()) / 2,
							_position.y + (_height - _label->getHeight()) / 2);
	}
}


void Button::setPosition(glm::vec3 position)
{
    GUIObject::setPosition(position);

	/*_image->setPosition(position);
	if (_label != nullptr)
	{
		_label->setPosition(position);
	}*/

	setAllElementsPositions();
}


void Button::setScale(glm::vec2 scale)
{
    GUIObject::setScale(scale);

	_image->setScale(scale);
	if (_label != nullptr)
	{
		_label->setScale(scale);
	}

	setAllElementsPositions();
}


void Button::setRotation(float angle)
{
    GUIObject::setRotation(angle);

	_image->setRotation(angle);
	if (_label != nullptr)
	{
		_label->setRotation(angle);
	}

	setAllElementsPositions();
}


void Button::move(glm::vec3 delta)
{
	GUIObject::move(delta);

	/*_image->move(delta);
	if (_label != nullptr)
	{
		_label->move(delta);
	}*/

	setAllElementsPositions();
}


void Button::scale(glm::vec2 delta)
{
	GUIObject::scale(delta);

	_image->scale(delta);
	if (_label != nullptr)
	{
		_label->scale(delta);
	}

	setAllElementsPositions();
}


void Button::rotate(float angle)
{
	GUIObject::rotate(angle);

	_image->rotate(angle);
	if (_label != nullptr)
	{
		_label->rotate(angle);
	}

	setAllElementsPositions();
}


void Button::setWidth(unsigned int width)
{
	_width = width;

	setAllElementsPositions();
}


unsigned int Button::getWidth()
{
	return _width;
}


void Button::setHeight(unsigned int height)
{
	_height = height;

	setAllElementsPositions();
}


unsigned int Button::getHeight()
{
	return _height;
}


void Button::setTextColor(glm::vec4 color)
{
	if (_label != nullptr)
	{
		_label->setColor(color);
	}
}


glm::vec2 Button::getRealSize()
{
	return _image->getRealSize();
}


void Button::update(float deltaTime)
{
	InputSystem& input = InputSystem::getInstance();

	double mouseX, mouseY;
	input.getCursorPosition(&mouseX, &mouseY, true);

	if (mouseX > _position.x && mouseX < _position.x + getRealSize().x &&
		mouseY > _position.y && mouseY < _position.y + getRealSize().y)
	{
		_image->setTexture(_textureHovered);

		if (input.isMouseButtonPressed(GLFW_MOUSE_BUTTON_LEFT))
		{
			if (_onClickCallback)
			{
				_onClickCallback();
			}
		}
	}
	else
	{
		_image->setTexture(_texture);
	}
}


void Button::addDataToRenderList(GUIRenderList* renderList)
{
	_image->addDataToRenderList(renderList);
	if (_label != nullptr)
	{
		_label->addDataToRenderList(renderList);
	}
}


void Button::setOnClickCallback(const std::function<void(void)>& onClickCallback)
{
	_onClickCallback = onClickCallback;
}
