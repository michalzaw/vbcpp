#include "RStaticModel.h"


RStaticModel::RStaticModel(string path, StaticModelMesh* meshes, unsigned int meshesCount, GLenum primitiveType,
             glm::vec3* collisionMesh, unsigned int collisionMeshSize)
    : Resource(RT_MODEL, path)
{
    _meshes = meshes;
    _meshesCount = meshesCount;

    _collisionMesh = collisionMesh;
    _collisionMeshSize = collisionMeshSize;

    _primitiveType = primitiveType;


    calculateAABB();
    createAabbVbo();
}


RStaticModel::~RStaticModel()
{
    if (_meshes)
    {
        delete[] _meshes;
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


void RStaticModel::calculateAABB()
{
    glm::vec3 min(0.0f, 0.0f, 0.0f);
    glm::vec3 max(0.0f, 0.0f, 0.0f);

    for (int i = 0; i < _meshesCount; ++i)
    {
        for (int j = 0; j < _meshes[i].verticesCount; ++j)
        {
            if (_meshes[i].vertices[j].position.x < min.x)
                min.x = _meshes[i].vertices[j].position.x;
            else if (_meshes[i].vertices[j].position.x > max.x)
                max.x = _meshes[i].vertices[j].position.x;

            if (_meshes[i].vertices[j].position.y < min.y)
                min.y = _meshes[i].vertices[j].position.y;
            else if (_meshes[i].vertices[j].position.y > max.y)
                max.y = _meshes[i].vertices[j].position.y;

            if (_meshes[i].vertices[j].position.z < min.z)
                min.z = _meshes[i].vertices[j].position.z;
            else if (_meshes[i].vertices[j].position.z > max.z)
                max.z = _meshes[i].vertices[j].position.z;
        }
    }

    _aabb.setSize(min, max);
}


void RStaticModel::createAabbVbo()
{

}


unsigned int RStaticModel::getMeshesCount()
{
    return _meshesCount;
}


StaticModelMesh* RStaticModel::getMesh(unsigned int i)
{
    if (i < _meshesCount)
        return &_meshes[i];
    else
        return NULL;
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
