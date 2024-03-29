#include <iostream>

#include "Model.h"


/*Model::Model(OGLDriver* driver, Vertex* vertices, unsigned int quantumOfVertices, Mesh* meshes, unsigned int quantumOfMeshes, GLenum primitiveType)
    : _oglDriver(driver)
{
    std::cout << "aaaaaaaa\n\n\n\n\naaaaaaaaaaaaa";
    _vertices = vertices;
    _quantumOfVertices = quantumOfVertices;
    std::cout << _quantumOfVertices << std::endl;

    _indices = new unsigned int[_quantumOfVertices];
    _indicesSize = quantumOfVertices;
    std::cout << _indicesSize << std::endl;

    for (int i = 0; i < _indicesSize; ++i)
        _indices[i] = i;

    _meshes = meshes;
    _quantumOfMeshes = quantumOfMeshes;

    _vbo = _oglDriver->CreateVBO(quantumOfVertices * sizeof(Vertex));
    _vbo->AddVertexData(vertices, quantumOfVertices);

    _ibo = _oglDriver->CreateIBO(_indicesSize * sizeof(unsigned int));
    _ibo->AddIndices(_indices, _indicesSize);

    _primitiveType = primitiveType;

}*/


Model::Model(Vertex* vertices,
             unsigned int quantumOfVertices,
             unsigned int* indices,
             unsigned int indicesSize,
             Mesh* meshes,
             unsigned int quantumOfMeshes,
             glm::vec3* collisionMesh,
             unsigned int collisionMeshSize,
             GLenum primitiveType)
{
    std::cout << "*** Model: Konstruktor" << std::endl;


    _vertices = vertices;
    _quantumOfVertices = quantumOfVertices;

    _indices = indices;
    _indicesSize = indicesSize;

    _meshes = meshes;
    _quantumOfMeshes = quantumOfMeshes;

    _vbo = OGLDriver::getInstance().createVBO(quantumOfVertices * sizeof(Vertex));
    _vbo->addVertexData(vertices, quantumOfVertices);

    _ibo = OGLDriver::getInstance().createIBO(_indicesSize * sizeof(unsigned int));
    _ibo->addIndices(_indices, _indicesSize);

    _collisionMesh = collisionMesh;
    _collisionMeshSize = collisionMeshSize;

    _primitiveType = primitiveType;

}


Model::~Model()
{
    std::cout << "*** Model: Destruktor" << std::endl;

    std::cout << "*** Model: Usuwanie _vbo" << std::endl;
    if (_vbo)
    {
        OGLDriver::getInstance().deleteVBO(_vbo);
        _vbo = 0;
    }

    std::cout << "*** Model: Usuwanie _ibo" << std::endl;
    if (_ibo)
    {
        OGLDriver::getInstance().deleteIBO(_ibo);
        _ibo = 0;
    }

    std::cout << "*** Model: Usuwanie _vertices" << std::endl;
    if (_vertices)
    {
        delete[] _vertices;
        _vertices = 0;
    }

    std::cout << "*** Model: Usuwanie _indices" << std::endl;
    if (_indices)
    {
        delete[] _indices;
        _indices = 0;
    }

    // >>>>>

    std::cout << "*** Model: Usuwanie _meshes" << std::endl;
    if (_meshes)
    {
        delete[] _meshes;
        _meshes = 0;
    }

    std::cout << "*** Model: Usuwanie _collisionMesh" << std::endl;
    if (_collisionMesh)
    {
        delete[] _collisionMesh;
        _collisionMesh = 0;
    }
}

VBO* Model::getVBO()
{
    return _vbo;
}


IBO* Model::getIBO()
{
    return _ibo;
}


unsigned int Model::getQuantumOfVertices()
{
    return _quantumOfVertices;
}


Vertex* Model::getVertices()
{
    return _vertices;
}


unsigned int Model::getIndicesSize()
{
    return _indicesSize;
}


unsigned int* Model::getIndices()
{
    return _indices;
}


unsigned int Model::getQuantumOfMeshes()
{
    return _quantumOfMeshes;
}


Mesh* Model::getMesh(unsigned int i)
{
    if (i < _quantumOfMeshes)
        return &_meshes[i];
    else
        return 0;
}


unsigned int Model::getCollisionMeshSize()
{
    return _collisionMeshSize;
}


glm::vec3* Model::getCollisionMesh()
{
    return _collisionMesh;
}


GLenum Model::getPrimitiveType()
{
    return _primitiveType;
}


Mesh* Model::getMeshes()
{
    return _meshes;
}
