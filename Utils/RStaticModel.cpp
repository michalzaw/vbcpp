#include "RStaticModel.h"


RStaticModel::RStaticModel(const std::string& path, StaticModelNode* rootNode, const std::vector<Material*>& materials,
                           GLenum primitiveType, glm::vec3* collisionMesh, unsigned int collisionMeshSize)
    : Resource(RT_MODEL, path)
{
    _rootNode = rootNode;

    _materials = materials;

    _collisionMesh = collisionMesh;
    _collisionMeshSize = collisionMeshSize;

    _primitiveType = primitiveType;


    calculateAABB();
}


RStaticModel::~RStaticModel()
{
    if (_rootNode)
    {
        delete _rootNode;
    }

    for (int i = 0; i < _materials.size(); ++i)
    {
        delete _materials[i];
    }

    if (_collisionMesh)
    {
        delete[] _collisionMesh;
    }
}


void RStaticModel::findMinAndMaxVertices(StaticModelNode* node, glm::mat4 parentTransform, glm::vec3& min, glm::vec3& max)
{
    glm::mat4 nodeTransform = parentTransform * node->transform.getTransformMatrix();

    glm::vec3 nodeMin(0, 0, 0);
    glm::vec3 nodeMax(0, 0, 0);
    for (int i = 0; i < node->meshesCount; ++i)
    {
        for (int j = 0; j < node->meshes[i].verticesCount; ++j)
        {
            glm::vec3 vertexPosition = VerticesUtils::getVertexPosition(node->meshes[i].vertices, node->meshes[i].vertexType, j);
            glm::vec4 v = nodeTransform * glm::vec4(vertexPosition.x,
                                                    vertexPosition.y,
                                                    vertexPosition.z,
                                                    1.0f);
            if (v.x < min.x)
                min.x = v.x;
            else if (v.x > max.x)
                max.x = v.x;

            if (v.y < min.y)
                min.y = v.y;
            else if (v.y > max.y)
                max.y = v.y;

            if (v.z < min.z)
                min.z = v.z;
            else if (v.z > max.z)
                max.z = v.z;


            // local aabb for node
            glm::vec3 vl = vertexPosition;

            if (vl.x < nodeMin.x)
                nodeMin.x = vl.x;
            else if (vl.x > nodeMax.x)
                nodeMax.x = vl.x;

            if (vl.y < nodeMin.y)
                nodeMin.y = vl.y;
            else if (vl.y > nodeMax.y)
                nodeMax.y = vl.y;

            if (vl.z < nodeMin.z)
                nodeMin.z = vl.z;
            else if (vl.z > nodeMax.z)
                nodeMax.z = vl.z;
        }
    }

    node->aabb.setSize(nodeMin, nodeMax);

    for (int i = 0; i < node->children.size(); ++i)
    {
        findMinAndMaxVertices(node->children[i], nodeTransform, min, max);
    }
}


void RStaticModel::calculateAABB()
{
    glm::vec3 min(0.0f, 0.0f, 0.0f);
    glm::vec3 max(0.0f, 0.0f, 0.0f);

    findMinAndMaxVertices(_rootNode, glm::mat4(1.0f), min, max);

    _aabb.setSize(min, max);
}


StaticModelNode* RStaticModel::getRootNode()
{
    return _rootNode;
}


StaticModelNode* RStaticModel::getNodeByName(const std::string& name)
{
    if (_rootNode->name == name)
    {
        return _rootNode;
    }

    return _rootNode->getNodeByName(name);
}


/*unsigned int RStaticModel::getMeshesCount()
{
    return _meshesCount;
}


StaticModelMesh* RStaticModel::getMesh(unsigned int i)
{
    if (i < _meshesCount)
        return &_meshes[i];
    else
        return NULL;
}*/


std::vector<Material*>& RStaticModel::getMaterials()
{
    return _materials;
}


Material* RStaticModel::getMaterial(unsigned int i)
{
    if (i < _materials.size())
        return _materials[i];
    else
        return NULL;
}


unsigned int RStaticModel::getMaterialsCount()
{
    return _materials.size();
}


unsigned int RStaticModel::getCollisionMeshSize()
{
    return _collisionMeshSize;
}


glm::vec3* RStaticModel::getCollisionMesh()
{
    return _collisionMesh;
}


AABB* RStaticModel::getAABB()
{
    return &_aabb;
}


void RStaticModel::setNewCollisionMesh(glm::vec3* collisionMesh, unsigned int collisionMeshSize)
{
	if (_collisionMesh)
	{
		delete[] _collisionMesh;
	}

	_collisionMesh = collisionMesh;
	_collisionMeshSize = collisionMeshSize;
}


void RStaticModel::recalculateAABB()
{
	calculateAABB();
}
