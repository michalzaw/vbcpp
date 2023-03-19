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
    unsigned char*  vertices;
    unsigned int    verticesCount;
    unsigned int    vertexSize;
    VertexType      vertexType;

    unsigned int*   indices;
    unsigned int    indicesCount;

    VBO* vbo;
    IBO* ibo;

    unsigned int firstVertex;
    unsigned int firstVertexInVbo;
    unsigned int materialIndex;

	bool useSharedVboAndIbo;

    StaticModelMesh()
        : vertices(nullptr), verticesCount(0),
        indices(nullptr), indicesCount(0),
        vbo(nullptr), ibo(nullptr),
        firstVertex(0), firstVertexInVbo(0), materialIndex(0),
        useSharedVboAndIbo(false)
    {

    }

    ~StaticModelMesh()
    {
        if (vertices)
        {
            if (vertexType == VertexType::STANDARD)
            {
                delete[](reinterpret_cast<Vertex*>(this->vertices));
            }
            else if (vertexType == VertexType::ANIMATED)
            {
                delete[](reinterpret_cast<AnimatedVertex*>(this->vertices));
            }
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

    // shaderType, vboSize, iboSize, staticData - only used when useSharedVboAndIbo is false
    template <typename VertType>
    void setMeshData(VertType* vertices, unsigned int verticesCount, unsigned int* indices, unsigned int indicesCount,
                     unsigned int materialIndex, ShaderType shaderType,
					 bool useSharedVboAndIbo = true, unsigned int vboSize = 0, unsigned int iboSize = 0, bool staticData = true)
    {
        bool isInitialized = vbo != nullptr || ibo != nullptr;

        if (vertices)
        {
            if (vertexType == VertexType::STANDARD)
            {
                delete[](reinterpret_cast<Vertex*>(this->vertices));
            }
            else if (vertexType == VertexType::ANIMATED)
            {
                delete[](reinterpret_cast<AnimatedVertex*>(this->vertices));
            }
        }
        if (indices)
        {
            delete[] this->indices;
        }

        this->vertexSize = sizeof(VertType);
        this->vertexType = VerticesUtils::getVertexType<VertType>();
        this->verticesCount = verticesCount;
        //this->vertices = new unsigned char[this->vertexSize * this->verticesCount];
        //memccpy(this->vertices, vertices, this->vertexSize * this->verticesCount);
        this->vertices = reinterpret_cast<unsigned char*>(vertices);

        if (this->vertexType == VertexType::VERTEX_TYPES_COUNT)
        {
            LOG_ERROR("Invalid vertex type!");
            return;
        }

        this->indices = indices;
        this->indicesCount = indicesCount;
        this->firstVertex = 0;
		this->firstVertexInVbo = 0;
        this->materialIndex = materialIndex;
		this->useSharedVboAndIbo = useSharedVboAndIbo;

        if (!isInitialized)
        {
            if (useSharedVboAndIbo)
            {
                int vboIndex = OGLDriver::getInstance().getVboIndexForVertices(shaderType, verticesCount, this->vertexSize);
                int iboIndex = OGLDriver::getInstance().getIboIndexForIndices(shaderType, indicesCount);
                vbo = OGLDriver::getInstance().vbos[shaderType][vboIndex];// createVBO(verticesCount * sizeof(Vertex));
                ibo = OGLDriver::getInstance().ibos[shaderType][iboIndex];//createIBO(indicesCount * sizeof(unsigned int));

                firstVertexInVbo = vbo->getQuantumOfVertices();
                for (unsigned int i = 0; i < this->indicesCount; ++i)
                {
                    if (this->indices[i] != OGLDriver::PRIMITIVE_RESTART_INDEX)
                    {
                        this->indices[i] += firstVertexInVbo;
                    }
                }

                this->firstVertex = ibo->getIndicesCount();
            }
            else
            {
                GLenum usage = staticData ? GL_STATIC_DRAW : GL_DYNAMIC_DRAW;
                vbo = OGLDriver::getInstance().createVBO(vboSize != 0 ? vboSize : verticesCount * this->vertexSize, usage);
                ibo = OGLDriver::getInstance().createIBO(iboSize != 0 ? iboSize : indicesCount * sizeof(unsigned int), usage);
            }

            int result = vbo->addVertexData(vertices, verticesCount);
            if (result < 0)
            {
                LOG_ERROR("Failed to add vertices to VBO");
            }
            result = ibo->addIndices(indices, indicesCount);
            if (result < 0)
            {
                LOG_ERROR("Failed to add indices to IBO");
            }
        }
        else
        {
            LOG_DEBUG("Update data in existing VBO and IBO");
            int result = vbo->updateVertexData(vertices, verticesCount);
            if (result < 0)
            {
                LOG_ERROR("Failed to add vertices to VBO");
            }
            result = ibo->updateIndices(indices, indicesCount);
            if (result < 0)
            {
                LOG_ERROR("Failed to add indices to IBO");
            }
        }
    }

    template <typename VertType>
    VertType* getVertices()
    {
        return reinterpret_cast<VertType*>(vertices);
    }
    

	/*void updateMeshData(Vertex* vertices, unsigned int verticesCount, unsigned int* indices, unsigned int indicesCount)
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
        this->firstVertex = 0;
		this->firstVertexInVbo = 0;

		vbo->updateVertexData(vertices, verticesCount);
		ibo->updateIndices(indices, indicesCount);
	}*/

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


    glm::mat4 getGlobalTransformMatrix()
    {
        if (parent != nullptr)
        {
            return parent->getGlobalTransformMatrix() * transform.getTransformMatrix();
        }

        return transform.getTransformMatrix();
    }

    unsigned int    getMeshesCount()
    {
        return meshesCount;
    }
    StaticModelMesh* getMesh(unsigned int i)
    {
        if (i < meshesCount)
            return &meshes[i];

        return NULL;
    }

    StaticModelNode* getNodeByName(const std::string& name)
    {
        for (StaticModelNode* child : children)
        {
            if (child->name == name)
            {
                return child;
            }
            else
            {
                StaticModelNode* node = child->getNodeByName(name);
                if (node != nullptr)
                {
                    return node;
                }
            }
        }

        return nullptr;
    }

    void getVerticesArray(std::vector<glm::vec3>& vertices, bool transformToGlobalSpace = false)
    {
        for (int i = 0; i < meshesCount; ++i)
        {
            if (meshes[i].vertexType == VertexType::STANDARD)
            {
                VerticesUtils::getVerticesPositionsArray<Vertex>(meshes[i].vertices, meshes[i].verticesCount, vertices);
            }
            else if (meshes[i].vertexType == VertexType::ANIMATED)
            {
                VerticesUtils::getVerticesPositionsArray<AnimatedVertex>(meshes[i].vertices, meshes[i].verticesCount, vertices);
            }
        }

        if (transformToGlobalSpace)
        {
            glm::mat4 transfromMatrix = getGlobalTransformMatrix();
            for (int i = 0; i < vertices.size(); ++i)
            {
                vertices[i] = glm::vec3(transfromMatrix * glm::vec4(vertices[i], 1.0f));
            }
        }
    }
};


class RStaticModel : public Resource
{
    private:
        StaticModelNode* _rootNode;

        std::vector<Material*> _materials;

        glm::vec3*      _collisionMesh;
        unsigned int    _collisionMeshSize;

        GLenum          _primitiveType;

        float           _aabbScaleFactor;
        AABB            _aabb;

        void findMinAndMaxVertices(StaticModelNode* node, glm::mat4 parentTransform, glm::vec3& min, glm::vec3& max);
        virtual void calculateAABB();

    public:
        RStaticModel(const std::string& path, StaticModelNode* rootNode, const std::vector<Material*>& materials,
                     GLenum primitiveType = GL_TRIANGLES, glm::vec3* collisionMesh = NULL, unsigned int collisionMeshSize = 0, float aabbScaleFactor = 1.0f);
        RStaticModel()
            : Resource(RT_MODEL, "")
        {
            _rootNode = NULL;

            _collisionMeshSize = 0;
        }
        virtual ~RStaticModel();

        StaticModelNode* getRootNode();
        StaticModelNode* getNodeByName(const std::string& name);

        std::vector<Material*>& getMaterials();
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
