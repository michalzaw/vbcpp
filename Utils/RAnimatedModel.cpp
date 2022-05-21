#include "RAnimatedModel.h"


RAnimatedModel::RAnimatedModel(const std::string& path, StaticModelNode* rootNode, const std::vector<Material*>& materials,
							   const std::map<std::string, BoneInfo>& boneInfos,
							   GLenum primitiveType, glm::vec3* collisionMesh, unsigned int collisionMeshSize)
	: RStaticModel(path, rootNode, materials, primitiveType, collisionMesh, collisionMeshSize),
	_boneInfos(boneInfos)
{

}


RAnimatedModel::~RAnimatedModel()
{

}
