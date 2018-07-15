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


btTriangleMesh* PhysicalBodyBvtTriangleMesh::buildTriangleMesh()
{
    btTriangleMesh* triMesh = new btTriangleMesh(true, false);
    btVector3 tmp_vertices[3];
    unsigned int index, vertexCount;

    if (_model)
    {
        for (int i = 0; i < _model->getMeshesCount(); ++i)
        {
            StaticModelMesh* mesh = _model->getMesh(i);

            unsigned int* indices = mesh->indices;
            Vertex* vertices = mesh->vertices;

            vertexCount = mesh->verticesCount;

            for (unsigned int j = 0; j < mesh->indicesCount; j += 3)
            {
                for (unsigned int k = 0; k < 3; k++)
                {
                    index = indices[j+k];

                    if (index > vertexCount) continue;

                    tmp_vertices[k] = btVector3(vertices[index].position[0], vertices[index].position[1], vertices[index].position[2]);
                }

                triMesh->addTriangle(tmp_vertices[0], tmp_vertices[1], tmp_vertices[3]);
            }
        }
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
