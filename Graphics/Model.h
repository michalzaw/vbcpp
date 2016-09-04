#ifndef MODEL_H_INCLUDED
#define MODEL_H_INCLUDED


#include <string>
#include <vector>

#include <GLEW/glew.h>
#include <glm/glm.hpp>

#include "Shader.h"
#include "VBO.h"
#include "IBO.h"
#include "Vertex.h"
#include "Material.h"
#include "OGLDriver.h"


class Mesh
{
    public:
        Material material;
        unsigned int firstVertex;
        unsigned int quantumOfVertice;

    Mesh() : firstVertex(0), quantumOfVertice(0) { std::cout << "Mesh: Konstruktor\n"; }
    virtual ~Mesh() { std::cout << "Mesh: Destruktor\n"; }
};


class Model
{
    private:
        OGLDriver* _oglDriver;

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

    public:
        //Model(OGLDriver* driver, Vertex* vertices, unsigned int quantumOfVertices, Mesh* meshes, unsigned int quantumOfMeshes, GLenum primitiveType = GL_TRIANGLES);
        Model(OGLDriver* driver, Vertex* vertices, unsigned int quantumOfVertices, unsigned int* indices, unsigned int indicesSize, Mesh* meshes, unsigned int quantumOfMeshes, glm::vec3* collisionMesh = NULL, unsigned int collisionMeshSize = 0, GLenum primitiveType = GL_TRIANGLES);
        ~Model();

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

};


#endif // MODEL_H_INCLUDED
