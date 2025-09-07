#ifndef MODELNODE_H_INCLUDED
#define MODELNODE_H_INCLUDED


#include "Transform.h"

#include "../Scripting/Utils/LuaMacros.h"

#include "../Utils/RStaticModel.h"


class RenderObject;


struct ModelNodeMesh
{
    VBO* vbo;
    IBO* ibo;

    unsigned int firstVertex;
    unsigned int firstVertexInVbo;
    unsigned int indicesCount;

    unsigned int vertexSize;

    Material* material;

    ModelNodeMesh(VBO* vbo, IBO* ibo, unsigned int firstVertex, unsigned int firstVertexInVbo, unsigned int indicesCount, unsigned int vertexSize, Material* material)
        : vbo(vbo), ibo(ibo), firstVertex(firstVertex), firstVertexInVbo(firstVertexInVbo), indicesCount(indicesCount), vertexSize(vertexSize), material(material)
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

        LUAF std::string getName();
		AABB* getAABB();
        LUAF Transform& getTransformNode();
        LUAF Transform& getTransform();
        glm::mat4& getTransformMatrix();
        glm::mat4& getNormalMatrix();

        LUAF ModelNodeMesh* getMesh(unsigned int i);
        LUAF unsigned int getMeshesCount();

        LUAF ModelNode* getParent();
        LUAF std::vector<ModelNode*>& getChildren();
        LUAF unsigned int getChildrenCount();

        void replaceMaterialsByName(const std::vector<Material*>& materials);

};


#endif // MODELNODE_H_INCLUDED
