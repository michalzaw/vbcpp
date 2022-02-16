#ifndef MODELGENERATOR_H_INCLUDED
#define MODELGENERATOR_H_INCLUDED


#include <glm/glm.hpp>


class StaticModelMesh;


namespace ModelGenerator
{
	glm::vec3* generateCollistionMesh(StaticModelMesh* meshes, unsigned int meshesCount, unsigned int totalIndicesCount);
}


#endif // MODELGENERATOR_H_INCLUDED
