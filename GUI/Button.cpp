#include "Button.h"

#include <GLFW/glfw3.h>

#include "../Utils/InputSystem.h"


Button::Button(RTexture* texture, RFont* font, const std::string& text)
	: _label(nullptr)
{
	_image = new Image(texture);

	if (font != nullptr)
	{
		_label = new Label(font);
		_label->setText(text);
	}
}


Button::Button(RTexture* texture, const UintRect& textureRect, RFont* font, const std::string& text)
{
	_image = new Image(texture, textureRect);

	if (_label != nullptr)
	{
		_label = new Label(font);
		_label->setText(text);
	}
}


Button::~Button()
{
	delete _image;
	delete _label;
}


void Button::setPosition(glm::vec3 position)
{
    GUIObject::setPosition(position);

	_image->setPosition(position);
	if (_label != nullptr)
	{
		_label->setPosition(position);
	}
}


void Button::setScale(glm::vec2 scale)
{
    GUIObject::setScale(scale);

	_image->setScale(scale);
	if (_label != nullptr)
	{
		_label->setScale(scale);
	}
}


void Button::setRotation(float angle)
{
    GUIObject::setRotation(angle);

	_image->setRotation(angle);
	if (_label != nullptr)
	{
		_label->setRotation(angle);
	}
}


void Button::move(glm::vec3 delta)
{
	GUIObject::move(delta);

	_image->move(delta);
	if (_label != nullptr)
	{
		_label->move(delta);
	}
}


void Button::scale(glm::vec2 delta)
{
	GUIObject::scale(delta);

	_image->scale(delta);
	if (_label != nullptr)
	{
		_label->scale(delta);
	}
}


void Button::rotate(float angle)
{
	GUIObject::rotate(angle);

	_image->rotate(angle);
	if (_label != nullptr)
	{
		_label->rotate(angle);
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
	input.getCursorPosition(&mouseX, &mouseY);

	if (mouseX > _position.x && mouseX < _position.x + getRealSize().x &&
		mouseY > _position.y && mouseY < _position.y + getRealSize().y)
	{
		if (input.isMouseButtonPressed(GLFW_MOUSE_BUTTON_LEFT))
		{
			if (_onClickCallback)
			{
				_onClickCallback();
			}
		}
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
