#ifndef RANIMATIONLOADER_H_INCLUDED
#define RANIMATIONLOADER_H_INCLUDED


#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "RAnimation.h"


class AnimationLoader final
{
	private:
		static const unsigned int IMPORT_FLAGS = aiProcess_Triangulate;

		void loadNode(const aiNode* assimpNode, AnimationNodeData& outNode);
		Bone* loadBone(const aiNodeAnim* chanel);
		void loadBones(const aiAnimation* assimpAnimation, std::unordered_map<std::string, Bone*>& outBones);

	public:
		RAnimation* loadAnimation(const std::string& fileName);
		
};


#endif // RANIMATIONLOADER_H_INCLUDED
