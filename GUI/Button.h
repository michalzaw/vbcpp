#ifndef BUTTON_H_INCLUDED
#define BUTTON_H_INCLUDED


#include <functional>

#include "GUIObject.h"
#include "Image.h"
#include "Label.h"


class Button final : public GUIObject
{
	private:
		Image* _image;
		Label* _label;

		std::function<void(void)> _onClickCallback;

		inline void changedVerticesTransformMatrixParameters() override
		{

		}

		inline void changedTexCoordTransformMatrixParameters() override
		{

		}

	public:
		Button(RTexture* texture, RFont* font = nullptr, const std::string& text = "");
		Button(RTexture* texture, const UintRect& textureRect, RFont* font = nullptr, const std::string& text = "");

		~Button();

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

		glm::vec2 getRealSize();

		void update(float deltaTime) override;

		void addDataToRenderList(GUIRenderList* renderList) override;

		void setOnClickCallback(const std::function<void(void)>& onClickCallback);

};


#endif // BUTTON_H_INCLUDED