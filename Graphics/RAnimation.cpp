#include "RAnimation.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/quaternion.hpp>


Bone::Bone(const std::string& name)
	:  _name(name)
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


glm::mat4 Bone::calculatePosition(float animationTime)
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


glm::mat4 Bone::calculateRotation(float animationTime)
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


glm::mat4 Bone::calculateScale(float animationTime)
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


glm::mat4 Bone::calculateLocalTransform(float animationTime)
{
	glm::mat4 translation = calculatePosition(animationTime);
	glm::mat4 rotation = calculateRotation(animationTime);
	glm::mat4 scale = calculateScale(animationTime);

	return translation * rotation * scale;
}


RAnimation::RAnimation(const std::string& path)
	: Resource(RT_ANIMATION, path),
	_duration(0.0f), _ticksPerSecond(0)
{

}


RAnimation::~RAnimation()
{
	for (auto& pair : _bones)
	{
		delete pair.second;
	}
}


const std::vector<std::string>& RAnimation::getBonesNames()
{
	if (_bones.size() > 0 && _boneNames.size() == 0)
	{
		for (auto bone : _bones)
		{
			_boneNames.push_back(bone.first);
		}
	}

	return _boneNames;
}
