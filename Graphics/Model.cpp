#include <iostream>

#include "Model.h"


Model::Model(Vertex* vertices, unsigned int quantumOfVertices, Mesh* meshes, unsigned int quantumOfMeshes, GLenum primitiveType)
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

    _vbo = new VBO;
    _vbo->Create(vertices, quantumOfVertices);

    _ibo = new IBO;
    _ibo->Create(_indices, _indicesSize);

    _primitiveType = primitiveType;

}


Model::Model(Vertex* vertices, unsigned int quantumOfVertices, unsigned int* indices, unsigned int indicesSize, Mesh* meshes, unsigned int quantumOfMeshes, GLenum primitiveType)
{
    std::cout << "*** Model: Konstruktor" << std::endl;

    _vertices = vertices;
    _quantumOfVertices = quantumOfVertices;

    _indices = indices;
    _indicesSize = indicesSize;

    _meshes = meshes;
    _quantumOfMeshes = quantumOfMeshes;

    _vbo = new VBO;
    _vbo->Create(_vertices, _quantumOfVertices);

    _ibo = new IBO;
    _ibo->Create(_indices, _indicesSize);

    _primitiveType = primitiveType;

}


Model::~Model()
{
    std::cout << "*** Model: Destruktor" << std::endl;

    std::cout << "*** Model: Usuwanie _vbo" << std::endl;
    if (_vbo)
    {
        delete _vbo;
        _vbo = 0;
    }

    std::cout << "*** Model: Usuwanie _ibo" << std::endl;
    if (_ibo)
    {
        delete _ibo;
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
}


VBO* Model::GetVBO()
{
    return _vbo;
}


IBO* Model::GetIBO()
{
    return _ibo;
}


unsigned int Model::GetQuantumOfVertices()
{
    return _quantumOfVertices;
}


Vertex* Model::GetVertices()
{
    return _vertices;
}


unsigned int Model::GetIndicesSize()
{
    return _indicesSize;
}


unsigned int* Model::GetIndices()
{
    return _indices;
}


unsigned int Model::GetQuantumOfMeshes()
{
    return _quantumOfMeshes;
}


Mesh* Model::GetMesh(unsigned int i)
{
    if (i < _quantumOfMeshes)
        return &_meshes[i];

}


GLenum Model::GetPrimitiveType()
{
    return _primitiveType;
}
