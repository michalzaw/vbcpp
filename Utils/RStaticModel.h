#ifndef RSTATICMODEL_H_INCLUDED
#define RSTATICMODEL_H_INCLUDED


#include "../Graphics/OGLDriver.h"
#include "../Graphics/Vertex.h"
#include "../Graphics/Material.h"
#include "../Graphics/VBO.h"
#include "../Graphics/IBO.h"
#include "../Graphics/AABB.h"
#include "../Graphics/Transform.h"


struct StaticModelMesh
{
    Vertex*         vertices;
    unsigned int    verticesCount;

    unsigned int*   indices;
    unsigned int    indicesCount;

    VBO* vbo;
    IBO* ibo;

    unsigned int firstVertex;
    unsigned int firstVertexInVbo;
    unsigned int materialIndex;

	bool useSharedVboAndIbo;

    ~StaticModelMesh()
    {
        if (vertices)
        {
            delete[] vertices;
        }

        if (indices)
        {
            delete[] indices;
        }

		if (!useSharedVboAndIbo)
		{
			if (vbo)
			{
				OGLDriver::getInstance().deleteVBO(vbo);
			}

			if (ibo)
			{
				OGLDriver::getInstance().deleteIBO(ibo);
			}
		}

    }

    void setMeshData(Vertex* vertices, unsigned int verticesCount, unsigned int* indices, unsigned int indicesCount,
                     unsigned int materialIndex, ShaderType shaderType,
					 bool useSharedVboAndIbo = true, unsigned int vboSize = 0, unsigned int iboSize = 0, bool staticData = true)
    {
        this->vertices = vertices;
        this->verticesCount = verticesCount;
        this->indices = indices;
        this->indicesCount = indicesCount;
        this->firstVertex = 0;
		this->firstVertexInVbo = 0;
        this->materialIndex = materialIndex;
		this->useSharedVboAndIbo = useSharedVboAndIbo;

		if (useSharedVboAndIbo)
		{
			int vboIndex = OGLDriver::getInstance().getVboIndexForVertices(shaderType, verticesCount, sizeof(vertices[0]));
			int iboIndex = OGLDriver::getInstance().getIboIndexForIndices(shaderType, indicesCount);
			vbo = OGLDriver::getInstance().vbos[shaderType][vboIndex];// createVBO(verticesCount * sizeof(Vertex));
			ibo = OGLDriver::getInstance().ibos[shaderType][iboIndex];//createIBO(indicesCount * sizeof(unsigned int));

			firstVertexInVbo = vbo->getQuantumOfVertices();
			for (unsigned int i = 0; i < this->indicesCount; ++i)
			{
				this->indices[i] += firstVertexInVbo;
			}

			this->firstVertex = ibo->getIndicesCount();
		}
		else
		{
			GLenum usage = staticData ? GL_STATIC_DRAW : GL_DYNAMIC_DRAW;
			vbo = OGLDriver::getInstance().createVBO(vboSize != 0 ? vboSize : verticesCount * sizeof(Vertex), usage);
			ibo = OGLDriver::getInstance().createIBO(iboSize != 0 ? iboSize : indicesCount * sizeof(unsigned int), usage);
		}

        vbo->addVertexData(vertices, verticesCount);
        ibo->addIndices(indices, indicesCount);
    }

	void updateMeshData(Vertex* vertices, unsigned int verticesCount, unsigned int* indices, unsigned int indicesCount)
	{
		if (vertices)
		{
			delete[] this->vertices;
		}

		if (indices)
		{
			delete[] this->indices;
		}

		this->vertices = vertices;
		this->verticesCount = verticesCount;
		this->indices = indices;
		this->indicesCount = indicesCount;

		firstVertexInVbo = 0;
		firstVertex = 0;

		vbo->updateVertexData(vertices, verticesCount);
		ibo->updateIndices(indices, indicesCount);
	}

};

// Przebudowac na klase
struct StaticModelNode
{
    std::string name;

    Transform transform;

    StaticModelMesh*    meshes;
    unsigned int        meshesCount;

    StaticModelNode*    parent;
    std::vector<StaticModelNode*>   children;

    AABB aabb;

    StaticModelNode()
    {
        meshes = NULL;
        meshesCount = 0;

        parent = NULL;
    }

    ~StaticModelNode()
    {
        if (meshes)
        {
            delete[] meshes;
        }

        for (int i = 0; i < children.size(); ++i)
        {
            delete children[i];
        }
    }


    unsigned int    getMeshesCount()
    {
        return meshesCount;
    }
    StaticModelMesh*getMesh(unsigned int i)
    {
        if (i < meshesCount)
            return &meshes[i];

        return NULL;
    }

};


class RStaticModel : public Resource
{
    private:
        StaticModelNode*_rootNode;


        Material*       _materials;
        unsigned int    _materialsCount;

        glm::vec3*      _collisionMesh;
        unsigned int    _collisionMeshSize;

        GLenum          _primitiveType;

        AABB _aabb;

        void findMinAndMaxVertices(StaticModelNode* node, glm::mat4 parentTransform, glm::vec3& min, glm::vec3& max);
        void calculateAABB();

    public:
        RStaticModel(string path, StaticModelNode* rootNode, Material* materials, unsigned int materialsCount,
                     GLenum primitiveType = GL_TRIANGLES, glm::vec3* collisionMesh = NULL, unsigned int collisionMeshSize = 0);
        RStaticModel()
            : Resource(RT_MODEL, "")
        {
            _rootNode = NULL;

            _collisionMeshSize = 0;
        }
        ~RStaticModel();

        StaticModelNode* getRootNode();

        Material*       getMaterial(unsigned int i);
        unsigned int    getMaterialsCount();

        unsigned int    getCollisionMeshSize();
        glm::vec3*      getCollisionMesh();

        GLenum getPrimitiveType() {return _primitiveType;}

        AABB* getAABB();

		void setNewCollisionMesh(glm::vec3* collisionMesh = NULL, unsigned int collisionMeshSize = 0);
		void recalculateAABB();

};



#endif // RSTATICMODEL_H_INCLUDED
