#include "ShapePolygonComponent.h"

#include "GraphicsManager.h"
#include "ModelGenerator.h"
#include "RenderObject.h"

#include "../Scene/SceneManager.h"

#include "../Utils/PolygonGenerator.h"
#include "../Utils/ResourceManager.h"


ShapePolygonComponent::ShapePolygonComponent()
	: Component(CT_SHAPE_POLYGON),
	_generatedModel(nullptr)
{

}

ShapePolygonComponent::ShapePolygonComponent(std::vector<glm::vec3>& points)
	: Component(CT_SHAPE_POLYGON),
	_points(points),
	_generatedModel(nullptr)
{

}


ShapePolygonComponent::~ShapePolygonComponent()
{
	Component* currentRenderComponent = getSceneObject()->getComponent(CT_RENDER_OBJECT);
	if (currentRenderComponent != nullptr)
	{
		getSceneObject()->removeComponent(currentRenderComponent);
	}

	if (_generatedModel != nullptr)
	{
		delete _generatedModel;
	}
}


std::vector<glm::vec3>& ShapePolygonComponent::getPoints()
{
	return _points;
}


void ShapePolygonComponent::addPoint(const glm::vec3& position)
{
	_points.push_back(position);
}


void ShapePolygonComponent::setPointPostion(int index, glm::vec3 newPosition)
{
	if (index >= _points.size())
	{
		return;
	}

	_points[index] = newPosition;
}


void ShapePolygonComponent::triangulateAndGenerateMeshIndices(std::vector<unsigned int>& outIndices)
{
	std::vector<bool> _points2(_points.size(), true);

	int numberOfPasses = _points.size() - 2;

	for (int i = 0; i < numberOfPasses; ++i)
	{
		int earPoint = PolygonGenerator::findEar(_points, _points2);
		LOG_DEBUG(LOG_VARIABLE(earPoint));

		if (earPoint >= 0)
		{
			int i1 = PolygonGenerator::getPreviousPointIndex(_points, _points2, earPoint);
			int i2 = earPoint;
			int i3 = PolygonGenerator::getNextPointIndex(_points, _points2, earPoint);

			LOG_DEBUG(LOG_VARIABLE(i1));
			LOG_DEBUG(LOG_VARIABLE(i2));
			LOG_DEBUG(LOG_VARIABLE(i3));

			outIndices.push_back(i3);
			outIndices.push_back(i2);
			outIndices.push_back(i1);
		
			_points2[earPoint] = false;
		}
	}
}


void ShapePolygonComponent::calculateNormals(std::vector<MeshMender::Vertex>& vertices, std::vector<unsigned int>& indices)
{
	for (int i = 0; i < indices.size(); i += 3)
	{
		int i1 = indices[i];
		int i2 = indices[i + 1];
		int i3 = indices[i + 2];

		glm::vec3 normal = glm::cross(glm::vec3(vertices[i3].pos) - glm::vec3(vertices[i2].pos),
									  glm::vec3(vertices[i1].pos) - glm::vec3(vertices[i2].pos));
		normal = glm::normalize(normal);

		vertices[i1].normal += normal;   vertices[i1].normal /= 2.0f;
		vertices[i2].normal += normal;   vertices[i2].normal /= 2.0f;
		vertices[i3].normal += normal;   vertices[i3].normal /= 2.0f;

		vertices[i1].normal = glm::normalize(glm::vec3(vertices[i1].normal));
		vertices[i2].normal = glm::normalize(glm::vec3(vertices[i2].normal));
		vertices[i3].normal = glm::normalize(glm::vec3(vertices[i3].normal));
	}
}


void ShapePolygonComponent::runMeshMender(std::vector<MeshMender::Vertex>& vertices, std::vector<unsigned int>& indices)
{
	MeshMender mender;
	std::vector<unsigned int> temp;
	mender.Mend(vertices, indices, temp, 0.0, 0.7, 0.7, 1.0, MeshMender::DONT_CALCULATE_NORMALS, MeshMender::DONT_RESPECT_SPLITS, MeshMender::DONT_FIX_CYLINDRICAL);
}


Vertex* ShapePolygonComponent::mapVerticesToInternalVertexFormat(std::vector<MeshMender::Vertex>& vertices)
{
	Vertex* outVertices = new Vertex[vertices.size()];
	for (int i = 0; i < vertices.size(); ++i)
	{
		outVertices[i].position = vertices[i].pos;
		outVertices[i].texCoord = glm::vec2(vertices[i].s, vertices[i].t);
		outVertices[i].normal = vertices[i].normal;
		outVertices[i].tangent = vertices[i].tangent;
		outVertices[i].bitangent = vertices[i].binormal;
	}

	return outVertices;
}


void ShapePolygonComponent::buildAndCreateRenderObject(bool useMeshMender)
{
	// vertices
	std::vector<MeshMender::Vertex> verticesVector(_points.size());

	for (int i = 0; i < _points.size(); ++i)
	{
		verticesVector[i].pos = _points[i];
		verticesVector[i].normal = glm::vec3(0.0f, 0.0f, 0.0f);
		verticesVector[i].s = _points[i].x;
		verticesVector[i].t = _points[i].z;
	}

	// indices
	std::vector<unsigned int> indicesVector;
	triangulateAndGenerateMeshIndices(indicesVector);

	// normals
	calculateNormals(verticesVector, indicesVector);

	// tangents and bitangents
	LOG_DEBUG("Before mesh mender. Number of vertices: " + Strings::toString(verticesVector.size()) + ", number of indices: " + Strings::toString(indicesVector.size()));
	if (useMeshMender)
	{
		runMeshMender(verticesVector, indicesVector);
	}
	LOG_DEBUG("After mesh mender. Number of vertices: " + Strings::toString(verticesVector.size()) + ", number of indices: " + Strings::toString(indicesVector.size()));

	Vertex* vertices = mapVerticesToInternalVertexFormat(verticesVector);

	int numberOfIndices = indicesVector.size();
	unsigned int* indices = new unsigned int[numberOfIndices];
	for (int i = 0; i < numberOfIndices; ++i)
	{
		indices[i] = indicesVector[i];
	}

	// meshes
	const bool isModelExist = _generatedModel != nullptr;
	const bool isGame = GameConfig::getInstance().mode == GM_GAME;

	StaticModelMesh* meshes = isModelExist ? _generatedModel->getRootNode()->meshes : new StaticModelMesh[1];
	meshes[0].setMeshData(vertices, verticesVector.size(), indices, numberOfIndices, 0, WIREFRAME_MATERIAL, isGame, DEFAULT_ROAD_BUFFER_SIZE, DEFAULT_ROAD_BUFFER_SIZE, false);

	// collision mesh
	unsigned int collisionMeshSize = meshes[0].indicesCount;
	glm::vec3* collisionMesh = ModelGenerator::generateCollistionMesh(meshes, 1, collisionMeshSize);

	// model
	if (isModelExist)
	{
		_generatedModel->recalculateAABB();
		_generatedModel->setNewCollisionMesh(collisionMesh, collisionMeshSize);

		RenderObject* currentRenderComponent = static_cast<RenderObject*>(getSceneObject()->getComponent(CT_RENDER_OBJECT));
		currentRenderComponent->setModel(_generatedModel);
	}
	else
	{
		std::vector<Material*> materials;
		materials.push_back(new Material);
		materials[0]->shader = NORMALMAPPING_MATERIAL;
		materials[0]->shininess = 96.0f;
		materials[0]->diffuseTexture = ResourceManager::getInstance().loadTexture("RoadProfiles/Road1/PavingStones_col.jpg");
		materials[0]->normalmapTexture = ResourceManager::getInstance().loadTexture("RoadProfiles/Road1/PavingStones_normal.jpg");

		StaticModelNode* modelNode = new StaticModelNode;
		modelNode->name = "road";
		modelNode->meshes = meshes;
		modelNode->meshesCount = 1;
		modelNode->parent = nullptr;

		_generatedModel = new RStaticModel("", modelNode, materials, GL_TRIANGLES, collisionMesh, collisionMeshSize);

		RenderObject* renderObject = getSceneObject()->getSceneManager()->getGraphicsManager()->addRenderObject(new RenderObject(_generatedModel), getSceneObject());
	}
}
