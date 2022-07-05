#include "SkeletalAnimationComponent.h"

#include <string>

#include "RAnimation.h"
#include "RenderObject.h"

#include "../Scene/SceneManager.h"
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


// todo: animation przeniesc do osobnego pliku
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


glm::vec3 getScale(const glm::mat4& transformMatrix)
{
	glm::vec3 scale;
	glm::quat orientation;
	glm::vec3 translation;
	glm::vec3 skew;
	glm::vec4 perspective;
	glm::decompose(transformMatrix, scale, orientation, translation, skew, perspective);

	return scale;
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

		//_boneTransformOffsetsMatrices[i] = glm::translate(offset) * rot2 * glm::inverse(rot1);
		//_boneTransformOffsetsMatrices[i] = glm::inverse(_boneTransformMatricesInAnimation[i]) * _boneTransformMatricesInModel[i];
	}

	createDebugBoneObjects();
}


// todo: animation przeniesc do osobnego pliku
void SkeletalAnimationComponent::createDebugBoneObjects()
{
	createDebugBoneObjectForModel(&_animatedModel->_bonesRootNode, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), getSceneObject());
	createDebugBoneObjectForAnimation(_animation->getRootNode(), glm::vec4(0.0f, 1.0f, 0.0f, 1.0f), getSceneObject());
}


SceneObject* SkeletalAnimationComponent::createDebugBoneObject(AnimationNodeData* nodeData, const glm::vec4& color)
{
	SceneObject* sceneObject = getSceneObject()->getSceneManager()->addSceneObject(nodeData->name);

	Material* material = new Material;
	material->diffuseColor = color;
	material->shader = NOTEXTURE_ALWAYS_VISIBLE_MATERIAL;

	Cube* cube = new Cube(2.0f, material);
	cube->init();

	getSceneObject()->getSceneManager()->getGraphicsManager()->addRenderObject(cube, sceneObject);

	nodeData->_helperSceneObject = sceneObject;

	return sceneObject;
}


void SkeletalAnimationComponent::createDebugBoneObjectForModel(AnimationNodeData* nodeData, const glm::vec4& color, SceneObject* parent)
{
	SceneObject* sceneObject = createDebugBoneObject(nodeData, color);

	parent->addChild(sceneObject);

	sceneObject->setPosition(getTranslation(nodeData->transformation));

	//glm::vec3 rotationEulerAngles = glm::eulerAngles(getRotation(nodeData->transformation));
	//rotationEulerAngles = glm::vec3(-rotationEulerAngles.x, -rotationEulerAngles.y, -rotationEulerAngles.z);
	//sceneObject->setRotation(rotationEulerAngles);

	// https://stackoverflow.com/questions/17918033/glm-decompose-mat4-into-translation-and-rotation
	// funkcja glm::decompose zwraca niepoprawny kwaternion
	// aby otrzymaæ prawid³ow¹ rotacjê nale¿y wykonaæ glm::conjugate
	// conjugate tworzy tak naprawdê rotacjê odwrotn¹
	// dlatego te¿ w ObjectPropertiesWindow dzia³a³ poni¿szy kod:
	// glm::vec3 rotationEulerAngles = glm::eulerAngles(getRotation(nodeData->transformation));
	// rotationEulerAngles = glm::vec3(-rotationEulerAngles.x, -rotationEulerAngles.y, -rotationEulerAngles.z);
	// jeœli jednak chcemy operowaæ na kwaternionie, to lepiej wykonaæ glm::conjugate(orientation);
	sceneObject->setRotationQuaternion(glm::conjugate(getRotation(nodeData->transformation)));
	sceneObject->setScale(getScale(nodeData->transformation));

	for (AnimationNodeData& child : nodeData->children)
	{
		createDebugBoneObjectForModel(&child, color, sceneObject);
	}
}


void SkeletalAnimationComponent::createDebugBoneObjectForAnimation(AnimationNodeData* nodeData, const glm::vec4& color, SceneObject* parent)
{
	SceneObject* sceneObject = createDebugBoneObject(nodeData, color);

	parent->addChild(sceneObject);

	Bone* bone = _animation->findBone(nodeData->name);
	if (bone != nullptr)
	{
		bone->update(0.0f);

		sceneObject->setPosition(getTranslation(bone->getLocalTransform()));
		sceneObject->setRotationQuaternion(glm::conjugate(getRotation(bone->getLocalTransform())));
		sceneObject->setScale(getScale(bone->getLocalTransform()));
	}

	for (AnimationNodeData& child : nodeData->children)
	{
		createDebugBoneObjectForAnimation(&child, color, sceneObject);
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


// todo: animation - add const to param
void SkeletalAnimationComponent::calculateBoneTransformInModel(AnimationNodeData* node, const glm::mat4& parentTransform)
{
	const std::string& nodeName = node->name;

	if (node->_helperSceneObject != nullptr)
	{
		node->transformation = node->_helperSceneObject->getLocalTransformMatrix();
	}

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


// todo: animation - add const to param
void SkeletalAnimationComponent::calculateBoneTransform(AnimationNodeData* node, const glm::mat4& parentTransform)
{
	const std::string& nodeName = node->name;
	glm::mat4 nodeTransform = node->transformation;

	if (node->_helperSceneObject != nullptr)
	{
		node->transformation = node->_helperSceneObject->getLocalTransformMatrix();
	}

	/*Bone* bone = _animation->findBone(nodeName);
	if (bone != nullptr)
	{
		bone->update(_currentTime);
		nodeTransform = bone->getLocalTransform(); // todo: animation lepiej ¿eby update zwraca³ t¹ transformacjê, zamiast zapisywaæ j¹ wewn¹trz bone
	}*/

	glm::mat4 globalTransform = parentTransform * nodeTransform;


	//std::string boneName = mapAnimationNodeNameToBoneNameInModel(nodeName);
	//auto boneInfo = _animatedModel->getBoneInfos().find(boneName);
	int index = 0;
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

				_finalBoneMatrices[index] = globalTransform * _boneTransformOffsetsMatrices[index] * offset;
			}
		}
	}

	/*auto boneInfo = _animatedModel->getBoneInfos().find(nodeName);
	if (boneInfo != _animatedModel->getBoneInfos().end())
	{
		int index = boneInfo->second.id;
		const glm::mat4& offset = boneInfo->second.offset;

		_finalBoneMatrices[index] = globalTransform * offset;
	}*/


	for (int i = 0; i < node->children.size(); ++i)
	{
		calculateBoneTransform(&node->children[i], globalTransform);
	}
}


void SkeletalAnimationComponent::update(float deltaTime)
{
	_deltaTime = deltaTime;

	//_currentTime += _deltaTime * _animation->getTicksPerSecond();
	//_currentTime = 2.5f;
	_currentTime = fmod(_currentTime, _animation->getDuration());

	calculateBoneTransform(_animation->getRootNode());
	//calculateBoneTransformInModel(&_animatedModel->_bonesRootNode);
	//calculateBoneTransform(_animatedModel->getRootNode());
	//calculateBoneTransform(&_animatedModel->_bonesRootNode);
}
