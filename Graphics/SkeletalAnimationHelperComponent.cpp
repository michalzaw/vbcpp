#include "SkeletalAnimationHelperComponent.h"

#include "RAnimation.h"

#include "../Scene/SceneManager.h"

#include "../Utils/RAnimatedModel.h"


SkeletalAnimationHelperComponent::SkeletalAnimationHelperComponent(RAnimation* animation, RAnimatedModel* animatedModel)
	: Component(CT_SKELETAL_ANIMATION_HELPER),
	_animation(animation), _animatedModel(animatedModel)
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
	SceneObject* modelSceneObject = getSceneObject()->getSceneManager()->addSceneObject("Model");
	SceneObject* animationSceneObject = getSceneObject()->getSceneManager()->addSceneObject("Animation");

	getSceneObject()->addChild(modelSceneObject);
	getSceneObject()->addChild(animationSceneObject);

	createBoneObjectForModel(&_animatedModel->_bonesRootNode, modelSceneObject);
	createBoneObjectForAnimation(_animation->getRootNode(), animationSceneObject);
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


void SkeletalAnimationHelperComponent::update(float deltaTime)
{
	for (const auto& data : _modelBoneHelperDatas)
	{
		data.animationNode->transformation = data.helperObject->getLocalTransformMatrix();
	}

	for (const auto& data : _animationBoneHelperDatas)
	{
		Bone* bone = _animation->findBone(data.animationNode->name);
		if (bone != nullptr)
		{

			data.helperObject->setPosition(getTranslationFromMatrix(bone->getLocalTransform()));
			data.helperObject->setRotationQuaternion(getRotationFromMatrix(bone->getLocalTransform()));
			data.helperObject->setScale(getScaleFromMatrix(bone->getLocalTransform()));
		}

		//data.helperObject->setPosition(getTranslationFromMatrix(data.animationNode->transformation));
		//data.helperObject->setRotationQuaternion(getRotationFromMatrix(data.animationNode->transformation));
		//data.helperObject->setScale(getScaleFromMatrix(data.animationNode->transformation));
	}
}
