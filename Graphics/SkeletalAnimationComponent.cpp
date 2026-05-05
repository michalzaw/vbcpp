#include "SkeletalAnimationComponent.h"

#include <string>

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>

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

	/*if (GameConfig::getInstance().mode == GM_EDITOR)
	{
		// create editor helper
		SkeletalAnimationHelperComponent* helper = getSceneObject()->getSceneManager()->getGraphicsManager()->addSkeletalAnimationHelper(new SkeletalAnimationHelperComponent(_currentAnimationState->animation, _animatedModel));;
		getSceneObject()->addComponent(helper);
	}*/
}


void SkeletalAnimationComponent::calculateRootLoopDelta(AnimationState& animationState)
{
	glm::vec3 positionInStartFrame = getRootBonePositionInStartFrame(animationState.name);
	glm::vec3 positionInEndFrame = getRootBonePositionInEndFrame(animationState.name);
	animationState.rootLoopDeltaPosition = positionInEndFrame - positionInStartFrame;

	LOG_DEBUG("Calculated rootLoopDeltaPosition=" + Strings::toString(animationState.rootLoopDeltaPosition));
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


glm::mat4 SkeletalAnimationComponent::calculateBoneTranslation(RAnimation* animation, Bone* bone, float currentTime, bool lockRootBoneTranslation,
															   const std::unordered_map<std::string, BoneInfo*>::const_iterator& boneInfoIterator)
{
	if (boneInfoIterator != _animatedModel->getBoneInfos().end() &&
		(boneInfoIterator->second->id != _rootNodeIndex || lockRootBoneTranslation))
	{
		return _translationMatrices[boneInfoIterator->second->id];
	}
	else
	{
		return bone->calculatePosition(currentTime + animation->getStartFrame());
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


void SkeletalAnimationComponent::calculateBoneTransformInSingleAnimation(AnimationState* animationState, const std::string& nodeName, bool lockRootBoneTranslation,
																		 const std::unordered_map<std::string, BoneInfo*>::const_iterator& boneInfoIterator, glm::mat4& outTransform)
{
	auto animationBone = animationState->animation->getBones().find(nodeName);

	if (animationBone != animationState->animation->getBones().end())
	{
		Bone* bone = animationBone->second;

		glm::mat4 boneTranslation = calculateBoneTranslation(animationState->animation, bone, animationState->currentTime, lockRootBoneTranslation, boneInfoIterator);
		glm::quat boneRotation = calculateBoneRotation(animationState->animation, bone, animationState->currentTime);
		glm::mat4 boneScale = bone->calculateScale(animationState->currentTime + animationState->animation->getStartFrame());

		outTransform = boneTranslation * glm::toMat4(boneRotation) * boneScale;
	}
}


void SkeletalAnimationComponent::calculateBoneTransformWithAnimationStateBlending(AnimationState* currentAnimationState, AnimationState* nextAnimationState, const std::string& nodeName, bool lockRootBoneTranslation,
																				  const std::unordered_map<std::string, BoneInfo*>::const_iterator& boneInfoIterator, glm::mat4& outTransform)
{
	auto animationBoneInCurrentState = currentAnimationState->animation->getBones().find(nodeName);
	auto animationBoneInNextState = nextAnimationState->animation->getBones().find(nodeName);

	if (animationBoneInCurrentState != currentAnimationState->animation->getBones().end() &&
		animationBoneInNextState != nextAnimationState->animation->getBones().end())
	{
		Bone* bone1 = animationBoneInCurrentState->second;
		Bone* bone2 = animationBoneInNextState->second;

		// zastanowic sie czy tu tez nie interpolowac translacji miedzy stanami
		glm::mat4 boneTranslation = calculateBoneTranslation(currentAnimationState->animation, bone1, currentAnimationState->currentTime, lockRootBoneTranslation, boneInfoIterator);
		glm::quat boneRotation = calculateBoneRotation(currentAnimationState->animation, bone1, currentAnimationState->currentTime);
		glm::quat boneRotation2 = calculateBoneRotation(nextAnimationState->animation, bone2, nextAnimationState->currentTime);
		glm::mat4 boneScale = bone1->calculateScale(currentAnimationState->currentTime + currentAnimationState->animation->getStartFrame());

		glm::quat boneRotationFinal = glm::slerp(boneRotation, boneRotation2, _stateBlendingFactor);

		outTransform = boneTranslation * glm::toMat4(boneRotationFinal) * boneScale;
	}
}


void SkeletalAnimationComponent::calculateRootMotionInSingleAnimation(AnimationState* currentAnimationState, const AnimationNodeData* node, const glm::mat4& parentTransform,
																	  const std::unordered_map<std::string, BoneInfo*>::const_iterator& boneInfoIterator)
{
	glm::mat4 nodeTransform = node->transformation;

	calculateBoneTransformInSingleAnimation(currentAnimationState, node->name, false, boneInfoIterator, nodeTransform);

	glm::mat4 globalTransform = parentTransform * nodeTransform;

	glm::vec4 pos = globalTransform * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	glm::vec3 currentRootPosition(pos.x, pos.y, pos.z);

	currentAnimationState->rootDeltaInLastFrame = currentRootPosition - currentAnimationState->previousRootPosition;

	if (currentAnimationState->currentTime < currentAnimationState->previousTime)
	{
		currentAnimationState->rootDeltaInLastFrame = currentRootPosition - currentAnimationState->previousRootPosition + currentAnimationState->rootLoopDeltaPosition;
	}

	currentAnimationState->previousRootPosition = currentRootPosition;
}


void SkeletalAnimationComponent::calculateRootMotionWithAnimationStateBlending(AnimationState* currentAnimationState, AnimationState* nextAnimationState, const AnimationNodeData* node, const glm::mat4& parentTransform,
																			   const std::unordered_map<std::string, BoneInfo*>::const_iterator& boneInfoIterator)
{
	calculateRootMotionInSingleAnimation(currentAnimationState, node, parentTransform, boneInfoIterator);
	calculateRootMotionInSingleAnimation(nextAnimationState, node, parentTransform, boneInfoIterator);

	currentAnimationState->rootDeltaInLastFrame = nextAnimationState->rootDeltaInLastFrame = glm::mix(currentAnimationState->rootDeltaInLastFrame,
																									  nextAnimationState->rootDeltaInLastFrame,
																									  _stateBlendingFactor);
}


void SkeletalAnimationComponent::calculateBoneTransform(AnimationState* currentAnimationState, AnimationState* nextAnimationState, const AnimationNodeData* node,
														std::vector<glm::mat4>& outFinalBoneMatrices, bool lockRootBoneTranslation, bool rootMotion, const glm::mat4& parentTransform /*= glm::mat4(1.0f)*/)
{
	const std::string& nodeName = node->name;

	auto boneInfo = _animatedModel->getBoneInfos().find(nodeName);

	glm::mat4 nodeTransform = node->transformation;

	if (nextAnimationState == nullptr)
	{
		calculateBoneTransformInSingleAnimation(currentAnimationState, nodeName, lockRootBoneTranslation, boneInfo, nodeTransform);
	}
	else
	{
		calculateBoneTransformWithAnimationStateBlending(currentAnimationState, nextAnimationState, nodeName, lockRootBoneTranslation, boneInfo, nodeTransform);
	}

	glm::mat4 globalTransform = parentTransform * nodeTransform;

	if (boneInfo != _animatedModel->getBoneInfos().end())
	{
		int index = boneInfo->second->id;
		const glm::mat4& offset = boneInfo->second->offset;

		outFinalBoneMatrices[index] = globalTransform * offset;
	}

	if (rootMotion &&
		boneInfo != _animatedModel->getBoneInfos().end() &&
		boneInfo->second->id == _rootNodeIndex)
	{
		if (nextAnimationState == nullptr)
		{
			calculateRootMotionInSingleAnimation(currentAnimationState, node, parentTransform, boneInfo);
		}
		else
		{
			calculateRootMotionWithAnimationStateBlending(currentAnimationState, nextAnimationState, node, parentTransform, boneInfo);
		}
	}


	/* root motion */

	/*if (rootMotion &&
		boneInfo != _animatedModel->getBoneInfos().end() &&
		boneInfo->second->id == _rootNodeIndex)
	{
		glm::vec4 pos = globalTransform * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
		glm::vec3 currentRootPosition(pos.x, pos.y, pos.z);

		currentAnimationState->rootDeltaInLastFrame = currentRootPosition - currentAnimationState->previousRootPosition;

		if (currentAnimationState->currentTime < currentAnimationState->previousTime)
		{
			currentAnimationState->rootDeltaInLastFrame = currentRootPosition - currentAnimationState->previousRootPosition + currentAnimationState->rootLoopDeltaPosition;
		}

		currentAnimationState->previousRootPosition = currentRootPosition;

		_lockRootBoneTranslation = true;
		calculateBoneTransformInSingleAnimation(_currentAnimationState, nodeName, boneInfo, nodeTransform);
		_lockRootBoneTranslation = false;

		// to samo co wy¿ej
		globalTransform = parentTransform * nodeTransform;

		if (boneInfo != _animatedModel->getBoneInfos().end())
		{
			int index = boneInfo->second->id;
			const glm::mat4& offset = boneInfo->second->offset;

			outFinalBoneMatrices[index] = globalTransform * offset;
		}
	}*/

	/* root motion*/

	for (int i = 0; i < node->children.size(); ++i)
	{
		calculateBoneTransform(currentAnimationState, nextAnimationState, &node->children[i], outFinalBoneMatrices, lockRootBoneTranslation, rootMotion, globalTransform);
	}
}


void SkeletalAnimationComponent::update(float deltaTime)
{
	if (_play)
	{
		if (_currentAnimationState)
		{
			_currentAnimationState->previousTime = _currentAnimationState->currentTime;

			_currentAnimationState->currentTime += deltaTime * _currentAnimationState->animation->getTicksPerSecond() * _animationSpeed;
			_currentAnimationState->currentTime = fmod(_currentAnimationState->currentTime, _currentAnimationState->animation->getDuration());
		}

		if (_nextAnimationState != nullptr)
		{
			_nextAnimationState->previousTime = _nextAnimationState->currentTime;

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
	auto insertedAnimationStateIterator = _animationStates.insert(std::make_pair(animationState.name, std::move(animationState))).first;
	AnimationState& insertedAnimationState = insertedAnimationStateIterator->second;

	calculateRootLoopDelta(insertedAnimationState);
	
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

		_nextAnimationState->previousRootPosition = getRootBonePositionInFrame(*_nextAnimationState, _nextAnimationState->currentTime);
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
		calculateBoneTransform(_currentAnimationState, _nextAnimationState, _currentAnimationState->animation->getRootNode(), _finalBoneMatrices, _lockRootBoneTranslation, true, glm::scale(glm::vec3(_scale, _scale, _scale)));

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


glm::vec3 SkeletalAnimationComponent::getRootBonePositionInStartFrame(const std::string& stateName/* = ""*/)
{
	if (_rootBoneName != "")
	{
		const auto& animationStateIterator = _animationStates.find(stateName);
		const AnimationState& animationState = animationStateIterator != _animationStates.end() ? animationStateIterator->second : *_currentAnimationState;

		return getRootBonePositionInFrame(animationState, 0);
	}

	return glm::vec3(0.0f, 0.0f, 0.0f);
}


glm::vec3 SkeletalAnimationComponent::getRootBonePositionInEndFrame(const std::string& stateName/* = ""*/)
{
	if (_rootBoneName != "")
	{
		const auto& animationStateIterator = _animationStates.find(stateName);
		const AnimationState& animationState = animationStateIterator != _animationStates.end() ? animationStateIterator->second : *_currentAnimationState;

		return getRootBonePositionInFrame(animationState, animationState.animation->getEndFrame() - animationState.animation->getStartFrame());
	}

	return glm::vec3(0.0f, 0.0f, 0.0f);
}


glm::vec3 SkeletalAnimationComponent::getRootBonePositionInFrame(const AnimationState& animationState, int frame)
{
	std::vector<glm::mat4> finalBoneMatricesInStartFrame(MAX_BONES, glm::mat4(1.0f));
	AnimationState tempState(animationState.name, animationState.animation, frame);

	calculateBoneTransform(&tempState, nullptr, tempState.animation->getRootNode(), finalBoneMatricesInStartFrame, false, false, glm::scale(glm::vec3(_scale, _scale, _scale)));

	const glm::mat4& transformMatrix = finalBoneMatricesInStartFrame[_rootNodeIndex];

	return glm::vec3(transformMatrix * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
}


glm::vec3 SkeletalAnimationComponent::getRootBonePositionInStartFrameOld(const std::string& stateName/* = ""*/)
{
	const auto& animationState = _animationStates.find(stateName);
	RAnimation* animation = animationState != _animationStates.end() ? animationState->second.animation : _currentAnimationState->animation;

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


glm::vec3 SkeletalAnimationComponent::getRootBonePositionInEndFrameOld(const std::string& stateName/* = ""*/)
{
	const auto& animationState = _animationStates.find(stateName);
	RAnimation* animation = animationState != _animationStates.end() ? animationState->second.animation : _currentAnimationState->animation;

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
