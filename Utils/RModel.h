#ifndef RMODEL_H_INCLUDED
#define RMODEL_H_INCLUDED

#include "Resource.h"
#include "../Graphics/Model.h"
#include "../Graphics/OGLDriver.h"
#include "../Graphics/AABB.h"


class RModel : virtual public Resource
{
    public:
        RModel(std::string path, Model* m);
        virtual ~RModel();

        VBO* getVBO();
        IBO* getIBO();
        unsigned int getQuantumOfVertices();
        Vertex* getVertices();
        unsigned int getIndicesSize();
        unsigned int* getIndices();
        unsigned int getQuantumOfMeshes();
        Mesh* getMesh(unsigned int i);
        unsigned int getCollisionMeshSize();
        glm::vec3* getCollisionMesh();
        GLenum getPrimitiveType();
        Mesh* getMeshes();
        AABB* getAABB();
        VBO* getAabbVbo()
        {
            return _aabbVbo;
        }
        IBO* getAabbIbo()
        {
            return _aabbIbo;
        }

    protected:
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

        AABB _aabb;

        VBO* _aabbVbo;
        IBO* _aabbIbo;

        void calculateAABB();
        void createAabbVbo();
};


#endif // RMODEL_H_INCLUDED
