#include "SkeletalAnimationComponent2.h"

#include "RAnimation.h"
#include "RenderObject.h"
#include "SkeletalAnimationHelperComponent.h"

#include "../Scene/SceneManager.h"

#include "../Utils/RAnimatedModel.h"


SkeletalAnimationComponent2::SkeletalAnimationComponent2(RAnimation* animation, const std::unordered_map<std::string, std::string>& animationNodeNameToBoneNameInModelMap)
	: Component(CT_SKELETAL_ANIMATION_2),
	_animation(animation),
	_finalBoneMatrices(MAX_BONES, glm::mat4(1.0f)),
	_currentTime(0.0f),
	_animationDuration(animation->getDuration()), _animationTicksPerSecond(animation->getTicksPerSecond()), _play(true),
	_animatedModel(nullptr)
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
}


void SkeletalAnimationComponent2::calculateBoneTransformInModel(AnimationNodeData* node, const glm::mat4& parentTransform)
{
	const std::string& nodeName = node->name;

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


void SkeletalAnimationComponent2::calculateBoneTransformInAnimation(AnimationNodeData* node, const glm::mat4& parentTransform)
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
	calculateBoneTransformInAnimation(_animation->getRootNode());
}