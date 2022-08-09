#include "AnimationLoader.h"


#include "../Utils/AssimpGlmConverter.h"
#include "../Utils/Logger.h"


void AnimationLoader::loadNode(const aiNode* assimpNode, AnimationNodeData& outNode)
{
	outNode.name = std::string(assimpNode->mName.C_Str());
	outNode.transformation = AssimpGlmConverter::toMat4(assimpNode->mTransformation);

	for (unsigned int i = 0; i < assimpNode->mNumChildren; ++i)
	{
		AnimationNodeData child;
		loadNode(assimpNode->mChildren[i], child);
		outNode.children.push_back(child);
	}
}


Bone* AnimationLoader::loadBone(const aiNodeAnim* chanel)
{
	Bone* bone = new Bone(chanel->mNodeName.C_Str());

	for (unsigned int i = 0; i < chanel->mNumPositionKeys; ++i)
	{
		KeyPosition keyPosition;
		keyPosition.value = AssimpGlmConverter::toVec3(chanel->mPositionKeys[i].mValue);
		keyPosition.timestamp = chanel->mPositionKeys[i].mTime;

		bone->_positions.push_back(keyPosition);
	}

	for (unsigned int i = 0; i < chanel->mNumRotationKeys; ++i)
	{
		KeyRotation keyRotation;
		keyRotation.value = AssimpGlmConverter::toQuat(chanel->mRotationKeys[i].mValue);
		keyRotation.timestamp = chanel->mRotationKeys[i].mTime;

		bone->_rotations.push_back(keyRotation);
	}

	for (unsigned int i = 0; i < chanel->mNumScalingKeys; ++i)
	{
		KeyScale keyScale;
		keyScale.value = AssimpGlmConverter::toVec3(chanel->mScalingKeys[i].mValue);
		keyScale.timestamp = chanel->mScalingKeys[i].mTime;

		bone->_scales.push_back(keyScale);
	}

	return bone;
}


void AnimationLoader::loadBones(const aiAnimation* assimpAnimation, std::unordered_map<std::string, Bone*>& outBones)
{
	for (unsigned int i = 0; i < assimpAnimation->mNumChannels; ++i)
	{
		const auto& chanel = assimpAnimation->mChannels[i];

		Bone* bone = loadBone(chanel);
		outBones.insert(std::make_pair(bone->getName(), bone));
	}
}


RAnimation* AnimationLoader::loadAnimation(const std::string& fileName)
{
	Assimp::Importer assimpImporter;
	const aiScene* assimpScene = assimpImporter.ReadFile(fileName, IMPORT_FLAGS);

	if (assimpScene == nullptr)
	{
		LOG_ERROR("Error parsing file: " + fileName + ": " + assimpImporter.GetErrorString());
		return nullptr;
	}

	if (!assimpScene->HasAnimations())
	{
		LOG_ERROR("File: " + fileName + ": has no animation!");
		return nullptr;
	}

	aiAnimation* assimpAnimation = assimpScene->mAnimations[0];

	RAnimation* animation = new RAnimation(fileName);

	animation->_duration = assimpAnimation->mDuration;
	animation->_ticksPerSecond = assimpAnimation->mTicksPerSecond;

	loadNode(assimpScene->mRootNode, animation->_rootNode);
	loadBones(assimpAnimation, animation->_bones);

	return animation;
}