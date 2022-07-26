#include "SkeletalAnimationComponent2.h"

#include "RAnimation.h"
#include "RenderObject.h"
#include "SkeletalAnimationHelperComponent.h"

#include "../Scene/SceneManager.h"

#include "../Utils/RAnimatedModel.h"


SkeletalAnimationComponent2::SkeletalAnimationComponent2(RAnimation* animation, const std::unordered_map<std::string, std::string>& boneMap)
	: Component(CT_SKELETAL_ANIMATION_2),
	_animation(animation),
	_finalBoneMatrices(MAX_BONES, glm::mat4(1.0f)),
	_currentTime(0.0f),
	_animationDuration(animation->getDuration()), _animationTicksPerSecond(animation->getTicksPerSecond()), _play(true),
	_animatedModel(nullptr),
	_defaultTranslationBoneMatrices(MAX_BONES, glm::mat4(1.0f)), _defaultRotationBoneMatrices(MAX_BONES, glm::mat4(1.0f)), _cachedModelNodes(MAX_BONES, nullptr),
	_boneMap(boneMap)
{

}


SkeletalAnimationComponent2::~SkeletalAnimationComponent2()
{

}


void SkeletalAnimationComponent2::onAttachedToScenObject()
{
	RenderObject* renderObject = static_cast<RenderObject*>(getSceneObject()->getComponent(CT_RENDER_OBJECT));
	_animatedModel = dynamic_cast<RAnimatedModel*>(renderObject->getModel());

	if (_animatedModel == nullptr)
	{
		LOG_ERROR("SceneObject has not an animated model");
		return;
	}

	// todo: animation usuwanie componentu
	SkeletalAnimationHelperComponent* helper = getSceneObject()->getSceneManager()->getGraphicsManager()->addSkeletalAnimationHelper(new SkeletalAnimationHelperComponent(_animation, _animatedModel));;
	getSceneObject()->addComponent(helper);


	calculateDefaultRotationInModel(&_animatedModel->_bonesRootNode);
}



void SkeletalAnimationComponent2::calculateDefaultRotationInModel(AnimationNodeData* node, const glm::mat4& parentTransform)
{
	const std::string& nodeName = node->name;

	glm::mat4 nodeTransform = node->transformation;
	glm::vec3 nodeTranslation = getTranslationFromMatrix(nodeTransform);
	glm::quat nodeRotation = getRotationFromMatrix(nodeTransform);

	glm::mat4 nodeTranslationMatrix = glm::translate(nodeTranslation);
	glm::mat4 nodeRotationMatrix = glm::mat4_cast(nodeRotation);

	glm::mat4 globalTransform = parentTransform * nodeTransform;

	auto boneInfo = _animatedModel->getBoneInfos().find(nodeName);
	if (boneInfo != _animatedModel->getBoneInfos().end())
	{
		int index = boneInfo->second.id;

		_defaultTranslationBoneMatrices[index] = nodeTranslationMatrix;
		_defaultRotationBoneMatrices[index] = nodeRotationMatrix;
		//_defaultRotationBoneMatrices[index] = globalTransform;

		_cachedModelNodes[index] = node;
	}


	for (int i = 0; i < node->children.size(); ++i)
	{
		calculateDefaultRotationInModel(&node->children[i], globalTransform);
	}
}


void SkeletalAnimationComponent2::calculateBoneTransformInModel(AnimationNodeData* node, const glm::mat4& parentTransform)
{
	const std::string& nodeName = node->name;
	/*if (_boneMap.find(nodeName) != _boneMap.end())
	{
		return;
	}*/

	glm::mat4 nodeTransform = node->transformation;

	glm::mat4 globalTransform = parentTransform * nodeTransform;


	auto boneInfo = _animatedModel->getBoneInfos().find(nodeName);
	if (boneInfo != _animatedModel->getBoneInfos().end())
	{
		int index = boneInfo->second.id;
		const glm::mat4& offset = boneInfo->second.offset;

		_finalBoneMatrices[index] = globalTransform * offset;
	}

	for (int i = 0; i < node->children.size(); ++i)
	{
		calculateBoneTransformInModel(&node->children[i], globalTransform);
	}
}


// iteracja po koœciach animacji
//void SkeletalAnimationComponent2::calculateBoneTransformInAnimation(AnimationNodeData* node, const glm::mat4& parentTransform)
//{
//	const std::string& nodeName = node->name;
//
//	glm::mat4 nodeTransform = node->transformation;
//
//	glm::mat4 globalTransform = parentTransform;
//
//	Bone* bone = _animation->findBone(nodeName);
//	if (bone != nullptr)
//	{
//		bone->update(_currentTime);
//		//nodeTransform = bone->getLocalTransform(); // todo: animation lepiej ¿eby update zwraca³ t¹ transformacjê, zamiast zapisywaæ j¹ wewn¹trz bone
//		nodeTransform = bone->_rotation;
//		globalTransform = parentTransform * nodeTransform;
//
//		const std::string& boneNameInAnimation = bone->getName();
//
//		for (const auto& pair : _boneMap)
//		{
//			if (pair.second == nodeName)
//			{
//				const std::string& boneNameInModel = pair.first;
//				auto boneInfo = _animatedModel->getBoneInfos().find(boneNameInModel);
//
//				if (boneInfo != _animatedModel->getBoneInfos().end())
//				{
//					int index = boneInfo->second.id;
//
//					globalTransform = parentTransform * _defaultTranslationBoneMatrices[index] * nodeTransform;
//
//					_finalBoneMatrices[index] = globalTransform * _defaultRotationBoneMatrices[index] * boneInfo->second.offset;
//
//
//
//
//
//					//globalTransform = parentTransform * _defaultTranslationBoneMatrices[index];
//
//					//_finalBoneMatrices[index] = globalTransform * _defaultRotationBoneMatrices[index] * boneInfo->second.offset;
//
//					for (int i = 0; i < _cachedModelNodes[index]->children.size(); ++i)
//					{
//						calculateBoneTransformInModel(&_cachedModelNodes[index]->children[i], globalTransform);
//					}
//				}
//			}
//		}
//	}
//
//	//glm::mat4 globalTransform = parentTransform * nodeTransform;
//
//	for (int i = 0; i < node->children.size(); ++i)
//	{
//		calculateBoneTransformInAnimation(&node->children[i], globalTransform);
//	}
//}


// iteracja po koœciach modelu
void SkeletalAnimationComponent2::calculateBoneTransformInAnimation(AnimationNodeData* node, const glm::mat4& parentTransform)
{
	const std::string& nodeName = node->name;

	glm::mat4 nodeTransform = node->transformation;

	glm::mat4 globalTransform = parentTransform;


	auto boneInfo = _animatedModel->getBoneInfos().find(nodeName);
	if (boneInfo != _animatedModel->getBoneInfos().end())
	{
		int index = boneInfo->second.id;

		globalTransform = parentTransform * _defaultTranslationBoneMatrices[index] * _defaultRotationBoneMatrices[index];

		auto animationBoneName = _boneMap.find(nodeName);
		if (animationBoneName != _boneMap.end())
		{
			auto animationBone = _animation->findBone(animationBoneName->second);
			if (animationBone != nullptr)
			{
				animationBone->update(_currentTime);

				//globalTransform = globalTransform * animationBone->_rotation;
				globalTransform = parentTransform * _defaultTranslationBoneMatrices[index] * _defaultRotationBoneMatrices[index] * animationBone->_rotation;
			}
		}

		_finalBoneMatrices[index] = globalTransform * boneInfo->second.offset;
	}

	for (int i = 0; i < node->children.size(); ++i)
	{
		calculateBoneTransformInAnimation(&node->children[i], globalTransform);
	}
}


void SkeletalAnimationComponent2::update(float deltaTime)
{
	if (_play)
	{
		_currentTime += deltaTime * _animationTicksPerSecond;
		_currentTime = fmod(_currentTime, _animationDuration);
	}

	calculateBoneTransformInModel(&_animatedModel->_bonesRootNode);
	//calculateBoneTransformInAnimation(_animation->getRootNode());
	//calculateBoneTransformInAnimation(&_animatedModel->_bonesRootNode);
}