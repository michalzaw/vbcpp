#ifndef GUIZMOWPARAMS_H_INCLUDED
#define GUIZMOWPARAMS_H_INCLUDED


#include <ImGuizmo.h>


const int availableTranslationSnapValues[] = { 1, 5, 10 };
const int availableRotationSnapValues[] = { 1, 5, 10 };
const int availableScaleSnapValues[] = { 1, 5, 10 };


struct GuizmoParams final
{
	ImGuizmo::OPERATION currentOperation;
	ImGuizmo::MODE currentMode;
	
	bool translationSnap;
	bool rotationSnap;
	bool scaleSnap;

	int currentTranslationSnapValueIndex;
	int currentRotationSnapValueIndex;
	int currentScaleSnapValueIndex;

	// do not use immediately, call function getCurrentSnapValues
	float currentSnapValues[3];

	GuizmoParams()
		: currentOperation(ImGuizmo::TRANSLATE), currentMode(ImGuizmo::LOCAL),
		translationSnap(false), rotationSnap(false), scaleSnap(false),
		currentTranslationSnapValueIndex(0), currentRotationSnapValueIndex(0), currentScaleSnapValueIndex(0)
	{

	}

	float* getCurrentSnapValues()
	{
		if (currentOperation == ImGuizmo::TRANSLATE && translationSnap)
		{
			currentSnapValues[0] = currentSnapValues[1] = currentSnapValues[2] = availableTranslationSnapValues[currentTranslationSnapValueIndex];

			return &currentSnapValues[0];
		}
		else if (currentOperation == ImGuizmo::ROTATE && rotationSnap)
		{
			currentSnapValues[0] = currentSnapValues[1] = currentSnapValues[2] = availableRotationSnapValues[currentRotationSnapValueIndex];

			return &currentSnapValues[0];
		}
		else if (currentOperation == ImGuizmo::SCALE && scaleSnap)
		{
			currentSnapValues[0] = currentSnapValues[1] = currentSnapValues[2] = availableScaleSnapValues[currentScaleSnapValueIndex];

			return &currentSnapValues[0];
		}
		else
		{
			return NULL;
		}
	}
};


#endif // GUIZMOWPARAMS_H_INCLUDED
