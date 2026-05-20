// Generated file
#include "SkeletalAnimationComponentLuaBindings.h"

#include "../../../Graphics/SkeletalAnimationComponent.h"


namespace SkeletalAnimationComponentLuaBindings
{
    void bind(sol::state* lua)
    {
        lua->new_usertype<SkeletalAnimationComponent>("SkeletalAnimationComponent",
            sol::base_classes, sol::bases<Component>(),

            "setCurrentAnimationState", &SkeletalAnimationComponent::setCurrentAnimationState,
            "setNextAnimationState", &SkeletalAnimationComponent::setNextAnimationState,
            "getCurrentAnimationState", &SkeletalAnimationComponent::getCurrentAnimationState,
            "getNextAnimationState", &SkeletalAnimationComponent::getNextAnimationState,
            "getFinalBoneMatrices", &SkeletalAnimationComponent::getFinalBoneMatrices,
            "getAnimationSpeed", &SkeletalAnimationComponent::getAnimationSpeed,
            "isPlay", &SkeletalAnimationComponent::isPlay,
            "isLockRootBoneTranslation", &SkeletalAnimationComponent::isLockRootBoneTranslation,
            "getRootBone", &SkeletalAnimationComponent::getRootBone,
            "isEndToStartFrameBlending", &SkeletalAnimationComponent::isEndToStartFrameBlending,
            "getEndToStartFrameBlendingTime", &SkeletalAnimationComponent::getEndToStartFrameBlendingTime,
            "getStateBlendingDuration", &SkeletalAnimationComponent::getStateBlendingDuration,
            "getScale", &SkeletalAnimationComponent::getScale,
            "setAnimationSpeed", &SkeletalAnimationComponent::setAnimationSpeed,
            "setPlay", &SkeletalAnimationComponent::setPlay,
            "setLockRootBoneTranslation", &SkeletalAnimationComponent::setLockRootBoneTranslation,
            "setRootBone", &SkeletalAnimationComponent::setRootBone,
            "setEndToStartFrameBlending", &SkeletalAnimationComponent::setEndToStartFrameBlending,
            "setEndToStartFrameBlendingTime", &SkeletalAnimationComponent::setEndToStartFrameBlendingTime,
            "setStateBlendingDuration", &SkeletalAnimationComponent::setStateBlendingDuration,
            "setScale", &SkeletalAnimationComponent::setScale,
            "getRootBonePositionInStartFrame", &SkeletalAnimationComponent::getRootBonePositionInStartFrame,
            "getRootBonePositionInEndFrame", &SkeletalAnimationComponent::getRootBonePositionInEndFrame,
            "getRootBonePositionInFrame", &SkeletalAnimationComponent::getRootBonePositionInFrame



        );
    }
}
