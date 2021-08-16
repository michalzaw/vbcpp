#ifndef PROGRESSBAR_H_INCLUDED
#define PROGRESSBAR_H_INCLUDED


#include "GUIObject.h"


class ProgressBar final : public GUIObject
{
	public:
		static const int TYPE1 = 0;
		static const int TYPE2 = 1;

	private:
		bool _isInitialized;

		bool _indeterminate;
		float _speed;
		int _type;
		float _value;
		
		glm::mat4 _verticesTransformMatrix;
		bool _verticesTransformMatrixIsCalculated;

		void createVBO();

		void calculateVerticesTransformMatrix();

		inline void changedVerticesTransformMatrixParameters() { _verticesTransformMatrixIsCalculated = false; }

	public:
		ProgressBar(bool indeterminate = true);
		~ProgressBar();

		void setSize(float size);
		float getSize();

		void setIndeterminate(bool indeterminate);
		bool isIndeterminate();

		void setSpeed(float speed);
		float getSpeed();

		void setType(int type);
		int getType();

		void setValue(float value);
		float getValue();

		void update(float deltaTime) override;

		glm::mat4& getVerticesTransformMatrix(unsigned int vboIndex = 0) override;

		void addDataToRenderList(GUIRenderList* renderList) override;
};


#endif // PROGRESSBAR_H_INCLUDED