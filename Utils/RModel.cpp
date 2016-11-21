#include "RModel.h"

RModel::RModel(std::string path, Model* m)
: Resource(path)
{
    std::cout << "*** RModel: Konstruktor: " << _path << std::endl;

    //_oglDriver = m->getDriver();

    _vertices = m->getVertices();
    _quantumOfVertices = m->getQuantumOfVertices();

    _indices = m->getIndices();
    _indicesSize = m->getIndicesSize();

    _meshes = m->getMeshes();
    _quantumOfMeshes = m->getQuantumOfMeshes();

    _vbo = m->getVBO();

    _ibo = m->getIBO();

    _collisionMesh = m->getCollisionMesh();
    _collisionMeshSize = m->getCollisionMeshSize();

    _primitiveType = m->getPrimitiveType();
}

RModel::~RModel()
{
    std::cout << "*** RModel: Destruktor: " << _path << std::endl;

    std::cout << "*** RModel: Usuwanie _vbo" << std::endl;
    if (_vbo)
    {
        OGLDriver::getInstance().deleteVBO(_vbo);
        _vbo = 0;
    }

    std::cout << "*** RModel: Usuwanie _ibo" << std::endl;
    if (_ibo)
    {
        OGLDriver::getInstance().deleteIBO(_ibo);
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

VBO* RModel::getVBO()
{
    return _vbo;
}


IBO* RModel::getIBO()
{
    return _ibo;
}


unsigned int RModel::getQuantumOfVertices()
{
    return _quantumOfVertices;
}


Vertex* RModel::getVertices()
{
    return _vertices;
}


unsigned int RModel::getIndicesSize()
{
    return _indicesSize;
}


unsigned int* RModel::getIndices()
{
    return _indices;
}


unsigned int RModel::getQuantumOfMeshes()
{
    return _quantumOfMeshes;
}


Mesh* RModel::getMesh(unsigned int i)
{
    if (i < _quantumOfMeshes)
        return &_meshes[i];
    else
        return 0;
}


unsigned int RModel::getCollisionMeshSize()
{
    return _collisionMeshSize;
}


glm::vec3* RModel::getCollisionMesh()
{
    return _collisionMesh;
}


GLenum RModel::getPrimitiveType()
{
    return _primitiveType;
}


Mesh* RModel::getMeshes()
{
    return _meshes;
}
