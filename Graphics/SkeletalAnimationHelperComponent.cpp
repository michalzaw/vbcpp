#include "SkeletalAnimationHelperComponent.h"

#include "RAnimation.h"
#include "SkeletalAnimationComponent.h"
#include "SkeletalAnimationComponent2.h"

#include "../Scene/SceneManager.h"

#include "../Utils/RAnimatedModel.h"


SkeletalAnimationHelperComponent::SkeletalAnimationHelperComponent(RAnimation* animation, RAnimatedModel* animatedModel)
	: Component(CT_SKELETAL_ANIMATION_HELPER),
	_animation(animation), _animatedModel(animatedModel),
	_modelRootSceneObject(nullptr), _animationRootSceneObject(nullptr), _finalBonesRootSceneObject(nullptr), _finalSkeletonRootSceneObject(nullptr),
	_finalBonesHelperObjects(100),
	_showModelBones(false), _showAnimationBones(false), _showFinalBones(false), _showFinalSkeletonBones(false),
	_animationBonesTransformFromAnimation(true), _animationBonesTransformDefault(false), _animationBonesTransformCustom(false)
{

}

SkeletalAnimationHelperComponent::~SkeletalAnimationHelperComponent()
{

}


void SkeletalAnimationHelperComponent::onAttachedToScenObject()
{
	_modelRootSceneObject = getSceneObject()->getSceneManager()->addSceneObject("Bones (model)");
	_animationRootSceneObject = getSceneObject()->getSceneManager()->addSceneObject("Bones (animation)");
	_finalBonesRootSceneObject = getSceneObject()->getSceneManager()->addSceneObject("Final bones (unused)");
	_finalSkeletonRootSceneObject = getSceneObject()->getSceneManager()->addSceneObject("Final skeleton (unused)");

	getSceneObject()->addChild(_modelRootSceneObject);
	getSceneObject()->addChild(_animationRootSceneObject);
	getSceneObject()->addChild(_finalBonesRootSceneObject);
	getSceneObject()->addChild(_finalSkeletonRootSceneObject);

	_modelRootSceneObject->setIsActive(_showModelBones);
	_animationRootSceneObject->setIsActive(_showAnimationBones);
	_finalBonesRootSceneObject->setIsActive(_showFinalBones);
	_finalSkeletonRootSceneObject->setIsActive(_showFinalSkeletonBones);

	_modelRootSceneObject->setFlags(SOF_NOT_SERIALIZABLE);
	_animationRootSceneObject->setFlags(SOF_NOT_SERIALIZABLE);
	_finalBonesRootSceneObject->setFlags(SOF_NOT_SERIALIZABLE);
	_finalSkeletonRootSceneObject->setFlags(SOF_NOT_SERIALIZABLE);

	createBoneObjectForModel(_animatedModel->getBonesRootNode(), _modelRootSceneObject);
	createBoneObjectForAnimation(_animation->getRootNode(), _animationRootSceneObject);
	createFinalBoneObject(_animatedModel->getBonesRootNode(), _finalBonesRootSceneObject);

	SkeletalAnimationComponent2* skeletalAnimationComponent = static_cast<SkeletalAnimationComponent2*>(getSceneObject()->getComponent(CT_SKELETAL_ANIMATION_2));
	if (skeletalAnimationComponent != nullptr)
	{
		createFinalSkeletonBoneObject(skeletalAnimationComponent->_finalSkeletonRootNode, _finalSkeletonRootSceneObject);
	}
}


SceneObject* SkeletalAnimationHelperComponent::createBoneObject(const std::string& nodeName, const glm::vec4& color)
{
	SceneObject* sceneObject = getSceneObject()->getSceneManager()->addSceneObject(nodeName);

	Material* material = new Material;
	material->diffuseColor = color;
	material->shader = NOTEXTURE_ALWAYS_VISIBLE_MATERIAL;

	Cube* cube = new Cube(2.0f, material);
	cube->init();

	getSceneObject()->getSceneManager()->getGraphicsManager()->addRenderObject(cube, sceneObject);

	return sceneObject;
}


void SkeletalAnimationHelperComponent::createBoneObjectForModel(AnimationNodeData* nodeData, SceneObject* parent)
{
	SceneObject* sceneObject = createBoneObject(nodeData->name, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));

	parent->addChild(sceneObject);

	sceneObject->setTransformFromMatrix(nodeData->transformation);

	_modelBoneHelperDatas.push_back({ sceneObject, nodeData });

	for (AnimationNodeData& child : nodeData->children)
	{
		createBoneObjectForModel(&child, sceneObject);
	}
}


void SkeletalAnimationHelperComponent::createBoneObjectForAnimation(AnimationNodeData* nodeData, SceneObject* parent)
{
	SceneObject* sceneObject = createBoneObject(nodeData->name, glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));

	parent->addChild(sceneObject);

	auto animationBone = _animation->getBones().find(nodeData->name);
	if (animationBone != _animation->getBones().end())
	{
		Bone* bone = animationBone->second;
		glm::mat4 boneTransform = bone->calculateLocalTransform(0.0f * _animation->getTicksPerSecond());

		sceneObject->setTransformFromMatrix(boneTransform);
	}

	_animationBoneHelperDatas.push_back({ sceneObject, nodeData });

	for (AnimationNodeData& child : nodeData->children)
	{
		createBoneObjectForAnimation(&child, sceneObject);
	}
}


void SkeletalAnimationHelperComponent::createFinalBoneObject(AnimationNodeData* nodeData, SceneObject* parent)
{
	auto boneInfo = _animatedModel->getBoneInfos().find(nodeData->name);
	if (boneInfo != _animatedModel->getBoneInfos().end())
	{
		int index = boneInfo->second->id;

		SceneObject* sceneObject = createBoneObject(nodeData->name, glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
		parent->addChild(sceneObject);

		_finalBonesHelperObjects[index] = sceneObject;
	}

	for (AnimationNodeData& child : nodeData->children)
	{
		createFinalBoneObject(&child, parent);
	}
}


void SkeletalAnimationHelperComponent::createFinalSkeletonBoneObject(FinalSkeletonNode* nodeData, SceneObject* parent)
{
	std::string nodeName = "";
	if (nodeData->modelNode != nullptr)
	{
		nodeName += nodeData->modelNode->name + " (M) ";
	}
	if (nodeData->animationNode != nullptr)
	{
		nodeName += nodeData->animationNode->name + " (A) ";
	}

	SceneObject* sceneObject = createBoneObject(nodeName, glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));

	parent->addChild(sceneObject);

	/*if (nodeData->modelNode != nullptr)
	{
		sceneObject->setPosition(getTranslationFromMatrix(nodeData->modelNode->transformation));
		sceneObject->setRotationQuaternion(getRotationFromMatrix(nodeData->modelNode->transformation));
		sceneObject->setScale(getScaleFromMatrix(nodeData->modelNode->transformation));
	}
	if (nodeData->animationNode != nullptr)
	{
		sceneObject->setPosition(getTranslationFromMatrix(nodeData->animationNode->transformation));
		sceneObject->setRotationQuaternion(getRotationFromMatrix(nodeData->animationNode->transformation));
		sceneObject->setScale(getScaleFromMatrix(nodeData->animationNode->transformation));
	}*/

	sceneObject->setPosition(nodeData->position);
	sceneObject->setRotationQuaternion(nodeData->orientation);

	_finalSkeletonHelperObjects.push_back({ sceneObject, nodeData });

	for (FinalSkeletonNode* child : nodeData->children)
	{
		createFinalSkeletonBoneObject(child, sceneObject);
	}

}


void SkeletalAnimationHelperComponent::update(float deltaTime)
{
	SkeletalAnimationComponent* skeletalAnimationComponent = static_cast<SkeletalAnimationComponent*>(getSceneObject()->getComponent(CT_SKELETAL_ANIMATION));
	SkeletalAnimationComponent2* skeletalAnimationComponent2 = static_cast<SkeletalAnimationComponent2*>(getSceneObject()->getComponent(CT_SKELETAL_ANIMATION_2));

	for (const auto& data : _modelBoneHelperDatas)
	{
		data.animationNode->transformation = data.helperObject->getLocalTransformMatrix();
	}

	if (_animationBonesTransformFromAnimation)
	{
		for (const auto& data : _animationBoneHelperDatas)
		{
			auto animationBone = _animation->getBones().find(data.animationNode->name);
			if (animationBone != _animation->getBones().end())
			{
				Bone* bone = animationBone->second;

				float currentTime;
				if (skeletalAnimationComponent != nullptr)
				{
					currentTime = skeletalAnimationComponent->getCurrentTime();
				}
				else if (skeletalAnimationComponent2 != nullptr)
				{
					currentTime = skeletalAnimationComponent2->getCurrentTime();
				}

				glm::mat4 boneTransform = bone->calculateLocalTransform(currentTime);

				data.helperObject->setTransformFromMatrix(boneTransform);
			}
		}
	}
	else if (_animationBonesTransformDefault)
	{
		for (const auto& data : _animationBoneHelperDatas)
		{
			data.helperObject->setTransformFromMatrix(data.animationNode->transformation);
		}
	}

	if (skeletalAnimationComponent2 != nullptr)
	{
		for (int i = 0; i < 100; ++i)
		{
			if (_finalBonesHelperObjects[i] != nullptr)
			{
				_finalBonesHelperObjects[i]->setTransformFromMatrix(skeletalAnimationComponent2->_finalBoneMatrices[i]);
			}
		}
	}
}

void SkeletalAnimationHelperComponent::setShowModelBones(bool showModelBones)
{
	_showModelBones = showModelBones;
	
	if (_modelRootSceneObject != nullptr)
	{
		_modelRootSceneObject->setIsActive(showModelBones);
	}
}


void SkeletalAnimationHelperComponent::setShowAnimationBones(bool showAnimationBones)
{
	_showAnimationBones = showAnimationBones;

	if (_animationRootSceneObject != nullptr)
	{
		_animationRootSceneObject->setIsActive(showAnimationBones);
	}
}


void SkeletalAnimationHelperComponent::setShowFinalBones(bool showFinalBones)
{
	_showFinalBones = showFinalBones;

	if (_finalBonesRootSceneObject != nullptr)
	{
		_finalBonesRootSceneObject->setIsActive(showFinalBones);
	}
}


void SkeletalAnimationHelperComponent::setShowFinalSkeletonBones(bool showFinalSkeletonBones)
{
	_showFinalSkeletonBones = showFinalSkeletonBones;

	if (_finalSkeletonRootSceneObject != nullptr)
	{
		_finalSkeletonRootSceneObject->setIsActive(showFinalSkeletonBones);
	}
}


void SkeletalAnimationHelperComponent::setAnimationBonesTransformFromAnimation(bool animationBonesTransformFromAnimation)
{
	_animationBonesTransformFromAnimation = animationBonesTransformFromAnimation;

	if (_animationBonesTransformFromAnimation)
	{
		_animationBonesTransformDefault = false;
		_animationBonesTransformCustom = false;
	}
}


void SkeletalAnimationHelperComponent::setAnimationBonesTransformDefault(bool animationBonesTransformDefault)
{
	_animationBonesTransformDefault = animationBonesTransformDefault;

	if (_animationBonesTransformDefault)
	{
		_animationBonesTransformFromAnimation = false;
		_animationBonesTransformCustom = false;
	}
}


void SkeletalAnimationHelperComponent::setAnimationBonesTransformCustom(bool animationBonesTransformCustom)
{
	_animationBonesTransformCustom = animationBonesTransformCustom;

	if (_animationBonesTransformCustom)
	{
		_animationBonesTransformFromAnimation = false;
		_animationBonesTransformDefault = false;
	}
}
