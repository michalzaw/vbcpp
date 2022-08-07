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


FinalSkeletonNode* createFinalSkeletonProcessNodeFromModel(AnimationNodeData* node, FinalSkeletonNode* parent = nullptr)
{
	FinalSkeletonNode* finalSkeletonNode = new FinalSkeletonNode;
	finalSkeletonNode->modelNode = node;
	finalSkeletonNode->parent = parent;

	for (AnimationNodeData& child : node->children)
	{
		finalSkeletonNode->children.push_back(createFinalSkeletonProcessNodeFromModel(&child, finalSkeletonNode));
	}

	return finalSkeletonNode;
}


FinalSkeletonNode* createFinalSkeletonProcessNodeFromAnimation(AnimationNodeData* node, FinalSkeletonNode* parent = nullptr)
{
	FinalSkeletonNode* finalSkeletonNode = new FinalSkeletonNode;
	finalSkeletonNode->animationNode = node;
	finalSkeletonNode->parent = parent;

	for (AnimationNodeData& child : node->children)
	{
		finalSkeletonNode->children.push_back(createFinalSkeletonProcessNodeFromAnimation(&child, finalSkeletonNode));
	}

	return finalSkeletonNode;
}


// mode: 0 - model nodes, 1 - animation nodes, 2 - model and animation nodes
FinalSkeletonNode* findNodeByName(FinalSkeletonNode* skeletonNode, const std::string& nodeName, int mode)
{
	if (mode == 0 && skeletonNode->modelNode != nullptr && skeletonNode->modelNode->name == nodeName)
	{
		return skeletonNode;
	}
	else if (mode == 1 && skeletonNode->animationNode != nullptr && skeletonNode->animationNode->name == nodeName)
	{
		return skeletonNode;
	}
	else if (skeletonNode->modelNode != nullptr && skeletonNode->modelNode->name == nodeName ||
			 skeletonNode->animationNode != nullptr && skeletonNode->animationNode->name == nodeName)
	{
		return skeletonNode;
	}

	for (FinalSkeletonNode* child : skeletonNode->children)
	{
		FinalSkeletonNode* result = findNodeByName(child, nodeName, mode);
		if (result != nullptr)
		{
			return result;
		}
	}

	return nullptr;
}


void inserModelNodeToSkeleton(FinalSkeletonNode* animationSkeletonRootNode, FinalSkeletonNode* modelSkeletonNode, const std::unordered_map<std::string, std::string>& boneMap)
{
	auto animationBoneName = boneMap.find(modelSkeletonNode->modelNode->name);
	if (animationBoneName != boneMap.end())
	{
		FinalSkeletonNode* result = findNodeByName(animationSkeletonRootNode, animationBoneName->second, 1);
		if (result != nullptr)
		{
			result->modelNode = modelSkeletonNode->modelNode;
		}
	}
	else
	{
		if (modelSkeletonNode->parent != nullptr)
		{
			FinalSkeletonNode* result = findNodeByName(animationSkeletonRootNode, modelSkeletonNode->parent->modelNode->name, 0);
			if (result != nullptr)
			{
				FinalSkeletonNode* newNode = new FinalSkeletonNode;
				newNode->modelNode = modelSkeletonNode->modelNode;

				result->children.push_back(newNode);
			}
		}
	}

	for (FinalSkeletonNode* child : modelSkeletonNode->children)
	{
		inserModelNodeToSkeleton(animationSkeletonRootNode, child, boneMap);
	}
}


void setTransformInSkeleton(FinalSkeletonNode* skeletonNode)
{
	if (skeletonNode->animationNode != nullptr)
	{
		skeletonNode->position = getTranslationFromMatrix(skeletonNode->animationNode->transformation);
		skeletonNode->orientation = getRotationFromMatrix(skeletonNode->animationNode->transformation);
	}
	if (skeletonNode->modelNode != nullptr)
	{
		skeletonNode->position = getTranslationFromMatrix(skeletonNode->modelNode->transformation);
	}

	for (FinalSkeletonNode* child : skeletonNode->children)
	{
		setTransformInSkeleton(child);
	}
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

	FinalSkeletonNode* modelSkeleton = createFinalSkeletonProcessNodeFromModel(_animatedModel->getBonesRootNode());
	FinalSkeletonNode* animationSkeleton = createFinalSkeletonProcessNodeFromAnimation(_animation->getRootNode());

	inserModelNodeToSkeleton(animationSkeleton, modelSkeleton, _boneMap);

	_finalSkeletonRootNode = animationSkeleton;

	setTransformInSkeleton(_finalSkeletonRootNode);

	//createFinalSkeletonProcessNodeFromAnimation(_animation->getRootNode(), _finalSkeletonRootNode);

	SkeletalAnimationHelperComponent* helper = getSceneObject()->getSceneManager()->getGraphicsManager()->addSkeletalAnimationHelper(new SkeletalAnimationHelperComponent(_animation, _animatedModel));;
	getSceneObject()->addComponent(helper);


	calculateDefaultRotationInModel(_animatedModel->getBonesRootNode());
}


/*FinalSkeletonNode* foo(AnimationNodeData* node, const std::string& animationNodeName)
{
	if (node->name == animationNodeName)
	{
		FinalSkeletonNode* result = new FinalSkeletonNode;
		result->animationNode = node;

		return result;
	}

	for (AnimationNodeData& child : node->children)
	{
		FinalSkeletonNode* result = foo(node, animationNodeName);
		if (result != nullptr)
		{
			FinalSkeletonNode* resultNode = new FinalSkeletonNode;
			resultNode->children.push_back(result);
			resultNode->animationNode = &child;

			result->parent = resultNode;

			return result;
		}
	}

	return nullptr;
}


void SkeletalAnimationComponent2::createFinalSkeletonProcessNodeFromAnimation(AnimationNodeData* node, FinalSkeletonNode* currentNodeInFinalSkeleton)
{
	if (currentNodeInFinalSkeleton->modelNode != nullptr)
	{
		auto animationBoneName = _boneMap.find(currentNodeInFinalSkeleton->modelNode->name);
		if (animationBoneName != _boneMap.end())
		{
			if (node->name == animationBoneName->second)
			{
				// aktualny node animacji odpowiada node z modelu
				currentNodeInFinalSkeleton->animationNode = node;
			}
			else
			{
				// aktualny node animacji nie odpowiada node z modelu - szukamy wsrod jego children
				FinalSkeletonNode* result = foo(node, animationBoneName->second);

				if (result != nullptr)
				{
					auto currentParent = currentNodeInFinalSkeleton->parent;
					currentNodeInFinalSkeleton->parent = result;
				}
			}

			for (FinalSkeletonNode* child : currentNodeInFinalSkeleton->children)
			{
				createFinalSkeletonProcessNodeFromAnimation(node, child);
			}
		}
		else
		{
			// danemu node z modelu nie odpowiada ¿aden node z animacji - zostawiamy go jako leaf i wracamy do gory drzewa
		}
	}
}*/


/*FinalSkeletonNode* findByModelNodeName(FinalSkeletonNode* node, const std::string& expectedNodeName)
{
	if (node->modelNode != nullptr && node->modelNode->name == expectedNodeName)
	{
		return node;
	}

	for (FinalSkeletonNode* child : node->children)
	{
		FinalSkeletonNode* result = findByModelNodeName(child, expectedNodeName);
		if (result != nullptr)
		{
			return result;
		}
	}

	return nullptr;
}


void SkeletalAnimationComponent2::createFinalSkeletonProcessNodeFromAnimation(AnimationNodeData* node, FinalSkeletonNode* parent)
{
	std::string nodeNameInModel = "";
	for (const auto& pair : _boneMap)
	{
		if (pair.second == node->name)
		{
			nodeNameInModel = pair.first;
			break;
		}
	}

	FinalSkeletonNode* result = nullptr;
	if (nodeNameInModel != "")
	{
		result = findByModelNodeName(parent, nodeNameInModel);
	}

	if (result != nullptr)
	{
		result->animationNode = node;
	}
	else
	{
		result = new FinalSkeletonNode;
		result->animationNode = node;
		result->parent = parent;

		parent->children.push_back(result);
	}

	for (AnimationNodeData& child : node->children)
	{
		createFinalSkeletonProcessNodeFromAnimation(&child, result);
	}
}*/


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
		int index = boneInfo->second->id;

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
		int index = boneInfo->second->id;
		const glm::mat4& offset = boneInfo->second->offset;

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
//	auto animationBone = _animation->getBones().find(nodeName);
//	if (animationBone != _animation->getBones().end())
//	{
//		Bone* bone = animationBone->second;
//		//nodeTransform = bone->calculateLocalTransform(_currentTime);
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
//					int index = boneInfo->second->id;
//
//					globalTransform = parentTransform * _defaultTranslationBoneMatrices[index] * nodeTransform;
//
//					_finalBoneMatrices[index] = globalTransform * _defaultRotationBoneMatrices[index] * boneInfo->second->offset;
//
//
//
//
//
//					//globalTransform = parentTransform * _defaultTranslationBoneMatrices[index];
//
//					//_finalBoneMatrices[index] = globalTransform * _defaultRotationBoneMatrices[index] * boneInfo->second->offset;
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
//void SkeletalAnimationComponent2::calculateBoneTransformInAnimation(AnimationNodeData* node, const glm::mat4& parentTransform)
//{
//	const std::string& nodeName = node->name;
//
//	glm::mat4 nodeTransform = node->transformation;
//
//	glm::mat4 globalTransform = parentTransform;
//
//
//	auto boneInfo = _animatedModel->getBoneInfos().find(nodeName);
//	if (boneInfo != _animatedModel->getBoneInfos().end())
//	{
//		int index = boneInfo->second->id;
//
//		globalTransform = parentTransform * _defaultTranslationBoneMatrices[index] * _defaultRotationBoneMatrices[index];
//
//		auto animationBoneName = _boneMap.find(nodeName);
//		if (animationBoneName != _boneMap.end())
//		{
//			auto animationBone = _animation->findBone(animationBoneName->second);
//			if (animationBone != nullptr)
//			{
//				animationBone->update(_currentTime);
//
//				//globalTransform = globalTransform * animationBone->_rotation;
//				globalTransform = parentTransform * _defaultTranslationBoneMatrices[index] * _defaultRotationBoneMatrices[index] * animationBone->_rotation;
//			}
//		}
//
//		//_finalBoneMatrices[index] = globalTransform * boneInfo->second->offset;
//	}
//
//	for (int i = 0; i < node->children.size(); ++i)
//	{
//		calculateBoneTransformInAnimation(&node->children[i], globalTransform);
//	}
//}


// wersja bez retargetingu
void SkeletalAnimationComponent2::calculateBoneTransformInAnimation(AnimationNodeData* node, const glm::mat4& parentTransform)
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

	auto boneInfo = _animatedModel->getBoneInfos().find(/*"krystian_" + */nodeName);
	if (boneInfo != _animatedModel->getBoneInfos().end())
	{
		int index = boneInfo->second->id;

		_finalBoneMatrices[index] = globalTransform * boneInfo->second->offset;
		//_finalBoneMatrices[index] = globalTransform * _defaultRotationBoneMatrices[index] * boneInfo->second.offset;
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

	/*
	calculateBoneTransformInModel(_animatedModel->getBonesRootNode());
	//calculateBoneTransformInAnimation(_animation->getRootNode());
	calculateBoneTransformInAnimation(_animatedModel->getBonesRootNode());
	*/

	calculateBoneTransformInAnimation(_animation->getRootNode());
}