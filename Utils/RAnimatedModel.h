#ifndef RANIMATEDMODEL_H_INCLUDED
#define RANIMATEDMODEL_H_INCLUDED


#include <map>

#include <glm/glm.hpp>

#include "RStaticModel.h"


struct BoneInfo final
{
	int id;
	glm::mat4 offset;

};


class RAnimatedModel : public RStaticModel
{
	private:
		std::map<std::string, BoneInfo> _boneInfos;

	public:
		RAnimatedModel(const std::string& path, StaticModelNode* rootNode, const std::vector<Material*>& materials,
					   const std::map<std::string, BoneInfo>& boneInfos,
					   GLenum primitiveType = GL_TRIANGLES, glm::vec3* collisionMesh = nullptr, unsigned int collisionMeshSize = 0);
		virtual ~RAnimatedModel();

		inline const std::map<std::string, BoneInfo>& getBoneInfos() { return _boneInfos; }

};


#endif // RANIMATEDMODEL_H_INCLUDED
