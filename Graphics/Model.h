#ifndef MODEL_H_INCLUDED
#define MODEL_H_INCLUDED


#include <string>
#include <vector>

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "RShader.h"
#include "VBO.h"
#include "IBO.h"
#include "Vertex.h"
#include "Material.h"
#include "OGLDriver.h"

#include "../Utils/Resource.h"

class Mesh
{
    public:
        Material material;
        unsigned int firstVertex;
        unsigned int quantumOfVertice;

    Mesh() : firstVertex(0), quantumOfVertice(0) { std::cout << "Mesh: Konstruktor\n"; }
    virtual ~Mesh() { std::cout << "Mesh: Destruktor\n"; }
};


class Model //: virtual public Resource
{
    private:
        Vertex*         _vertices;
        unsigned int    _quantumOfVertices;

        unsigned int*   _indices;
        unsigned int    _indicesSize;

        Mesh*           _meshes;
        unsigned int    _quantumOfMeshes;

        glm::vec3*      _collisionMesh;
        unsigned int    _collisionMeshSize;

        VBO* _vbo;
        IBO* _ibo;

        GLenum _primitiveType;

    public:
        //Model(OGLDriver* driver, Vertex* vertices, unsigned int quantumOfVertices, Mesh* meshes, unsigned int quantumOfMeshes, GLenum primitiveType = GL_TRIANGLES);
        Model(Vertex* vertices, unsigned int quantumOfVertices, unsigned int* indices, unsigned int indicesSize, Mesh* meshes, unsigned int quantumOfMeshes, glm::vec3* collisionMesh = NULL, unsigned int collisionMeshSize = 0, GLenum primitiveType = GL_TRIANGLES);
        ~Model();

        VBO*            getVBO();
        IBO*            getIBO();
        unsigned int    getQuantumOfVertices();
        Vertex*         getVertices();
        unsigned int    getIndicesSize();
        unsigned int*   getIndices();
        unsigned int    getQuantumOfMeshes();
        Mesh*           getMesh(unsigned int i);
        unsigned int    getCollisionMeshSize();
        glm::vec3*      getCollisionMesh();
        GLenum          getPrimitiveType();
        Mesh*           getMeshes();

};


#endif // MODEL_H_INCLUDED
