#include "SkeletalAnimationComponent.h"

#include <string>

#include "RAnimation.h"
#include "RenderObject.h"
#include "SkeletalAnimationHelperComponent.h"

#include "../Scene/SceneManager.h"
#include "../Scene/SceneObject.h"

#include "../Utils/GlmUtils.h"
#include "../Utils/RAnimatedModel.h"


SkeletalAnimationComponent::SkeletalAnimationComponent(RAnimation* animation)
	: Component(CT_SKELETAL_ANIMATION),
	_finalBoneMatrices(MAX_BONES, glm::mat4(1.0f)),
	_currentTime(0.0f),
	_play(true),
	_lockRootBoneTranslation(true), _rootBoneName(""), _rootNodeIndex(-1),
	_scale(1.0f),
	_animatedModel(nullptr),
	_translationMatrices(MAX_BONES, glm::mat4(1.0f))
{
	setAnimation(animation);
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

	calculateModelBonesTranslations(_animatedModel->getBonesRootNode());

	if (GameConfig::getInstance().mode == GM_EDITOR)
	{
		// create editor helper
		SkeletalAnimationHelperComponent* helper = getSceneObject()->getSceneManager()->getGraphicsManager()->addSkeletalAnimationHelper(new SkeletalAnimationHelperComponent(_animation, _animatedModel));;
		getSceneObject()->addComponent(helper);
	}
}


void SkeletalAnimationComponent::calculateModelBonesTranslations(const AnimationNodeData* node)
{
	const std::string& nodeName = node->name;

	auto boneInfo = _animatedModel->getBoneInfos().find(nodeName);
	if (boneInfo != _animatedModel->getBoneInfos().end())
	{
		int index = boneInfo->second->id;
		
		_translationMatrices[index] = glm::translate(GlmUtils::getTranslationFromMatrix(node->transformation));
	}

	for (int i = 0; i < node->children.size(); ++i)
	{
		calculateModelBonesTranslations(& node->children[i]);
	}
}


glm::mat4 SkeletalAnimationComponent::calculateBoneTranslation(Bone* bone, const std::unordered_map<std::string, BoneInfo*>::const_iterator& boneInfoIterator)
{
	if (boneInfoIterator != _animatedModel->getBoneInfos().end() &&
		(boneInfoIterator->second->id != _rootNodeIndex || _lockRootBoneTranslation))
	{
		return _translationMatrices[boneInfoIterator->second->id];
	}
	else
	{
		return bone->calculatePosition(_currentTime + _startFrame);
	}
}


void SkeletalAnimationComponent::calculateBoneTransform(const AnimationNodeData* node, const glm::mat4& parentTransform /*= glm::mat4(1.0f)*/)
{
	const std::string& nodeName = node->name;

	glm::mat4 nodeTransform = node->transformation;

	auto animationBone = _animation->getBones().find(nodeName);
	auto boneInfo = _animatedModel->getBoneInfos().find(nodeName);

	if (animationBone != _animation->getBones().end())
	{
		Bone* bone = animationBone->second;

		//nodeTransform = bone->calculateLocalTransform(_currentTime + _startFrame);
		glm::mat4 boneTranslation = calculateBoneTranslation(bone, boneInfo);
		glm::mat4 boneRotation = bone->calculateRotation(_currentTime + _startFrame);
		glm::mat4 boneScale = bone->calculateScale(_currentTime + _startFrame);

		nodeTransform = boneTranslation * boneRotation * boneScale;
	}

	glm::mat4 globalTransform = parentTransform * nodeTransform;

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

		float animationDuration = getAnimationDuration();

		_currentTime = fmod(_currentTime, animationDuration);

		calculateBoneTransform(_animation->getRootNode());
	}
}


void SkeletalAnimationComponent::recalculateAllBonesTransform()
{
	calculateBoneTransform(_animation->getRootNode());
}


void SkeletalAnimationComponent::setAnimation(RAnimation* animation)
{
	_animation = animation;
	_startFrame = 0;
	_endFrame = animation->getDuration();
	_animationTicksPerSecond = animation->getTicksPerSecond();
}


void SkeletalAnimationComponent::setRootBone(const std::string& boneName)
{
	if (boneName.empty())
	{
		_rootBoneName = "";
		_rootNodeIndex = -1;

		return;
	}

	auto boneInfo = _animatedModel->getBoneInfos().find(boneName);
	if (boneInfo != _animatedModel->getBoneInfos().end())
	{
		_rootBoneName = boneName;
		_rootNodeIndex = boneInfo->second->id;
	}
	else
	{
		LOG_WARNING("Invalid bone name");
	}
}
