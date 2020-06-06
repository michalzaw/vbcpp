#include "RModel.h"

RModel::RModel(std::string path, Model* m)
: Resource(RT_MODEL, path)
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

    calculateAABB();
    createAabbVbo();
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


AABB* RModel::getAABB()
{
    return &_aabb;
}


void RModel::calculateAABB()
{
    glm::vec3 min(0.0f, 0.0f, 0.0f);
    glm::vec3 max(0.0f, 0.0f, 0.0f);

    for (int i = 0; i < _quantumOfVertices; ++i)
    {
        if (_vertices[i].position.x < min.x)
            min.x = _vertices[i].position.x;
        else if (_vertices[i].position.x > max.x)
            max.x = _vertices[i].position.x;

        if (_vertices[i].position.y < min.y)
            min.y = _vertices[i].position.y;
        else if (_vertices[i].position.y > max.y)
            max.y = _vertices[i].position.y;

        if (_vertices[i].position.z < min.z)
            min.z = _vertices[i].position.z;
        else if (_vertices[i].position.z > max.z)
            max.z = _vertices[i].position.z;
    }

    _aabb.setSize(min, max);
}


void RModel::createAabbVbo()
{
    glm::vec3 min = _aabb.getMinCoords();
    glm::vec3 max = _aabb.getMaxCoords();

    Vertex vertices[8];
    vertices[0].position = glm::vec3(min.x, min.y, min.z);
    vertices[1].position = glm::vec3(min.x, min.y, max.z);
    vertices[2].position = glm::vec3(min.x, max.y, min.z);
    vertices[3].position = glm::vec3(min.x, max.y, max.z);
    vertices[4].position = glm::vec3(max.x, min.y, min.z);
    vertices[5].position = glm::vec3(max.x, min.y, max.z);
    vertices[6].position = glm::vec3(max.x, max.y, min.z);
    vertices[7].position = glm::vec3(max.x, max.y, max.z);

    _aabbVbo = OGLDriver::getInstance().createVBO(8 * sizeof(Vertex));
    _aabbVbo->addVertexData(vertices, 8);

    unsigned int indices[24] = {0, 1, 1, 3, 3, 2, 2, 0, 4, 5, 5, 7, 7, 6, 6, 4, 1, 5, 3, 7, 2, 6, 0, 4};

    _aabbIbo = OGLDriver::getInstance().createIBO(24 * sizeof(unsigned int));
    _aabbIbo->addIndices(indices, 24);
}
