#ifndef PREFAB_H_INCLUDED
#define PREFAB_H_INCLUDED


#include "RenderObject.h"


enum PrefabType
{
    PT_CUBE

};


class Prefab : public RenderObject
{
    protected:
        virtual void generateModel() = 0;

    public:
        Prefab()
        {

        }

        ~Prefab()
        {
            delete _model;
        }

        void init()
        {
            generateModel();
        }
};


class Cube : public Prefab
{
    protected:
        float _size;
        Material _material;

        virtual void generateModel()
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
                vertices[i].normal = normals[i / 5];
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


            StaticModelMesh* meshes = new StaticModelMesh[1];
            Material* materials = new Material[1];
            materials[0] = _material;
            meshes[0].setMeshData(vertices, 24, indices, 36, 0, materials[0].shader);

            StaticModelNode* modelNode = new StaticModelNode;
            modelNode->name = "object";
            modelNode->transformMatrix = glm::mat4(1.0f);
            modelNode->meshes = meshes;
            modelNode->meshesCount = 1;
            modelNode->parent = NULL;
            modelNode->childrenCount = 0;
            modelNode->children = NULL;

            setModel(new RStaticModel("", modelNode, materials, 1));
        }

    public:
        Cube(float size, Material material)
            : _size(size), _material(material)
        {

        }
};


#endif // PREFAB_H_INCLUDED
