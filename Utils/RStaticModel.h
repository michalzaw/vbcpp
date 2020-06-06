#ifndef RSTATICMODEL_H_INCLUDED
#define RSTATICMODEL_H_INCLUDED


#include "../Graphics/OGLDriver.h"
#include "../Graphics/Vertex.h"
#include "../Graphics/Material.h"
#include "../Graphics/VBO.h"
#include "../Graphics/IBO.h"
#include "../Graphics/AABB.h"
#include "../Graphics/Transform.h"


struct StaticModelMesh
{
    Vertex*         vertices;
    unsigned int    verticesCount;

    unsigned int*   indices;
    unsigned int    indicesCount;

    VBO* vbo;
    IBO* ibo;

    unsigned int firstVertex;
    unsigned int firstVertexInVbo;
    unsigned int materialIndex;

    ~StaticModelMesh()
    {
        if (vertices)
        {
            delete[] vertices;
        }

        if (indices)
        {
            delete[] indices;
        }

        if (vbo)
        {
            //OGLDriver::getInstance().deleteVBO(vbo);
        }

        if (ibo)
        {
            //OGLDriver::getInstance().deleteIBO(ibo);
        }

    }

    void setMeshData(Vertex* vertices, unsigned int verticesCount, unsigned int* indices, unsigned int indicesCount,
                     unsigned int materialIndex, ShaderType shaderType)
    {
        this->vertices = vertices;
        this->verticesCount = verticesCount;
        this->indices = indices;
        this->indicesCount = indicesCount;
        this->firstVertex = 0;
        this->materialIndex = materialIndex;

        int vboIndex = OGLDriver::getInstance().getVboIndexForVertices(shaderType, verticesCount, sizeof(vertices[0]));
        int iboIndex = OGLDriver::getInstance().getIboIndexForIndices(shaderType, indicesCount);
        vbo = OGLDriver::getInstance().vbos[shaderType][vboIndex];// createVBO(verticesCount * sizeof(Vertex));
        ibo = OGLDriver::getInstance().ibos[shaderType][iboIndex];//createIBO(indicesCount * sizeof(unsigned int));

        firstVertexInVbo = vbo->getQuantumOfVertices();
        for (unsigned int i = 0; i < this->indicesCount; ++i)
        {
            this->indices[i] += firstVertexInVbo;
        }

        this->firstVertex = ibo->getIndicesCount();

        vbo->addVertexData(vertices, verticesCount);
        ibo->addIndices(indices, indicesCount);
    }

};

// Przebudowac na klase
struct StaticModelNode
{
    std::string name;

    Transform transform;

    StaticModelMesh*    meshes;
    unsigned int        meshesCount;

    StaticModelNode*    parent;
    std::vector<StaticModelNode*>   children;

    AABB aabb;

    StaticModelNode()
    {
        meshes = NULL;
        meshesCount = 0;

        parent = NULL;
    }

    ~StaticModelNode()
    {
        if (meshes)
        {
            delete[] meshes;
        }

        for (int i = 0; i < children.size(); ++i)
        {
            delete children[i];
        }
    }


    unsigned int    getMeshesCount()
    {
        return meshesCount;
    }
    StaticModelMesh*getMesh(unsigned int i)
    {
        if (i < meshesCount)
            return &meshes[i];

        return NULL;
    }

};


class RStaticModel : public Resource
{
    private:
        StaticModelNode*_rootNode;


        Material*       _materials;
        unsigned int    _materialsCount;

        glm::vec3*      _collisionMesh;
        unsigned int    _collisionMeshSize;

        GLenum          _primitiveType;

        AABB _aabb;

        void findMinAndMaxVertices(StaticModelNode* node, glm::mat4 parentTransform, glm::vec3& min, glm::vec3& max);
        void calculateAABB();

    public:
        RStaticModel(string path, StaticModelNode* rootNode, Material* materials, unsigned int materialsCount,
                     GLenum primitiveType = GL_TRIANGLES, glm::vec3* collisionMesh = NULL, unsigned int collisionMeshSize = 0);
        RStaticModel()
            : Resource(RT_MODEL, "")
        {
            _rootNode = NULL;

            _collisionMeshSize = 0;
        }
        ~RStaticModel();

        StaticModelNode* getRootNode();

        Material*       getMaterial(unsigned int i);
        unsigned int    getMaterialsCount();

        unsigned int    getCollisionMeshSize();
        glm::vec3*      getCollisionMesh();

        GLenum getPrimitiveType() {return _primitiveType;}

        AABB* getAABB();

};



#endif // RSTATICMODEL_H_INCLUDED
