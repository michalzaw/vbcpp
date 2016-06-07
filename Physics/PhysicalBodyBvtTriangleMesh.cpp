#include "PhysicalBodyBvtTriangleMesh.hpp"

PhysicalBodyBvtTriangleMesh::PhysicalBodyBvtTriangleMesh(Model* model, btVector3 pos)
: PhysicalBody(0, pos),
_model(model)
{
    printf("BvtTrisngleMeshShape - Konstruktor\n");
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
        unsigned int* indices = _model->GetIndices();
        Vertex* vertices = _model->GetVertices();

        vertexCount = _model->GetQuantumOfVertices();

        for (unsigned int j = 0; j < _model->GetIndicesSize(); j += 3)
        {
            for (unsigned int k = 0; k < 3; k++)
            {
                index = indices[j+k];

                if (index > vertexCount) continue;

                tmp_vertices[k] = btVector3(vertices[index].Position[0], vertices[index].Position[1], vertices[index].Position[2]);
            }

            triMesh->addTriangle(tmp_vertices[0], tmp_vertices[1], tmp_vertices[3]);
        }
    }

    if (!triMesh)
        return 0;
    else
        return triMesh;
}

void PhysicalBodyBvtTriangleMesh::updateBody()
{
    safe_delete<btRigidBody>(_rigidBody);
    safe_delete<btCollisionShape>(_collShape);
    safe_delete<btDefaultMotionState>(_motionState);

    _collShape = new btBvhTriangleMeshShape( buildTriangleMesh(), true, true);

    btVector3 inertia(0, 0, 0);

    // BVT Triangle Mesh is for big static meshes, so we set mass to zero
    _collShape->calculateLocalInertia(0, inertia);

    _motionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), _position));

    btRigidBody::btRigidBodyConstructionInfo rigidBodyCI(_mass, _motionState, _collShape, inertia);
    _rigidBody = new btRigidBody(rigidBodyCI);
}
