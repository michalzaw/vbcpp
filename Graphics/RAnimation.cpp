#include "RAnimation.h"


Bone::Bone(int id, const std::string& name)
	: _id(id), _name(name), _localTransform(1.0f)
{

}


Bone::~Bone()
{

}


RAnimation::RAnimation(const std::string& path)
	: Resource(RT_ANIMATION, path)
{

}


RAnimation::~RAnimation()
{

}
