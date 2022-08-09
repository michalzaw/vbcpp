#ifndef ANIMATEDMODELLOADER_H_INCLUDED
#define ANIMATEDMODELLOADER_H_INCLUDED


#include "StaticModelLoader.h"

#include "../Utils/RAnimatedModel.h"


class AnimatedModelLoader : public StaticModelLoader
{
	private:
		std::unordered_map<std::string, BoneInfo*> _boneInfos;
		int _boneCounter;

		void setBoneDataInVertex(AnimatedVertex& vertex, int boneId, float weight);

		void extractBoneWeights(AnimatedVertex* vertices, unsigned int verticesCount, const aiMesh* assimpMesh);

		void loadNode(const aiNode* assimpNode, AnimationNodeData& outNode); // todo: animation - ta sama funkcja co w AnimationLoader

		void getTransformFromAssimpNode(aiNode* assimpNode, Transform& transform) override;
		bool loadMeshFromNode(const aiMesh* assimpMesh, StaticModelMesh& mesh, bool isLoadingSingleNode, const glm::mat4& globalNodeTransform) override;

	public:
		AnimatedModelLoader();
		~AnimatedModelLoader();

		RAnimatedModel* loadAnimatedModelWithHierarchy(const std::string& fileName, const std::string& texturesPath);

};


#endif // ANIMATEDMODELLOADER_H_INCLUDED
