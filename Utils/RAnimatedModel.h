#ifndef RANIMATEDMODEL_H_INCLUDED
#define RANIMATEDMODEL_H_INCLUDED


#include <unordered_map>

#include <glm/glm.hpp>

#include "RStaticModel.h"

#include "../Graphics/AnimationNodeData.h"


struct BoneInfo final
{
	int id;
	glm::mat4 offset;

};


class RAnimatedModel : public RStaticModel
{
	friend class AnimatedModelLoader;

	private:
		std::unordered_map<std::string, BoneInfo*> _boneInfos;
		AnimationNodeData _bonesRootNode;

	public:
		RAnimatedModel(const std::string& path, StaticModelNode* rootNode, const std::vector<Material*>& materials,
					   const std::unordered_map<std::string, BoneInfo*>& boneInfos,
					   GLenum primitiveType = GL_TRIANGLES, glm::vec3* collisionMesh = nullptr, unsigned int collisionMeshSize = 0);
		virtual ~RAnimatedModel();

		inline const std::unordered_map<std::string, BoneInfo*>& getBoneInfos() { return _boneInfos; }
		inline AnimationNodeData* getBonesRootNode() { return &_bonesRootNode; }

};


#endif // RANIMATEDMODEL_H_INCLUDED
