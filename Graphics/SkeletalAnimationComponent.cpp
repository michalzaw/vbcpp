#include "SkeletalAnimationComponent.h"

#include <string>

#include "RAnimation.h"
#include "RenderObject.h"

#include "../Scene/SceneObject.h"

#include "../Utils/Math.h"
#include "../Utils/RAnimatedModel.h"



SkeletalAnimationComponent::SkeletalAnimationComponent(RAnimation* animation, const std::unordered_map<std::string, std::string>& animationNodeNameToBoneNameInModelMap)
	: Component(CT_SKELETAL_ANIMATION),
	_animation(animation),
	_finalBoneMatrices(MAX_BONES, glm::mat4(1.0f)),
	_currentTime(0.0f), _deltaTime(0.0f),
	_animatedModel(nullptr),
	_boneTransformMatricesInModel(MAX_BONES, glm::mat4(1.0f)), _boneTransformMatricesInAnimation(MAX_BONES, glm::mat4(1.0f)), _boneTransformOffsetsMatrices(MAX_BONES, glm::mat4(1.0f)),
	_animationNodeNameToBoneNameInModelMap(animationNodeNameToBoneNameInModelMap)
{

}


SkeletalAnimationComponent::~SkeletalAnimationComponent()
{

}


glm::vec3 getTranslation(const glm::mat4& transformMatrix)
{
	glm::vec3 scale;
	glm::quat orientation;
	glm::vec3 translation;
	glm::vec3 skew;
	glm::vec4 perspective;
	glm::decompose(transformMatrix, scale, orientation, translation, skew, perspective);

	return translation;
}


glm::quat getRotation(const glm::mat4& transformMatrix)
{
	glm::vec3 scale;
	glm::quat orientation;
	glm::vec3 translation;
	glm::vec3 skew;
	glm::vec4 perspective;
	glm::decompose(transformMatrix, scale, orientation, translation, skew, perspective);

	return orientation;
}


void SkeletalAnimationComponent::onAttachedToScenObject()
{
	RenderObject* renderObject = static_cast<RenderObject*>(getSceneObject()->getComponent(CT_RENDER_OBJECT));
	_animatedModel = dynamic_cast<RAnimatedModel*>(renderObject->getModel());

	if (_animatedModel == nullptr)
	{
		LOG_ERROR("SceneObject has not an animated model");
		return;
	}

	_currentTime = 0.0f;
	//calculateBoneTransform(_animation->getRootNode());

	/*for (const auto& boneInfo : _animatedModel->getBoneInfos())
	{

	}*/

	calculateDefaultBoneTransformInAnimation(_animation->getRootNode());
	calculateDefaultBoneTransformInModel(&_animatedModel->_bonesRootNode);

	for (int i = 0; i < MAX_BONES; ++i)
	{
		glm::vec3 offset = getTranslation(_boneTransformMatricesInModel[i]) - getTranslation(_boneTransformMatricesInAnimation[i]);
		glm::mat4 rot1 = glm::mat4_cast(getRotation(_boneTransformMatricesInModel[i]));
		glm::mat4 rot2 = glm::mat4_cast(getRotation(_boneTransformMatricesInAnimation[i]));

		_boneTransformOffsetsMatrices[i] = glm::translate(offset) * rot2 * glm::inverse(rot1);
		//_boneTransformOffsetsMatrices[i] = glm::inverse(_boneTransformMatricesInAnimation[i]) * _boneTransformMatricesInModel[i];
	}
}


std::string SkeletalAnimationComponent::mapAnimationNodeNameToBoneNameInModel(const std::string& animationNodeName)
{
	return animationNodeName;

	/*auto mappedName = _animationNodeNameToBoneNameInModelMap.find(animationNodeName);
	if (mappedName == _animationNodeNameToBoneNameInModelMap.end())
	{
		return animationNodeName;
	}

	return mappedName->second;*/
}


void SkeletalAnimationComponent::calculateDefaultBoneTransformInModel(const AnimationNodeData* node, const glm::mat4& parentTransform)
{
	const std::string& nodeName = node->name;
	glm::mat4 nodeTransform = node->transformation;

	glm::mat4 globalTransform = parentTransform * nodeTransform;


	// ------------------------------------------------------------
	auto boneInfo = _animatedModel->getBoneInfos().find(nodeName);
	if (boneInfo != _animatedModel->getBoneInfos().end())
	{
		int index = boneInfo->second.id;
		const glm::mat4& offset = boneInfo->second.offset;

		//_finalBoneMatrices[index] = globalTransform * offset;
		_boneTransformMatricesInModel[index] = globalTransform;// *offset;
	}
	// ------------------------------------------------------------


	for (int i = 0; i < node->children.size(); ++i)
	{
		calculateDefaultBoneTransformInModel(&node->children[i], globalTransform);
	}
}


void SkeletalAnimationComponent::calculateDefaultBoneTransformInAnimation(const AnimationNodeData* node, const glm::mat4& parentTransform)
{
	const std::string& nodeName = node->name;
	glm::mat4 nodeTransform = node->transformation;

	glm::mat4 globalTransform = parentTransform * nodeTransform;


	// ------------------------------------------------------------
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

				//_finalBoneMatrices[index] = globalTransform * offset;
				_boneTransformMatricesInAnimation[index] = globalTransform;// *offset;
			}
		}
	}
	// ------------------------------------------------------------


	for (int i = 0; i < node->children.size(); ++i)
	{
		calculateDefaultBoneTransformInAnimation(&node->children[i], globalTransform);
	}
}


void SkeletalAnimationComponent::calculateBoneTransform(const StaticModelNode* node, const glm::mat4& parentTransform)
{
	const std::string& nodeName = node->name;
	glm::mat4 nodeTransform = node->transform.getTransformMatrix();

	glm::mat4 globalTransform = parentTransform * nodeTransform;


	auto boneInfo = _animatedModel->getBoneInfos().find(nodeName);

	if (boneInfo != _animatedModel->getBoneInfos().end())
	{
		int index = boneInfo->second.id;
		const glm::mat4& offset = boneInfo->second.offset;

		_finalBoneMatrices[index] = globalTransform * offset;
	}


	/*for (const auto& pair : _animationNodeNameToBoneNameInModelMap)
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
	}*/

	for (int i = 0; i < node->children.size(); ++i)
	{
		calculateBoneTransform(node->children[i], globalTransform);
	}
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

				_finalBoneMatrices[index] = globalTransform * /*_boneTransformOffsetsMatrices[index] */ offset;
			}
		}
	}

	auto boneInfo = _animatedModel->getBoneInfos().find(nodeName);
	if (boneInfo != _animatedModel->getBoneInfos().end())
	{
		int index = boneInfo->second.id;
		const glm::mat4& offset = boneInfo->second.offset;

		_finalBoneMatrices[index] = globalTransform * offset;
	}


	for (int i = 0; i < node->children.size(); ++i)
	{
		calculateBoneTransform(&node->children[i], globalTransform);
	}
}


void SkeletalAnimationComponent::update(float deltaTime)
{
	_deltaTime = deltaTime;

	_currentTime += _deltaTime * _animation->getTicksPerSecond();
	//_currentTime = 2.5f;
	_currentTime = fmod(_currentTime, _animation->getDuration());

	calculateBoneTransform(_animation->getRootNode());
	//calculateBoneTransform(_animatedModel->getRootNode());
	//calculateBoneTransform(&_animatedModel->_bonesRootNode);
}
