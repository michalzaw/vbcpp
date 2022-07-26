#include "RAnimation.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/quaternion.hpp>


Bone::Bone(int id, const std::string& name)
	: _id(id), _name(name), _localTransform(1.0f)
{

}


Bone::~Bone()
{

}


float Bone::getTimeFactor(float lastKeyframeTimestamp, float nextKeyframeTimestamp, float animationTime)
{
	float length = animationTime - lastKeyframeTimestamp;
	float framesDiff = nextKeyframeTimestamp - lastKeyframeTimestamp;

	return length / framesDiff;
}


int Bone::getPositionIndex(float animationTime)
{
	for (int i = 0; i < _positions.size() - 1; ++i)
	{
		if (animationTime < _positions[i + 1].timestamp)
		{
			return i;
		}
	}

	return 0;
}


int Bone::getRotationIndex(float animationTime)
{
	for (int i = 0; i < _rotations.size() - 1; ++i)
	{
		if (animationTime < _rotations[i + 1].timestamp)
		{
			return i;
		}
	}

	return 0;
}


int Bone::getScaleIndex(float animationTime)
{
	for (int i = 0; i < _scales.size() - 1; ++i)
	{
		if (animationTime < _scales[i + 1].timestamp)
		{
			return i;
		}
	}

	return 0;
}


glm::mat4 Bone::interpolatePosition(float animationTime)
{
	if (_positions.size() == 1)
	{
		return glm::translate(_positions[0].value);
	}

	int p0 = getPositionIndex(animationTime);
	int p1 = p0 + 1;

	float timeFactor = getTimeFactor(_positions[p0].timestamp, _positions[p1].timestamp, animationTime);
	glm::vec3 position = glm::mix(_positions[p0].value, _positions[p1].value, timeFactor);

	return glm::translate(position);
}


glm::mat4 Bone::interpolateRotation(float animationTime)
{
	if (_rotations.size() == 1)
	{
		return glm::toMat4(glm::normalize(_rotations[0].value));
	}

	int p0 = getRotationIndex(animationTime);
	int p1 = p0 + 1;

	float timeFactor = getTimeFactor(_rotations[p0].timestamp, _rotations[p1].timestamp, animationTime);
	glm::quat rotation = glm::slerp(_rotations[p0].value, _rotations[p1].value, timeFactor);

	return glm::toMat4(rotation);
}


glm::mat4 Bone::interpolateScale(float animationTime)
{
	if (_scales.size() == 1)
	{
		return glm::scale(_scales[0].value);
	}

	int p0 = getScaleIndex(animationTime);
	int p1 = p0 + 1;

	float timeFactor = getTimeFactor(_scales[p0].timestamp, _scales[p1].timestamp, animationTime);
	glm::vec3 scale = glm::mix(_scales[p0].value, _scales[p1].value, timeFactor);

	return glm::scale(scale);
}


void Bone::update(float animationTime)
{
	_translation = interpolatePosition(animationTime);
	_rotation = interpolateRotation(animationTime);
	_scale = interpolateScale(animationTime);
	
	_localTransform = _translation * _rotation * _scale;
}


RAnimation::RAnimation(const std::string& path)
	: Resource(RT_ANIMATION, path)
{

}


RAnimation::~RAnimation()
{

}


Bone* RAnimation::findBone(const std::string& name)
{
	auto result = std::find_if(_bones.begin(), _bones.end(), [&name](Bone& bone)
		{
			return bone.getName() == name;
		});

	if (result == _bones.end())
	{
		LOG_DEBUG("Invalid bone name: " + name);
		return nullptr;
	}

	return &(*result);
}
