#include "RModel.h"

RModel::RModel(std::string path, Model* m)
: Resource(path)
{
    std::cout << "*** RModel: Konstruktor: " << _path << std::endl;

    //_oglDriver = m->getDriver();

    _vertices = m->GetVertices();
    _quantumOfVertices = m->GetQuantumOfVertices();

    _indices = m->GetIndices();
    _indicesSize = m->GetIndicesSize();

    _meshes = m->getMeshes();
    _quantumOfMeshes = m->GetQuantumOfMeshes();

    _vbo = m->GetVBO();

    _ibo = m->GetIBO();

    _collisionMesh = m->GetCollisionMesh();
    _collisionMeshSize = m->GetCollisionMeshSize();

    _primitiveType = m->GetPrimitiveType();
}

RModel::~RModel()
{
    std::cout << "*** RModel: Destruktor: " << _path << std::endl;

    std::cout << "*** RModel: Usuwanie _vbo" << std::endl;
    if (_vbo)
    {
        OGLDriver::getInstance().DeleteVBO(_vbo);
        _vbo = 0;
    }

    std::cout << "*** RModel: Usuwanie _ibo" << std::endl;
    if (_ibo)
    {
        OGLDriver::getInstance().DeleteIBO(_ibo);
        _ibo = 0;
    }

    std::cout << "*** RModel: Usuwanie _vertices" << std::endl;
    if (_vertices)
    {
        delete[] _vertices;
        _vertices = 0;
    }

    std::cout << "*** RModel: Usuwanie _indices" << std::endl;
    if (_indices)
    {
        delete[] _indices;
        _indices = 0;
    }

    // >>>>>

    std::cout << "*** RModel: Usuwanie _meshes" << std::endl;
    if (_meshes)
    {
        delete[] _meshes;
        _meshes = 0;
    }

    std::cout << "*** RModel: Usuwanie _collisionMesh" << std::endl;
    if (_collisionMesh)
    {
        delete[] _collisionMesh;
        _collisionMesh = 0;
    }
}

VBO* RModel::GetVBO()
{
    return _vbo;
}


IBO* RModel::GetIBO()
{
    return _ibo;
}


unsigned int RModel::GetQuantumOfVertices()
{
    return _quantumOfVertices;
}


Vertex* RModel::GetVertices()
{
    return _vertices;
}


unsigned int RModel::GetIndicesSize()
{
    return _indicesSize;
}


unsigned int* RModel::GetIndices()
{
    return _indices;
}


unsigned int RModel::GetQuantumOfMeshes()
{
    return _quantumOfMeshes;
}


Mesh* RModel::GetMesh(unsigned int i)
{
    if (i < _quantumOfMeshes)
        return &_meshes[i];
    else
        return 0;
}


unsigned int RModel::GetCollisionMeshSize()
{
    return _collisionMeshSize;
}


glm::vec3* RModel::GetCollisionMesh()
{
    return _collisionMesh;
}


GLenum RModel::GetPrimitiveType()
{
    return _primitiveType;
}


Mesh* RModel::getMeshes()
{
    return _meshes;
}
