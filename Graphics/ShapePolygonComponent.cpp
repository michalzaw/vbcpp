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
	/*for (int i = 0; i < _points.size(); ++i)
	{
		_points2.push_back(true);
	}*/

	int numberOfPasses = _points.size() - 2;
	int numberOfIndices = numberOfPasses * 3;

	Vertex* vertices = new Vertex[_points.size()];
	unsigned int* indices = new unsigned int[numberOfIndices];

	for (int i = 0; i < _points.size(); ++i)
	{
		vertices[i].position = _points[i];
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

		_generatedModel->recalculateAABB();
		//_generatedModel->setNewCollisionMesh(collisionMesh, collistionMeshSize);

		RenderObject* currentRenderComponent = static_cast<RenderObject*>(getSceneObject()->getComponent(CT_RENDER_OBJECT));
		currentRenderComponent->setModel(_generatedModel);
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

	if (!isModelExist)
	{
		std::vector<Material*> materials;
		materials.push_back(new Material);
		materials[0]->shader = WIREFRAME_MATERIAL;

		StaticModelNode* modelNode = new StaticModelNode;
		modelNode->name = "road";
		modelNode->meshes = meshes;
		modelNode->meshesCount = 1;
		modelNode->parent = nullptr;

		_generatedModel = new RStaticModel("", modelNode, materials, GL_TRIANGLES, nullptr, 0);

		RenderObject* renderObject = getSceneObject()->getSceneManager()->getGraphicsManager()->addRenderObject(new RenderObject(_generatedModel), getSceneObject());
	}
}
