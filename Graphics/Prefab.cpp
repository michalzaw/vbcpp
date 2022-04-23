#include "Prefab.h"

#include "../Game/GameConfig.h"

#include "../Utils/Math.h"


Prefab::Prefab(PrefabType type)
    : _prefabType(type),
    _generatedModel(nullptr)
{
    _type = CT_PREFAB;
}


Prefab::~Prefab()
{
    if (_generatedModel != nullptr)
    {
        delete _generatedModel;
    }
}


PrefabType Prefab::getPrefabType()
{
    return _prefabType;
}


void Prefab::init()
{
    if (_generatedModel != nullptr)
    {
        delete _generatedModel;
    }

    _generatedModel = generateModel();

    setModel(_generatedModel);
}


// ------------------------------------------------------------------------------------------------
// Cube
// ------------------------------------------------------------------------------------------------


Cube::Cube(float size, Material* material)
    : Prefab(PrefabType::CUBE),
    _size(size), _material(material)
{

}


Cube::~Cube()
{
    delete _material;
}


RStaticModel* Cube::generateModel()
{
    float s = _size / 2.0f;

    Vertex* vertices = new Vertex[24];

    glm::vec3 positions[24] = {
        glm::vec3(s, s, s), glm::vec3(-s, s, s), glm::vec3(-s, -s, s), glm::vec3(s, -s, s),
        glm::vec3(s, s, -s), glm::vec3(-s, s, -s), glm::vec3(-s, -s, -s), glm::vec3(s, -s, -s),
        glm::vec3(s, s, s), glm::vec3(s, -s, s), glm::vec3(s, -s, -s), glm::vec3(s, s, -s),
        glm::vec3(-s, s, s), glm::vec3(-s, -s, s), glm::vec3(-s, -s, -s), glm::vec3(-s, s, -s),
        glm::vec3(s, s, -s), glm::vec3(-s, s, -s), glm::vec3(-s, s, s), glm::vec3(s, s, s),
        glm::vec3(s, -s, -s), glm::vec3(-s, -s, -s), glm::vec3(-s, -s, s), glm::vec3(s, -s, s),
    };

    glm::vec2 texCoords[4] = {
        glm::vec2(1.0f, 1.0f), glm::vec2(0.0f, 1.0f), glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 0.0f)
    };

    glm::vec3 normals[6] = {
        glm::vec3(0.0f, 0.0f, 1.0f),
        glm::vec3(0.0f, 0.0f, -1.0f),
        glm::vec3(1.0f, 0.0f, 0.0f),
        glm::vec3(-1.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 1.0f, 0.0f),
        glm::vec3(0.0f, -1.0f, 0.0f)
    };

    for (int i = 0; i < 24; ++i)
    {
        vertices[i].position = positions[i];
        vertices[i].texCoord = texCoords[i % 4];
        vertices[i].normal = normals[i / 4];
    }


    unsigned int ind[36]{
        0, 1, 2, 0, 2, 3,
        5, 4, 7, 5, 7, 6,
        8, 9, 10, 8, 10, 11,
        15, 14, 13, 15, 13, 12,
        16, 17, 18, 16, 18, 19,
        22, 21, 20, 22, 20, 23
    };

    unsigned int* indices = new unsigned int[36];
    for (int i = 0; i < 36; ++i)
    {
        indices[i] = ind[i];
    }

    bool isGame = GameConfig::getInstance().mode == GM_GAME;

    StaticModelMesh* meshes = new StaticModelMesh[1];
    std::vector<Material*> materials;
    materials.push_back(new Material(*_material));
    meshes[0].setMeshData(vertices, 24, indices, 36, 0, materials[0]->shader, isGame);

    StaticModelNode* modelNode = new StaticModelNode;
    modelNode->name = "object";
    modelNode->meshes = meshes;
    modelNode->meshesCount = 1;
    modelNode->parent = NULL;

    return new RStaticModel("", modelNode, materials);
}


void Cube::setSize(float size)
{
    _size = size;
}


float Cube::getSize()
{
    return _size;
}


// ------------------------------------------------------------------------------------------------
// Plane
// ------------------------------------------------------------------------------------------------


PlanePrefab::PlanePrefab(glm::vec2 size, Material* material)
    : Prefab(PrefabType::PLANE),
    _size(size), _material(material)
{

}


PlanePrefab::~PlanePrefab()
{
    delete _material;
}


RStaticModel* PlanePrefab::generateModel()
{
    glm::vec2 s = _size / 2.0f;

    Vertex* vertices = new Vertex[4];

    glm::vec3 positions[4] = {
        glm::vec3(-s.x, 0, -s.y),
        glm::vec3(-s.x, 0, s.y),
        glm::vec3(s.x, 0, s.y),
        glm::vec3(s.x, 0, -s.y)
    };

    glm::vec2 texCoords[4] = {
        glm::vec2(0.0f, 1.0f),
        glm::vec2(0.0f, 0.0f),
        glm::vec2(1.0f, 0.0f),
        glm::vec2(1.0f, 1.0f)
    };

    for (int i = 0; i < 4; ++i)
    {
        vertices[i].position = positions[i];
        vertices[i].texCoord = texCoords[i];
        vertices[i].normal = glm::vec3(0.0f, 1.0f, 0.0f);
    }


    unsigned int ind[6] = { 0, 1, 2, 0, 2, 3 };

    unsigned int* indices = new unsigned int[6];
    for (int i = 0; i < 6; ++i)
    {
        indices[i] = ind[i];
    }

    bool isGame = GameConfig::getInstance().mode == GM_GAME;

    StaticModelMesh* meshes = new StaticModelMesh[1];
    std::vector<Material*> materials;
    materials.push_back(new Material(*_material));
    meshes[0].setMeshData(vertices, 4, indices, 6, 0, materials[0]->shader, isGame);

    StaticModelNode* modelNode = new StaticModelNode;
    modelNode->name = "object";
    modelNode->meshes = meshes;
    modelNode->meshesCount = 1;
    modelNode->parent = NULL;

    return new RStaticModel("", modelNode, materials);
}


void PlanePrefab::setSize(const glm::vec2& size)
{
    _size = size;
}


const glm::vec2& PlanePrefab::getSize()
{
    return _size;
}


// ------------------------------------------------------------------------------------------------
// Cylinder
// ------------------------------------------------------------------------------------------------


CylinderPrefab::CylinderPrefab(float radius, float height, Material* material)
    : Prefab(PrefabType::CYLINDER),
    _radius(radius), _height(height), _axis(1), _quality(15), _material(material)
{

}


CylinderPrefab::~CylinderPrefab()
{
    delete _material;
}


glm::vec3 getAxisVector(int axis)
{
    if (axis == 0)
    {
        return glm::vec3(1.0f, 0.0f, 0.0f);
    }
    else if (axis == 1)
    {
        return glm::vec3(0.0f, 1.0f, 0.0f);
    }
    else if (axis == 2)
    {
        return glm::vec3(0.0f, 0.0f, 1.0f);
    }

    LOG_ERROR("Invalid axis value");

    return glm::vec3(0.0f, 0.0f, 0.0f);
}


RStaticModel* CylinderPrefab::generateModel()
{
    unsigned int numberOfVertices = _quality * 4 + 2;
    Vertex* vertices = new Vertex[numberOfVertices];

    glm::vec3 axisVector = getAxisVector(_axis);

    for (int i = 0; i < _quality; ++i)
    {
        float t = i / static_cast<float>(_quality);

        float x = _radius * cos(2.0f * PI * t);
        float y = _height * 0.5f;
        float z = _radius * sin(2.0f * PI * t);

        glm::vec3 v1, v2, normal;
        if (_axis == 0)
        {
            v1 = glm::vec3(y, x, z);
            v2 = glm::vec3(-y, x, z);
            normal = glm::normalize(glm::vec3(0.0f, x, z));
        }
        else if (_axis == 1)
        {
            v1 = glm::vec3(x, -y, z);
            v2 = glm::vec3(x, y, z);
            normal = glm::normalize(glm::vec3(x, 0.0f, z));
        }
        else if (_axis == 2)
        {
            v1 = glm::vec3(x, z, y);
            v2 = glm::vec3(x, z, -y);
            normal = glm::normalize(glm::vec3(x, z, 0.0f));
        }

        vertices[i * 2].position = v1;
        vertices[i * 2 + 1].position = v2;

        vertices[i * 2].normal = vertices[i * 2 + 1].normal = normal;

        // circles
        vertices[_quality * 2 + i].position = v1;
        vertices[_quality * 3 + i].position = v2;

        vertices[_quality * 2 + i].normal = -1.0f * axisVector;
        vertices[_quality * 3 + i].normal = 1.0f * axisVector;

        if (_axis != 1)
        {
            vertices[_quality * 2 + i].normal *= -1.0f;
            vertices[_quality * 3 + i].normal *= -1.0f;
        }
    }

    // center of circles
    vertices[numberOfVertices - 2].position = -_height * 0.5f * axisVector;
    vertices[numberOfVertices - 1].position = _height * 0.5f * axisVector;
    vertices[numberOfVertices - 2].normal = -1.0f * axisVector;
    vertices[numberOfVertices - 1].normal = 1.0f * axisVector;

    if (_axis != 1)
    {
        vertices[numberOfVertices - 2].position *= -1.0f;
        vertices[numberOfVertices - 1].position *= -1.0f;
        vertices[numberOfVertices - 2].normal *= -1.0f;
        vertices[numberOfVertices - 1].normal *= -1.0f;
    }

    unsigned int numberOfIndices = (_quality * 4) * 3;
    unsigned int* indices = new unsigned int[numberOfIndices];
    for (int i = 0; i < _quality; ++i)
    {
        indices[i * 6 + 0] = i * 2;
        indices[i * 6 + 1] = i * 2 + 1;
        indices[i * 6 + 2] = (i + 1) % _quality * 2;
        indices[i * 6 + 3] = (i + 1) % _quality * 2;
        indices[i * 6 + 4] = i * 2 + 1;
        indices[i * 6 + 5] = (i + 1) % _quality * 2 + 1;
    }

    // circle 1
    int circleBeginIndex = _quality * 2 * 3;
    for (int i = 0; i < _quality; ++i)
    {
        indices[circleBeginIndex + i * 3 + 0] = numberOfVertices - 2;
        indices[circleBeginIndex + i * 3 + 1] = _quality * 2 + i;
        indices[circleBeginIndex + i * 3 + 2] = _quality * 2 + (i + 1) % _quality;
    }

    // circle 2
    circleBeginIndex = _quality * 3 * 3;
    for (int i = 0; i < _quality; ++i)
    {
        indices[circleBeginIndex + i * 3 + 0] = numberOfVertices - 1;
        indices[circleBeginIndex + i * 3 + 1] = _quality * 3 + (i + 1) % _quality;
        indices[circleBeginIndex + i * 3 + 2] = _quality * 3 + i;
    }

    bool isGame = GameConfig::getInstance().mode == GM_GAME;

    StaticModelMesh* meshes = new StaticModelMesh[1];
    std::vector<Material*> materials;
    materials.push_back(new Material(*_material));
    meshes[0].setMeshData(vertices, numberOfVertices, indices, numberOfIndices, 0, materials[0]->shader, isGame);

    StaticModelNode* modelNode = new StaticModelNode;
    modelNode->name = "object";
    modelNode->meshes = meshes;
    modelNode->meshesCount = 1;
    modelNode->parent = NULL;

    return new RStaticModel("", modelNode, materials);
}


void CylinderPrefab::setRadius(float radius)
{
    _radius = radius;
}


float CylinderPrefab::getRadius()
{
    return _radius;
}


void CylinderPrefab::setHeight(float height)
{
    _height = height;
}


float CylinderPrefab::getHeight()
{
    return _height;
}


void CylinderPrefab::setAxis(int axis)
{
    _axis = axis;
}


int CylinderPrefab::getAxis()
{
    return _axis;
}


void CylinderPrefab::setQuality(int quality)
{
    _quality = quality;
}


int CylinderPrefab::getQuality()
{
    return _quality;
}
