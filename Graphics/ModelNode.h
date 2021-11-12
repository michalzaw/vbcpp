#ifndef MODELNODE_H_INCLUDED
#define MODELNODE_H_INCLUDED


#include "Transform.h"

#include "../Utils/RStaticModel.h"


class RenderObject;


struct ModelNodeMesh
{
    VBO* vbo;
    IBO* ibo;

    unsigned int firstVertex;
    unsigned int firstVertexInVbo;
    unsigned int indicesCount;

    Material* material;

    ModelNodeMesh(VBO* vbo, IBO* ibo, unsigned int firstVertex, unsigned int firstVertexInVbo, unsigned int indicesCount, Material* material)
        : vbo(vbo), ibo(ibo), firstVertex(firstVertex), firstVertexInVbo(firstVertexInVbo), indicesCount(indicesCount), material(material)
    {}

};


class ModelNode
{
    private:
        std::string _name;

		AABB* _aabb;

        Transform _transformNode;
        Transform _transform;
        glm::mat4 _transformMatrix;
        glm::mat4 _normalMatrix;

        std::vector<ModelNodeMesh*> _meshes;

        ModelNode*          _parent;
        std::vector<ModelNode*> _children;

    public:
        ModelNode(RStaticModel* staticModel, StaticModelNode* staticModelNode, const std::vector<std::string>& nodesToSkip, RenderObject* renderObject, ModelNode* parent = NULL);
        ~ModelNode();

        std::string getName();
		AABB* getAABB();
        Transform& getTransformNode();
        Transform& getTransform();
        glm::mat4& getTransformMatrix();
        glm::mat4& getNormalMatrix();

        ModelNodeMesh* getMesh(unsigned int i);
        unsigned int getMeshesCount();

        ModelNode* getParent();
        std::vector<ModelNode*>& getChildren();
        unsigned int getChildrenCount();

        void replaceMaterialsByName(const std::vector<Material*>& materials);

};


#endif // MODELNODE_H_INCLUDED
