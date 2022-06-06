#include "SkeletalAnimationComponent.h"

#include <string>

#include "RAnimation.h"
#include "RenderObject.h"

#include "../Scene/SceneObject.h"

#include "../Utils/RAnimatedModel.h"



SkeletalAnimationComponent::SkeletalAnimationComponent(RAnimation* animation)
	: Component(CT_SKELETAL_ANIMATION),
	_animation(animation),
	_finalBoneMatrices(MAX_BONES, glm::mat4(1.0f)),
	_currentTime(0.0f), _deltaTime(0.0f),
	_animatedModel(nullptr)
{

}


SkeletalAnimationComponent::~SkeletalAnimationComponent()
{

}


void SkeletalAnimationComponent::onAttachedToScenObject()
{
	RenderObject* renderObject = static_cast<RenderObject*>(getSceneObject()->getComponent(CT_RENDER_OBJECT));
	_animatedModel = dynamic_cast<RAnimatedModel*>(renderObject->getModel());

	if (_animatedModel == nullptr)
	{
		LOG_ERROR("SceneObject has not an animated model");
	}
}


std::string SkeletalAnimationComponent::mapAnimationNodeNameToBoneNameInModel(const std::string& animationNodeName)
{
	auto mappedName = _animationNodeNameToBoneNameInModelMap.find(animationNodeName);
	if (mappedName == _animationNodeNameToBoneNameInModelMap.end())
	{
		return animationNodeName;
	}

	return mappedName->second;
}


void SkeletalAnimationComponent::calculateBoneTransform(const AnimationNodeData* node, const glm::mat4& parentTransform)
{
	const std::string& nodeName = node->name;
	glm::mat4 nodeTransform = node->transformation;

	Bone* bone = _animation->findBone(nodeName);
	if (bone != nullptr)
	{
		bone->update(_currentTime);
		nodeTransform = bone->getLocalTransform(); // todo: animation lepiej ¿eby update zwraca³ t¹ transformacjê, zamiast zapisywaæ j¹ wewn¹trz bone
	}

	glm::mat4 globalTransform = parentTransform * nodeTransform;


	//std::string boneName = mapAnimationNodeNameToBoneNameInModel(nodeName);
	//auto boneInfo = _animatedModel->getBoneInfos().find(boneName);
	for (const auto& pair : _animationNodeNameToBoneNameInModelMap)
	{
		if (pair.second == nodeName)
		{
			const std::string& boneName = pair.first;
			auto boneInfo = _animatedModel->getBoneInfos().find(boneName);


			if (boneInfo != _animatedModel->getBoneInfos().end())
			{
				int index = boneInfo->second.id;
				const glm::mat4& offset = boneInfo->second.offset;

				_finalBoneMatrices[index] = globalTransform * offset;
			}
		}
	}

	/*auto boneInfo = _animatedModel->getBoneInfos().find(nodeName);
	if (boneInfo != _animatedModel->getBoneInfos().end())
	{
		int index = boneInfo->second.id;
		const glm::mat4& offset = boneInfo->second.offset;

		_finalBoneMatrices[index] = globalTransform * offset;
	}*/


	for (int i = 0; i < node->children.size(); ++i)
	{
		calculateBoneTransform(&node->children[i], globalTransform);
	}
}


void SkeletalAnimationComponent::update(float deltaTime)
{
	_deltaTime = deltaTime;

	_currentTime += _deltaTime * _animation->getTicksPerSecond();
	_currentTime = fmod(_currentTime, _animation->getDuration());

	calculateBoneTransform(_animation->getRootNode());
}
