#include "SkeletalAnimationComponent.h"

#include <string>

#include "RAnimation.h"
#include "RenderObject.h"
#include "SkeletalAnimationHelperComponent.h"

#include "../Scene/SceneManager.h"
#include "../Scene/SceneObject.h"

#include "../Utils/RAnimatedModel.h"


SkeletalAnimationComponent::SkeletalAnimationComponent(RAnimation* animation)
	: Component(CT_SKELETAL_ANIMATION),
	_animation(animation),
	_finalBoneMatrices(MAX_BONES, glm::mat4(1.0f)),
	_currentTime(0.0f),
	_animationDuration(animation->getDuration()), _animationTicksPerSecond(animation->getTicksPerSecond()), _play(true),
	_animatedModel(nullptr)
{

}


SkeletalAnimationComponent::~SkeletalAnimationComponent()
{

}


void SkeletalAnimationComponent::onAttachedToScenObject()
{
	RenderObject* renderObject = static_cast<RenderObject*>(getSceneObject()->getComponent(CT_RENDER_OBJECT));
	if (renderObject == nullptr)
	{
		LOG_ERROR("ScaneObject has not a renderObject");
		return;
	}

	_animatedModel = dynamic_cast<RAnimatedModel*>(renderObject->getModel());
	if (_animatedModel == nullptr)
	{
		LOG_ERROR("SceneObject has not an animated model");
		return;
	}

	if (GameConfig::getInstance().mode == GM_EDITOR)
	{
		// create editor helper
		SkeletalAnimationHelperComponent* helper = getSceneObject()->getSceneManager()->getGraphicsManager()->addSkeletalAnimationHelper(new SkeletalAnimationHelperComponent(_animation, _animatedModel));;
		getSceneObject()->addComponent(helper);
	}
}


void SkeletalAnimationComponent::calculateBoneTransform(const AnimationNodeData* node, const glm::mat4& parentTransform /*= glm::mat4(1.0f)*/)
{
	const std::string& nodeName = node->name;

	glm::mat4 nodeTransform = node->transformation;

	auto animationBone = _animation->getBones().find(nodeName);
	if (animationBone != _animation->getBones().end())
	{
		Bone* bone = animationBone->second;

		nodeTransform = bone->calculateLocalTransform(_currentTime);
	}

	glm::mat4 globalTransform = parentTransform * nodeTransform;

	auto boneInfo = _animatedModel->getBoneInfos().find(nodeName);
	if (boneInfo != _animatedModel->getBoneInfos().end())
	{
		int index = boneInfo->second->id;
		const glm::mat4& offset = boneInfo->second->offset;

		_finalBoneMatrices[index] = globalTransform * offset;
	}

	for (int i = 0; i < node->children.size(); ++i)
	{
		calculateBoneTransform(&node->children[i], globalTransform);
	}
}


void SkeletalAnimationComponent::update(float deltaTime)
{
	if (_play)
	{
		_currentTime += deltaTime * _animationTicksPerSecond;
		_currentTime = fmod(_currentTime, _animationDuration);
	}

	calculateBoneTransform(_animation->getRootNode());
}
