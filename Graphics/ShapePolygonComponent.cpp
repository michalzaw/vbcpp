#include "ShapePolygonComponent.h"

#include "GraphicsManager.h"
#include "RenderObject.h"

#include "../Scene/SceneManager.h"

#include "../Utils/PolygonGenerator.h"

const unsigned int DEFAULT_ROAD_BUFFER_SIZE = 1024 * 1024; // ~ 4600 vertices

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


glm::vec3* ShapePolygonComponent::generateCollistionMesh(StaticModelMesh* meshes, unsigned int meshesCount, unsigned int totalIndicesCount)
{
	glm::vec3* collisionMesh = new glm::vec3[totalIndicesCount];
	int counter = 0;

	for (int i = 0; i < meshesCount; ++i)
	{
		unsigned int indicesCount = meshes[i].indicesCount;

		for (int j = 0; j < indicesCount; ++j)
		{
			collisionMesh[counter + j] = meshes[i].vertices[meshes[i].indices[j] - meshes[i].firstVertexInVbo].position;
		}

		counter += indicesCount;
	}

	return collisionMesh;
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


void ShapePolygonComponent::buildAndCreateRenderObject()
{
	std::vector<bool> _points2(_points.size(), true);

	int numberOfPasses = _points.size() - 2;
	int numberOfIndices = numberOfPasses * 3;

	Vertex* vertices = new Vertex[_points.size()];
	unsigned int* indices = new unsigned int[numberOfIndices];

	for (int i = 0; i < _points.size(); ++i)
	{
		vertices[i].position = _points[i];
		vertices[i].normal = glm::vec3(0.0f, 1.0f, 0.0f);
	}

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

			indices[i * 3 + 0] = i3;
			indices[i * 3 + 1] = i2;
			indices[i * 3 + 2] = i1;
		}

		if (earPoint >= 0)
		{
			_points2[earPoint] = false;
		}
	}

	const bool isModelExist = _generatedModel != nullptr;
	const bool isGame = GameConfig::getInstance().mode == GM_GAME;

	StaticModelMesh* meshes = isModelExist ? _generatedModel->getRootNode()->meshes : new StaticModelMesh[1];
	if (isModelExist)
	{
		LOG_DEBUG("Build custom polygon: update existing meshes");
		meshes[0].updateMeshData(vertices, _points.size(), indices, numberOfIndices);
	}
	else if (isGame)
	{
		LOG_DEBUG("Build custom polygon: create new meshes in Game Mode");
		meshes[0].setMeshData(vertices, _points.size() > 2 ? _points.size() : 3, indices, numberOfIndices, 0, WIREFRAME_MATERIAL);
	}
	else
	{
		LOG_DEBUG("Build custom polygon: create new meshes in Editor Mode");
		meshes[0].setMeshData(vertices, _points.size() > 2 ? _points.size() : 3, indices, numberOfIndices, 0, WIREFRAME_MATERIAL, false, DEFAULT_ROAD_BUFFER_SIZE, DEFAULT_ROAD_BUFFER_SIZE, false);
	}


	unsigned int collisionMeshSize = meshes[0].indicesCount;
	glm::vec3* collisionMesh = generateCollistionMesh(meshes, 1, collisionMeshSize);


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
		materials[0]->shader = WIREFRAME_MATERIAL;

		StaticModelNode* modelNode = new StaticModelNode;
		modelNode->name = "road";
		modelNode->meshes = meshes;
		modelNode->meshesCount = 1;
		modelNode->parent = nullptr;

		_generatedModel = new RStaticModel("", modelNode, materials, GL_TRIANGLES, collisionMesh, collisionMeshSize);

		RenderObject* renderObject = getSceneObject()->getSceneManager()->getGraphicsManager()->addRenderObject(new RenderObject(_generatedModel), getSceneObject());
	}
}
