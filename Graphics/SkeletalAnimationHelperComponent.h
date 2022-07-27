#ifndef SKELETALANIMATIONHELPERCOMPONENT_H_INCLUDED
#define SKELETALANIMATIONHELPERCOMPONENT_H_INCLUDED


#include <vector>

#include "../Scene/Component.h"


class RAnimation;
class RAnimatedModel;
struct AnimationNodeData;
struct FinalSkeletonNode;


struct BoneHelperData final
{
	SceneObject* helperObject;
	AnimationNodeData* animationNode;
};

struct BoneHelperData2 final
{
	SceneObject* helperObject;
	FinalSkeletonNode* animationNode;
};


glm::vec3 getTranslationFromMatrix(const glm::mat4& transformMatrix);
glm::quat getRotationFromMatrix(const glm::mat4& transformMatrix);


class SkeletalAnimationHelperComponent final : public Component
{
	private:
		RAnimation* _animation;
		RAnimatedModel* _animatedModel;

		SceneObject* _modelRootSceneObject;
		SceneObject* _animationRootSceneObject;
		SceneObject* _finalBonesRootSceneObject;
		SceneObject* _finalSkeletonRootSceneObject;
		std::vector<BoneHelperData> _modelBoneHelperDatas;
		std::vector<BoneHelperData> _animationBoneHelperDatas;
		std::vector<SceneObject*>	_finalBonesHelperObjects;
		std::vector<BoneHelperData2>_finalSkeletonHelperObjects;

		bool _showModelBones;
		bool _showAnimationBones;
		bool _showFinalBones;
		bool _showFinalSkeletonBones;

		bool _animationBonesTransformFromAnimation;
		bool _animationBonesTransformDefault;
		bool _animationBonesTransformCustom;

		void onAttachedToScenObject() override;

		SceneObject* createBoneObject(AnimationNodeData* nodeData, const glm::vec4& color);
		void createBoneObjectForModel(AnimationNodeData* nodeData, SceneObject* parent);
		void createBoneObjectForAnimation(AnimationNodeData* nodeData, SceneObject* parent);
		void createFinalBoneObject(AnimationNodeData* nodeData, SceneObject* parent);

	public:
		SkeletalAnimationHelperComponent(RAnimation* animation, RAnimatedModel* animatedModel);
		~SkeletalAnimationHelperComponent();

		void update(float deltaTime) override;

		inline bool isShowModelBones() { return _showModelBones; }
		inline bool isShowAnimationBones() { return _showAnimationBones; }
		inline bool isShowFinalBones() { return _showFinalBones; }
		inline bool isShowFinalSkeletonBones() { return _showFinalSkeletonBones; }
		inline bool isAnimationBonesTransformFromAnimation() { return _animationBonesTransformFromAnimation; }
		inline bool isAnimationBonesTransformDefault() { return _animationBonesTransformDefault; }
		inline bool isAnimationBonesTransformCustom() { return _animationBonesTransformCustom; }

		void setShowModelBones(bool showModelBones);
		void setShowAnimationBones(bool showAnimationBones);
		void setShowFinalBones(bool showFinalBones);
		void setShowFinalSkeletonBones(bool showFinalSkeletonBones);
		void setAnimationBonesTransformFromAnimation(bool animationBonesTransformFromAnimation);
		void setAnimationBonesTransformDefault(bool animationBonesTransformDefault);
		void setAnimationBonesTransformCustom(bool animationBonesTransformCustom);

};


#endif // SKELETALANIMATIONHELPERCOMPONENT_H_INCLUDED
