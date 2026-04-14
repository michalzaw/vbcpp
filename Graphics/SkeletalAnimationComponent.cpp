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


SkeletalAnimationComponent::SkeletalAnimationComponent()
	: Component(CT_SKELETAL_ANIMATION),
	_currentAnimationState(nullptr), _nextAnimationState(nullptr),
	_finalBoneMatrices(MAX_BONES, glm::mat4(1.0f)), _finalBoneMatricesIsCalculated(false),
	_animationSpeed(1.0f),
	_play(true),
	_lockRootBoneTranslation(true), _rootBoneName(""), _rootNodeIndex(-1),
	_endToStartFrameBlending(true), _endToStartFrameBlendingTime(20.0f),
	_stateBlendingDuration(1.0f), _stateBlndingTime(0.0f), _stateBlendingFactor(0.0f),
	_scale(1.0f),
	_animatedModel(nullptr),
	_translationMatrices(MAX_BONES, glm::mat4(1.0f))
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

	calculateModelBonesTranslations(_animatedModel->getBonesRootNode());

	// todo: _animation
	/*if (GameConfig::getInstance().mode == GM_EDITOR)
	{
		// create editor helper
		SkeletalAnimationHelperComponent* helper = getSceneObject()->getSceneManager()->getGraphicsManager()->addSkeletalAnimationHelper(new SkeletalAnimationHelperComponent(_animation, _animatedModel));;
		getSceneObject()->addComponent(helper);
	}*/
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
	if (boneInfoIterator != _animatedModel->getBoneInfos().end() && boneInfoIterator->second->id == _rootNodeIndex)
	{
		_rootBoneTranslation = bone->calculatePosition(_currentAnimationState->currentTime + _currentAnimationState->animation->getStartFrame());
	}

	if (boneInfoIterator != _animatedModel->getBoneInfos().end() &&
		(boneInfoIterator->second->id != _rootNodeIndex || _lockRootBoneTranslation))
	{
		return _translationMatrices[boneInfoIterator->second->id];
	}
	else
	{
		return bone->calculatePosition(_currentAnimationState->currentTime + _currentAnimationState->animation->getStartFrame());
	}
}


glm::quat SkeletalAnimationComponent::calculateBoneRotation(RAnimation* animation, Bone* bone, float currentTime)
{
	float duration = animation->getDuration();
	float timeToEnd = duration - currentTime;

	return timeToEnd >= _endToStartFrameBlendingTime || !_endToStartFrameBlending
		? bone->calculateRotationQuat(currentTime + animation->getStartFrame())
		: calculateRotationInterpolated(animation, bone, currentTime + animation->getStartFrame(), timeToEnd, duration);
}


glm::quat SkeletalAnimationComponent::calculateRotationInterpolated(RAnimation* animation, Bone* bone, float animationTime, float timeToEnd, float duration)
{
	float blendFactor = 1.0f - (timeToEnd / _endToStartFrameBlendingTime);

	glm::quat endRotation = bone->calculateRotationQuat(animationTime);
	glm::quat startRotation = bone->calculateRotationQuat(std::max(animationTime - animation->getStartFrame() - duration, static_cast<float>(animation->getStartFrame())));

	glm::quat rotation = glm::slerp(endRotation, startRotation, blendFactor);

	return rotation;
}


void SkeletalAnimationComponent::calculateBoneTransform(const AnimationNodeData* node, const glm::mat4& parentTransform /*= glm::mat4(1.0f)*/)
{
	// todo: _animation
	const std::string& nodeName = node->name;

	glm::mat4 nodeTransform = node->transformation;

	auto animationBone = _currentAnimationState->animation->getBones().find(nodeName);
	auto boneInfo = _animatedModel->getBoneInfos().find(nodeName);

	glm::mat4 boneTranslation(1.0f);
	glm::quat boneRotation;
	glm::mat4 boneScale(1.0f);

	if (animationBone != _currentAnimationState->animation->getBones().end())
	{
		Bone* bone = animationBone->second;

		//nodeTransform = bone->calculateLocalTransform(_currentTime + _startFrame);
		boneTranslation = calculateBoneTranslation(bone, boneInfo);
		boneRotation = calculateBoneRotation(_currentAnimationState->animation, bone, _currentAnimationState->currentTime);
		boneScale = bone->calculateScale(_currentAnimationState->currentTime + _currentAnimationState->animation->getStartFrame());

		nodeTransform = boneTranslation * glm::toMat4(boneRotation) * boneScale;
	}

	if (_nextAnimationState != nullptr)
	{
		auto animationBone2 = _nextAnimationState->animation->getBones().find(nodeName);
		if (animationBone2 != _nextAnimationState->animation->getBones().end())
		{
			Bone* bone = animationBone2->second;

			//nodeTransform = bone->calculateLocalTransform(_currentTime + _startFrame);
			glm::mat4 boneTranslation2 = calculateBoneTranslation(bone, boneInfo);
			glm::quat boneRotation2 = calculateBoneRotation(_nextAnimationState->animation, bone, _nextAnimationState->currentTime);
			glm::mat4 boneScale2 = bone->calculateScale(_nextAnimationState->currentTime + _nextAnimationState->animation->getStartFrame());

			glm::quat boneRotationFinal = glm::slerp(boneRotation, boneRotation2, _stateBlendingFactor);

			nodeTransform = boneTranslation * glm::toMat4(boneRotationFinal) * boneScale;
		}
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
		if (_currentAnimationState)
		{
			_currentAnimationState->currentTime += deltaTime * _currentAnimationState->animation->getTicksPerSecond() * _animationSpeed;
			_currentAnimationState->currentTime = fmod(_currentAnimationState->currentTime, _currentAnimationState->animation->getDuration());
		}

		if (_nextAnimationState != nullptr)
		{
			_nextAnimationState->currentTime += deltaTime * _nextAnimationState->animation->getTicksPerSecond() * _animationSpeed;
			_nextAnimationState->currentTime = fmod(_nextAnimationState->currentTime, _nextAnimationState->animation->getDuration());

			_stateBlndingTime += deltaTime;
			_stateBlendingFactor = _stateBlndingTime / _stateBlendingDuration;

			if (_stateBlendingFactor >= 1.0f)
			{
				_currentAnimationState = _nextAnimationState;
				_nextAnimationState = nullptr;
			}
		}

		_finalBoneMatricesIsCalculated = false;
	}
}


void SkeletalAnimationComponent::recalculateAllBonesTransform()
{
	_finalBoneMatricesIsCalculated = false;
}


void SkeletalAnimationComponent::addAnimationState(AnimationState&& animationState)
{
	_animationStates.insert(std::make_pair(animationState.name, animationState));
	
	if (_currentAnimationState == nullptr)
	{
		_currentAnimationState = &_animationStates.begin()->second;
	}
}


void SkeletalAnimationComponent::setCurrentAnimationState(const std::string& name)
{
	const auto& animationState = _animationStates.find(name);
	if (animationState != _animationStates.end())
	{
		_currentAnimationState = &animationState->second;
	}
	else
	{
		LOG_WARNING("Invalid animation state name: " + name);
	}
}


void SkeletalAnimationComponent::setNextAnimationState(const std::string& name)
{
	const auto& animationState = _animationStates.find(name);
	if (animationState != _animationStates.end())
	{
		_nextAnimationState = &animationState->second;

		_stateBlndingTime = 0.0f;
		_stateBlendingFactor = 0.0f;
	}
	else
	{
		LOG_WARNING("Invalid animation state name: " + name);
	}
}


const std::vector<glm::mat4>& SkeletalAnimationComponent::getFinalBoneMatrices()
{
	if (!_finalBoneMatricesIsCalculated)
	{
		calculateBoneTransform(_currentAnimationState->animation->getRootNode());

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
	// todo: _animation
	RAnimation* animation = _currentAnimationState->animation;

	if (_rootBoneName != "")
	{
		auto bone = animation->getBones().find(_rootBoneName);
		if (bone != animation->getBones().end())
		{
			glm::mat4 transformMatrix = bone->second->calculatePosition(animation->getStartFrame());

			return glm::vec3(transformMatrix * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
		}
	}

	return glm::vec3(0.0f, 0.0f, 0.0f);
}


glm::vec3 SkeletalAnimationComponent::getRootBonePositionInEndFrame()
{
	// todo: _animation
	RAnimation* animation = _currentAnimationState->animation;

	if (_rootBoneName != "")
	{
		auto bone = animation->getBones().find(_rootBoneName);
		if (bone != animation->getBones().end())
		{
			glm::mat4 transformMatrix = bone->second->calculatePosition(animation->getEndFrame() - 1);

			return glm::vec3(transformMatrix * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
		}
	}

	return glm::vec3(0.0f, 0.0f, 0.0f);
}
