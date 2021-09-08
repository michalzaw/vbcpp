#ifndef PICKER_H_INCLUDED
#define PICKER_H_INCLUDED


#include <vector>

#include "GUIObject.h"
#include "Button.h"
#include "Label.h"


class Picker final : public GUIObject
{
	private:
		Button* _buttonArrowBack;
		Button* _buttonArrowForward;
		Image* _imageBackground;
		Label* _label;

		unsigned int _width;
		unsigned int _height;

		glm::vec2 _margin;

		glm::vec4 _backgroundColor;

		std::vector<std::string> _options;
		int _selectedOption;

		std::function<void(int, const std::string&)> _onValueChangedCallback;

		void setAllElementsPositions();
		void onValueChanged();

		inline void changedVerticesTransformMatrixParameters() override
		{

		}

		inline void changedTexCoordTransformMatrixParameters() override
		{

		}

	public:
		Picker(RFont* font, const std::vector<std::string>& options, unsigned int width, unsigned int height);
		~Picker();

		// https://stackoverflow.com/questions/29715420/overriding-overloaded-methods-hides-some-of-the-overloads
		// https://stackoverflow.com/questions/5636289/overloaded-method-not-seen-in-subclass
		using GUIObject::setPosition;
		using GUIObject::setScale;
		using GUIObject::setRotation;
		using GUIObject::move;
		using GUIObject::scale;
		using GUIObject::rotate;

		void setPosition(glm::vec3 position) override;
		void setScale(glm::vec2 scale) override;
		void setRotation(float angle) override;          // Radians

		void move(glm::vec3 delta) override;
		void scale(glm::vec2 delta) override;
		void rotate(float angle) override;

		void setTextScale(float scale);

		//glm::vec2 getRealSize();

		void setWidth(unsigned int width);
		unsigned int getWidth();

		void setHeight(unsigned int height);
		unsigned int getHeight();

		void setMargin(glm::vec2 margin);
		glm::vec2 getMargin();

		void setBackgroundColor(glm::vec4 color);
		glm::vec4 getBackgroundColor();

		void setSelectedOption(int selectedOptionIndex);
		void setSelectedOption(const std::string& selectedOption);
		int getSelectedOptionIndex();
		const std::string& getSelectedOptionValue();

		void update(float deltaTime) override;

		void addDataToRenderList(GUIRenderList* renderList) override;

		void setOnValueChangedCallback(const std::function<void(int, const std::string&)>& onValueChangedCallback);
};


#endif // PICKER_H_INCLUDED