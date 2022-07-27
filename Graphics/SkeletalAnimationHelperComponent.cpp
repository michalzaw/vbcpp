#include "SkeletalAnimationHelperComponent.h"

#include "RAnimation.h"
#include "SkeletalAnimationComponent2.h"

#include "../Scene/SceneManager.h"

#include "../Utils/RAnimatedModel.h"


SkeletalAnimationHelperComponent::SkeletalAnimationHelperComponent(RAnimation* animation, RAnimatedModel* animatedModel)
	: Component(CT_SKELETAL_ANIMATION_HELPER),
	_animation(animation), _animatedModel(animatedModel),
	_modelRootSceneObject(nullptr), _animationRootSceneObject(nullptr), _finalBonesRootSceneObject(nullptr), _finalSkeletonRootSceneObject(nullptr),
	_finalBonesHelperObjects(100),
	_showModelBones(true), _showAnimationBones(true), _showFinalBones(true), _showFinalSkeletonBones(true),
	_animationBonesTransformFromAnimation(true), _animationBonesTransformDefault(false), _animationBonesTransformCustom(false)
{

}

SkeletalAnimationHelperComponent::~SkeletalAnimationHelperComponent()
{

}


// todo: animation przeniesc do osobnego pliku
glm::vec3 getTranslationFromMatrix(const glm::mat4& transformMatrix)
{
	glm::vec3 scale;
	glm::quat orientation;
	glm::vec3 translation;
	glm::vec3 skew;
	glm::vec4 perspective;
	glm::decompose(transformMatrix, scale, orientation, translation, skew, perspective);

	return translation;
}


glm::quat getRotationFromMatrix(const glm::mat4& transformMatrix)
{
	glm::vec3 scale;
	glm::quat orientation;
	glm::vec3 translation;
	glm::vec3 skew;
	glm::vec4 perspective;
	glm::decompose(transformMatrix, scale, orientation, translation, skew, perspective);

	// https://stackoverflow.com/questions/17918033/glm-decompose-mat4-into-translation-and-rotation
	// funkcja glm::decompose zwraca niepoprawny kwaternion
	// aby otrzymaæ prawid³ow¹ rotacjê nale¿y wykonaæ glm::conjugate
	// conjugate tworzy tak naprawdê rotacjê odwrotn¹
	// dlatego te¿ w ObjectPropertiesWindow dzia³a³ poni¿szy kod:
	// glm::vec3 rotationEulerAngles = glm::eulerAngles(getRotation(nodeData->transformation));
	// rotationEulerAngles = glm::vec3(-rotationEulerAngles.x, -rotationEulerAngles.y, -rotationEulerAngles.z);
	// jeœli jednak chcemy operowaæ na kwaternionie, to lepiej wykonaæ glm::conjugate(orientation);
	return glm::conjugate(orientation);
}


glm::vec3 getScaleFromMatrix(const glm::mat4& transformMatrix)
{
	glm::vec3 scale;
	glm::quat orientation;
	glm::vec3 translation;
	glm::vec3 skew;
	glm::vec4 perspective;
	glm::decompose(transformMatrix, scale, orientation, translation, skew, perspective);

	return scale;
}


void SkeletalAnimationHelperComponent::onAttachedToScenObject()
{
	_modelRootSceneObject = getSceneObject()->getSceneManager()->addSceneObject("Model");
	_animationRootSceneObject = getSceneObject()->getSceneManager()->addSceneObject("Animation");
	_finalBonesRootSceneObject = getSceneObject()->getSceneManager()->addSceneObject("Final bones");
	_finalSkeletonRootSceneObject = getSceneObject()->getSceneManager()->addSceneObject("Final skeleton");

	getSceneObject()->addChild(_modelRootSceneObject);
	getSceneObject()->addChild(_animationRootSceneObject);
	getSceneObject()->addChild(_finalBonesRootSceneObject);
	getSceneObject()->addChild(_finalSkeletonRootSceneObject);

	createBoneObjectForModel(&_animatedModel->_bonesRootNode, _modelRootSceneObject);
	createBoneObjectForAnimation(_animation->getRootNode(), _animationRootSceneObject);
	createFinalBoneObject(&_animatedModel->_bonesRootNode, _finalBonesRootSceneObject);
}


SceneObject* SkeletalAnimationHelperComponent::createBoneObject(AnimationNodeData* nodeData, const glm::vec4& color)
{
	SceneObject* sceneObject = getSceneObject()->getSceneManager()->addSceneObject(nodeData->name);

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
	SceneObject* sceneObject = createBoneObject(nodeData, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));

	parent->addChild(sceneObject);

	sceneObject->setPosition(getTranslationFromMatrix(nodeData->transformation));
	sceneObject->setRotationQuaternion(getRotationFromMatrix(nodeData->transformation));
	sceneObject->setScale(getScaleFromMatrix(nodeData->transformation));

	_modelBoneHelperDatas.push_back({ sceneObject, nodeData });

	for (AnimationNodeData& child : nodeData->children)
	{
		createBoneObjectForModel(&child, sceneObject);
	}
}


void SkeletalAnimationHelperComponent::createBoneObjectForAnimation(AnimationNodeData* nodeData, SceneObject* parent)
{
	SceneObject* sceneObject = createBoneObject(nodeData, glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));

	parent->addChild(sceneObject);

	Bone* bone = _animation->findBone(nodeData->name);
	if (bone != nullptr)
	{
		bone->update(0.0f * _animation->getTicksPerSecond());

		sceneObject->setPosition(getTranslationFromMatrix(bone->getLocalTransform()));
		sceneObject->setRotationQuaternion(getRotationFromMatrix(bone->getLocalTransform()));
		sceneObject->setScale(getScaleFromMatrix(bone->getLocalTransform()));
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
		int index = boneInfo->second.id;

		SceneObject* sceneObject = createBoneObject(nodeData, glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
		parent->addChild(sceneObject);

		_finalBonesHelperObjects[index] = sceneObject;
	}

	for (AnimationNodeData& child : nodeData->children)
	{
		createFinalBoneObject(&child, parent);
	}
}


void SkeletalAnimationHelperComponent::update(float deltaTime)
{
	for (const auto& data : _modelBoneHelperDatas)
	{
		data.animationNode->transformation = data.helperObject->getLocalTransformMatrix();
	}

	if (_animationBonesTransformFromAnimation)
	{
		for (const auto& data : _animationBoneHelperDatas)
		{
			Bone* bone = _animation->findBone(data.animationNode->name);
			if (bone != nullptr)
			{
				data.helperObject->setPosition(getTranslationFromMatrix(bone->getLocalTransform()));
				data.helperObject->setRotationQuaternion(getRotationFromMatrix(bone->getLocalTransform()));
				data.helperObject->setScale(getScaleFromMatrix(bone->getLocalTransform()));
			}
		}
	}
	else if (_animationBonesTransformDefault)
	{
		for (const auto& data : _animationBoneHelperDatas)
		{
			data.helperObject->setPosition(getTranslationFromMatrix(data.animationNode->transformation));
			data.helperObject->setRotationQuaternion(getRotationFromMatrix(data.animationNode->transformation));
			data.helperObject->setScale(getScaleFromMatrix(data.animationNode->transformation));
		}
	}

	SkeletalAnimationComponent2* skeletalAnimationComponent = static_cast<SkeletalAnimationComponent2*>(getSceneObject()->getComponent(CT_SKELETAL_ANIMATION_2));
	for (int i = 0; i < 100; ++i)
	{
		if (_finalBonesHelperObjects[i] != nullptr)
		{
			_finalBonesHelperObjects[i]->setPosition(getTranslationFromMatrix(skeletalAnimationComponent->_finalBoneMatrices[i]));
			_finalBonesHelperObjects[i]->setRotationQuaternion(getRotationFromMatrix(skeletalAnimationComponent->_finalBoneMatrices[i]));
			//_finalBonesHelperObjects[i]->setScale(getScaleFromMatrix(skeletalAnimationComponent->_finalBoneMatrices[i]));
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
