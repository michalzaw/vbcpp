#ifndef ANIMATIONMODELDATA_H_INCLUDED
#define ANIMATIONMODELDATA_H_INCLUDED


#include <string>
#include <vector>

#include <glm/glm.hpp>


class SceneObject;


struct AnimationNodeData final
{
	glm::mat4 transformation;
	std::string name;
	std::vector<AnimationNodeData> children;

	SceneObject* _helperSceneObject = nullptr; // todo: animation - refactor

};


#endif // ANIMATIONMODELDATA_H_INCLUDED
