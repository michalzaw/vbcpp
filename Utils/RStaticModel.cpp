#include "RStaticModel.h"


RStaticModel::RStaticModel(string path, StaticModelNode* rootNode, Material* materials, unsigned int materialsCount
                           , GLenum primitiveType, glm::vec3* collisionMesh, unsigned int collisionMeshSize)
    : Resource(RT_MODEL, path)
{
    _rootNode = rootNode;

    _materials = materials;
    _materialsCount = materialsCount;

    _collisionMesh = collisionMesh;
    _collisionMeshSize = collisionMeshSize;

    _primitiveType = primitiveType;


    calculateAABB();
    createAabbVbo();
}


RStaticModel::~RStaticModel()
{
    if (_rootNode)
    {
        delete _rootNode;
    }

    if (_materials)
    {
        delete[] _materials;
    }

    if (_collisionMesh)
    {
        delete[] _collisionMesh;
    }

    if (_aabbVbo)
    {
        OGLDriver::getInstance().deleteVBO(_aabbVbo);
    }

    if (_aabbIbo)
    {
        OGLDriver::getInstance().deleteIBO(_aabbIbo);
    }
}


void RStaticModel::findMinAndMaxVertices(StaticModelNode* node, glm::mat4 parentTransform, glm::vec3& min, glm::vec3& max)
{
    glm::mat4 nodeTransform = parentTransform * node->transform.getTransformMatrix();

    for (int i = 0; i < node->meshesCount; ++i)
    {
        for (int j = 0; j < node->meshes[i].verticesCount; ++j)
        {
            glm::vec4 v = nodeTransform * glm::vec4(node->meshes[i].vertices[j].position.x,
                                                    node->meshes[i].vertices[j].position.y,
                                                    node->meshes[i].vertices[j].position.z,
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
        }
    }

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


void RStaticModel::createAabbVbo()
{

}


StaticModelNode* RStaticModel::getRootNode()
{
    return _rootNode;
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


Material* RStaticModel::getMaterial(unsigned int i)
{
    if (i < _materialsCount)
        return &_materials[i];
    else
        return NULL;
}


unsigned int RStaticModel::getMaterialsCount()
{
    return _materialsCount;
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


VBO* RStaticModel::getAabbVbo()
{
    return _aabbVbo;
}


IBO* RStaticModel::getAabbIbo()
{
    return _aabbIbo;
}
