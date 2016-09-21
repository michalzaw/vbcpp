#ifndef RMODEL_H_INCLUDED
#define RMODEL_H_INCLUDED

#include "Resource.h"
#include "../Graphics/Model.h"
#include "../Graphics/OGLDriver.h"

class RModel : virtual public Resource
{
    public:
        RModel(std::string path, Model* m);
        virtual ~RModel();

        VBO* GetVBO();
        IBO* GetIBO();
        unsigned int GetQuantumOfVertices();
        Vertex* GetVertices();
        unsigned int GetIndicesSize();
        unsigned int* GetIndices();
        unsigned int GetQuantumOfMeshes();
        Mesh* GetMesh(unsigned int i);
        unsigned int GetCollisionMeshSize();
        glm::vec3* GetCollisionMesh();
        GLenum GetPrimitiveType();
        Mesh* getMeshes();

    private:
        Vertex* _vertices;
        unsigned int _quantumOfVertices;

        unsigned int* _indices;
        unsigned int _indicesSize;

        Mesh* _meshes;
        unsigned int _quantumOfMeshes;

        glm::vec3* _collisionMesh;
        unsigned int _collisionMeshSize;

        VBO* _vbo;
        IBO* _ibo;

        GLenum _primitiveType;
};


#endif // RMODEL_H_INCLUDED
