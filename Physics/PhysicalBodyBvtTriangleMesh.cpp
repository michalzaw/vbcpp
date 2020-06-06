#include "PhysicalBodyBvtTriangleMesh.hpp"

PhysicalBodyBvtTriangleMesh::PhysicalBodyBvtTriangleMesh(RStaticModel* model)
: PhysicalBody(0),
_model(model)
{
    printf("BvtTriangleMeshShape - Konstruktor\n");
    updateBody();
}


PhysicalBodyBvtTriangleMesh::~PhysicalBodyBvtTriangleMesh()
{
    printf("BvtTriangleMeshShape - Destruktor\n");
}


void PhysicalBodyBvtTriangleMesh::addModelNodeToTriangleMesh(btTriangleMesh* triMesh, StaticModelNode* staticModelNode, glm::mat4 parentTransform)
{
    glm::mat4 nodeTransform = parentTransform * staticModelNode->transform.getTransformMatrix();

    for (int i = 0; i < staticModelNode->getMeshesCount(); ++i)
    {
        StaticModelMesh* mesh = staticModelNode->getMesh(i);

        unsigned int* indices = mesh->indices;
        Vertex* vertices = mesh->vertices;

        unsigned int vertexCount = mesh->verticesCount;

        for (unsigned int j = 0; j < mesh->indicesCount; j += 3)
        {
            btVector3 tmp_vertices[3];

            for (unsigned int k = 0; k < 3; k++)
            {
                unsigned index = indices[j+k] - mesh->firstVertexInVbo;

                if (index > vertexCount) continue;

                glm::vec4 v = nodeTransform * glm::vec4(vertices[index].position.x, vertices[index].position.y, vertices[index].position.z, 1.0f);
                tmp_vertices[k] = btVector3(v[0], v[1], v[2]);
            }

            triMesh->addTriangle(tmp_vertices[0], tmp_vertices[1], tmp_vertices[2]);
        }
    }

    for (int i = 0; i < staticModelNode->children.size(); ++i)
    {
        addModelNodeToTriangleMesh(triMesh, staticModelNode->children[i], nodeTransform);
    }
}


btTriangleMesh* PhysicalBodyBvtTriangleMesh::buildTriangleMesh()
{
    btTriangleMesh* triMesh = new btTriangleMesh(true, false);

    if (_model)
    {
        addModelNodeToTriangleMesh(triMesh, _model->getRootNode(), glm::mat4(1.0f));
    }

    if (!triMesh)
        return 0;
    else
        return triMesh;
}

void PhysicalBodyBvtTriangleMesh::updateBody()
{
    _collShape.reset( new btBvhTriangleMeshShape( buildTriangleMesh(), true, true) );

    btVector3 inertia(0, 0, 0);

    // BVT Triangle Mesh is for big static meshes, so we set mass to zero
    _collShape->calculateLocalInertia(0, inertia);

    _motionState.reset( new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), _position)) );

    btRigidBody::btRigidBodyConstructionInfo rigidBodyCI(_mass, _motionState.get(), _collShape.get(), inertia);
    _rigidBody.reset( new btRigidBody(rigidBodyCI) );
}
