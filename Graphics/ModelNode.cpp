#include "ModelNode.h"

#include "GraphicsManager.h"
#include "MirrorComponent.h"

#include "../Utils/Helpers.hpp"


ModelNode::ModelNode(RStaticModel* staticModel, StaticModelNode* staticModelNode, const std::vector<std::string>& nodesToSkip, RenderObject* renderObject, ModelNode* parent)
{
    _name = staticModelNode->name;
	_aabb = &(staticModelNode->aabb);
    _transformNode = staticModelNode->transform;

    for (int i = 0; i < staticModelNode->meshesCount; ++i)
    {
        const StaticModelMesh& mesh = staticModelNode->meshes[i];

        Material* material = staticModel->getMaterial(mesh.materialIndex);
        if (material->requireSeparateInstance)
        {
            staticModel->getMaterials().push_back(new Material(*material));
            material = staticModel->getMaterial(staticModel->getMaterialsCount() - 1);

            if (material->shader == MIRROR_MATERIAL)
            {
                renderObject->addMirrorMaterial(material);
            }
        }
        _meshes.push_back(new ModelNodeMesh(mesh.vbo, mesh.ibo, mesh.firstVertex, mesh.firstVertexInVbo, mesh.indicesCount, material));
    }

    _parent = parent;

    for (int i = 0; i < staticModelNode->children.size(); ++i)
    {
        if (!isVectorContains(nodesToSkip, staticModelNode->children[i]->name))
        {
            _children.push_back(new ModelNode(staticModel, staticModelNode->children[i], nodesToSkip, renderObject, this));
        }
    }
}


ModelNode::~ModelNode()
{
    for (int i = 0; i < _meshes.size(); ++i)
    {
        delete _meshes[i];
    }

    for (int i = 0; i < _children.size(); ++i)
    {
        delete _children[i];
    }
}


std::string ModelNode::getName()
{
    return _name;
}


AABB* ModelNode::getAABB()
{
	return _aabb;
}


Transform& ModelNode::getTransformNode()
{
    return _transformNode;
}


Transform& ModelNode::getTransform()
{
    return _transform;
}


glm::mat4& ModelNode::getTransformMatrix()
{
    _transformMatrix = _transformNode.getTransformMatrix() * _transform.getTransformMatrix();
    return _transformMatrix;
}


glm::mat4& ModelNode::getNormalMatrix()
{
    _normalMatrix = _transformNode.getNormalMatrix() * _transform.getNormalMatrix();
    return _normalMatrix;
}


ModelNodeMesh* ModelNode::getMesh(unsigned int i)
{
    return _meshes[i];
}


unsigned int ModelNode::getMeshesCount()
{
    return _meshes.size();
}


ModelNode* ModelNode::getParent()
{
    return _parent;
}


std::vector<ModelNode*>& ModelNode::getChildren()
{
    return _children;
}


unsigned int ModelNode::getChildrenCount()
{
    return _children.size();
}


void ModelNode::replaceMaterialsByName(const std::vector<Material*>& materials)
{
    for (ModelNodeMesh* mesh : _meshes)
    {
        for (Material* material : materials)
        {
            if (mesh->material->name == material->name)
            {
                mesh->material = material;
            }
        }
    }

    for (ModelNode* child : _children)
    {
        child->replaceMaterialsByName(materials);
    }
}
