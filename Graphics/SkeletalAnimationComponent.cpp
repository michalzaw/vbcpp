#include "SkeletalAnimationComponent.h"

#include <string>

#include <glm/gtx/quaternion.hpp>

#include "RAnimation.h"
#include "RenderObject.h"
#include "SkeletalAnimationHelperComponent.h"

#include "../Scene/SceneManager.h"
#include "../Scene/SceneObject.h"

#include "../Utils/GlmUtils.h"
#include "../Utils/RAnimatedModel.h"


SkeletalAnimationComponent::SkeletalAnimationComponent(RAnimation* animation, RAnimation* animation2/* = nullptr*/)
	: Component(CT_SKELETAL_ANIMATION),
	_finalBoneMatrices(MAX_BONES, glm::mat4(1.0f)), _finalBoneMatricesIsCalculated(false),
	_currentTime(0.0f),
	_currentTime2(0.0f),
	_animationSpeed(1.0f),
	_play(true),
	_lockRootBoneTranslation(true), _rootBoneName(""), _rootNodeIndex(-1),
	_endToStartFrameBlending(true), _blendingTime(20.0f),
	_blendingFactor(0.0f),
	_scale(1.0f),
	_animatedModel(nullptr),
	_translationMatrices(MAX_BONES, glm::mat4(1.0f))
{
	setAnimation(animation);
	_animation2 = animation2 != nullptr ? animation2 : _animation;
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
		return bone->calculatePosition(_currentTime + _animation->getStartFrame());
	}
}


glm::quat SkeletalAnimationComponent::calculateBoneRotation(RAnimation* animation, Bone* bone, float currentTime)
{
	float duration = animation->getDuration();
	float timeToEnd = duration - currentTime;

	return timeToEnd >= _blendingTime || !_endToStartFrameBlending
		? bone->calculateRotationQuat(currentTime + animation->getStartFrame())
		: calculateRotationInterpolated(animation, bone, currentTime + animation->getStartFrame(), timeToEnd, duration);
}


glm::quat SkeletalAnimationComponent::calculateRotationInterpolated(RAnimation* animation, Bone* bone, float animationTime, float timeToEnd, float duration)
{
	float blendFactor = 1.0f - (timeToEnd / _blendingTime);

	glm::quat endRotation = bone->calculateRotationQuat(animationTime);
	glm::quat startRotation = bone->calculateRotationQuat(std::max(animationTime - animation->getStartFrame() - duration, static_cast<float>(animation->getStartFrame())));

	glm::quat rotation = glm::slerp(endRotation, startRotation, blendFactor);

	return rotation;
}


void SkeletalAnimationComponent::calculateBoneTransform(const AnimationNodeData* node, const glm::mat4& parentTransform /*= glm::mat4(1.0f)*/)
{
	const std::string& nodeName = node->name;

	glm::mat4 nodeTransform = node->transformation;

	auto animationBone = _animation->getBones().find(nodeName);
	auto boneInfo = _animatedModel->getBoneInfos().find(nodeName);

	glm::mat4 boneTranslation(1.0f);
	glm::quat boneRotation;
	glm::mat4 boneScale(1.0f);

	if (animationBone != _animation->getBones().end())
	{
		Bone* bone = animationBone->second;

		//nodeTransform = bone->calculateLocalTransform(_currentTime + _startFrame);
		boneTranslation = calculateBoneTranslation(bone, boneInfo);
		boneRotation = calculateBoneRotation(_animation, bone, _currentTime);
		boneScale = bone->calculateScale(_currentTime + _animation->getStartFrame());
	}

	auto animationBone2 = _animation2->getBones().find(nodeName);
	if (animationBone2 != _animation2->getBones().end())
	{
		Bone* bone = animationBone2->second;

		//nodeTransform = bone->calculateLocalTransform(_currentTime + _startFrame);
		glm::mat4 boneTranslation2 = calculateBoneTranslation(bone, boneInfo);
		glm::quat boneRotation2 = calculateBoneRotation(_animation2, bone, _currentTime2);
		glm::mat4 boneScale2 = bone->calculateScale(_currentTime + _animation->getStartFrame());

		glm::quat boneRotationFinal = glm::slerp(boneRotation, boneRotation2, _blendingFactor);

		nodeTransform = boneTranslation * glm::toMat4(boneRotationFinal) * boneScale;
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
		_currentTime += deltaTime * _animation->getTicksPerSecond() * _animationSpeed;
		_currentTime2 += deltaTime * _animation2->getTicksPerSecond() * _animationSpeed;

		float animationDuration = _animation->getDuration();
		float animationDuration2 = _animation2->getDuration();

		_currentTime = fmod(_currentTime, animationDuration);
		_currentTime2 = fmod(_currentTime2, animationDuration2);

		_finalBoneMatricesIsCalculated = false;
	}
}


void SkeletalAnimationComponent::recalculateAllBonesTransform()
{
	_finalBoneMatricesIsCalculated = false;
}


void SkeletalAnimationComponent::setAnimation(RAnimation* animation)
{
	_animation = animation;
}


const std::vector<glm::mat4>& SkeletalAnimationComponent::getFinalBoneMatrices()
{
	if (!_finalBoneMatricesIsCalculated)
	{
		calculateBoneTransform(_animation->getRootNode());

		_finalBoneMatricesIsCalculated = true;
	}

	return _finalBoneMatrices;
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


glm::vec3 SkeletalAnimationComponent::getRootBonePositionInStartFrame()
{
	if (_rootBoneName != "")
	{
		auto bone = _animation->getBones().find(_rootBoneName);
		if (bone != _animation->getBones().end())
		{
			glm::mat4 transformMatrix = bone->second->calculatePosition(_animation->getStartFrame());

			return glm::vec3(transformMatrix * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
		}
	}

	return glm::vec3(0.0f, 0.0f, 0.0f);
}


glm::vec3 SkeletalAnimationComponent::getRootBonePositionInEndFrame()
{
	if (_rootBoneName != "")
	{
		auto bone = _animation->getBones().find(_rootBoneName);
		if (bone != _animation->getBones().end())
		{
			glm::mat4 transformMatrix = bone->second->calculatePosition(_animation->getEndFrame() - 1);

			return glm::vec3(transformMatrix * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
		}
	}

	return glm::vec3(0.0f, 0.0f, 0.0f);
}
