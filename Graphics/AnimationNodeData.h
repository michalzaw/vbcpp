#ifndef ANIMATIONMODELDATA_H_INCLUDED
#define ANIMATIONMODELDATA_H_INCLUDED


#include <string>
#include <vector>

#include <glm/glm.hpp>


struct AnimationNodeData final
{
	glm::mat4 transformation;
	std::string name;
	std::vector<AnimationNodeData> children;

};


#endif // ANIMATIONMODELDATA_H_INCLUDED
