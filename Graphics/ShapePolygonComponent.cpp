#include "ShapePolygonComponent.h"

#include "GraphicsManager.h"
#include "RenderObject.h"

#include "../Scene/SceneManager.h"

#include "../Utils/PolygonGenerator.h"


ShapePolygonComponent::ShapePolygonComponent()
	: Component(CT_SHAPE_POLYGON)
{

}

ShapePolygonComponent::ShapePolygonComponent(std::vector<glm::vec3>& points)
	: Component(CT_SHAPE_POLYGON),
	_points(points)
{

}


ShapePolygonComponent::~ShapePolygonComponent()
{

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
	Component* currentRenderComponent = getSceneObject()->getComponent(CT_RENDER_OBJECT);
	if (currentRenderComponent)
	{
		getSceneObject()->removeComponent(currentRenderComponent);
	}

	bool isOnePass = false;
	std::vector<unsigned int> indices;
	std::vector<bool> _points2;

	for (int i = 0; i < _points.size(); ++i)
	{
		_points2.push_back(true);
	}

	Vertex* vertices = new Vertex[_points.size()];
	for (int i = 0; i < _points.size(); ++i)
	{
		vertices[i].position = _points[i];
	}

	int numberOfPasses = isOnePass ? 1 : (_points.size() - 2);
	for (int i = 0; i < numberOfPasses; ++i)
	{
		int earPoint = PolygonGenerator::findEar(_points, _points2);
		LOG_DEBUG(LOG_VARIABLE(earPoint));

		//unsigned int* indices = new unsigned int[3];
		if (earPoint >= 0)
		{
			int i1 = PolygonGenerator::getPreviousPointIndex(_points, _points2, earPoint);
			int i2 = earPoint;
			int i3 = PolygonGenerator::getNextPointIndex(_points, _points2, earPoint);

			LOG_DEBUG(LOG_VARIABLE(i1));
			LOG_DEBUG(LOG_VARIABLE(i2));
			LOG_DEBUG(LOG_VARIABLE(i3));

			indices.push_back(i3);
			indices.push_back(i2);
			indices.push_back(i1);
			//indices[0] = i3;
			//indices[1] = i2;
			//indices[2] = i1;
		}

		if (earPoint >= 0)
		{
			_points2[earPoint] = false;
			//_points.erase(_points.begin() + earPoint);
		}
	}

	std::vector<Material*> materials;
	materials.push_back(new Material);
	materials[0]->shader = WIREFRAME_MATERIAL;

	StaticModelMesh* meshes = new StaticModelMesh[1];
	meshes[0].setMeshData(vertices, _points.size() > 2 ? _points.size() : 3, indices.data(), indices.size(), 0, WIREFRAME_MATERIAL, false);

	StaticModelNode* modelNode = new StaticModelNode;
	modelNode->name = "road";
	modelNode->meshes = meshes;
	modelNode->meshesCount = 1;
	modelNode->parent = nullptr;

	RStaticModel* model = new RStaticModel("", modelNode, materials, GL_TRIANGLES, nullptr, 0);
	RenderObject* renderObject = getSceneObject()->getSceneManager()->getGraphicsManager()->addRenderObject(new RenderObject(model), getSceneObject());
}
