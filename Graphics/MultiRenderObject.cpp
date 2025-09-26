#include "MultiRenderObject.h"


MultiRenderObject::MultiRenderObject(RStaticModel* model/* = nullptr*/, bool isDynamicObject/* = false*/)
	: RenderObject(model, isDynamicObject)
{
	_type = CT_MULTI_RENDER_OBJECT;
}


MultiRenderObject::MultiRenderObject(RStaticModel* model, const std::vector<std::string>& nodesToSkip, bool isDynamicObject/* = false*/)
	: RenderObject(model, nodesToSkip, isDynamicObject)
{
	_type = CT_MULTI_RENDER_OBJECT;
}


MultiRenderObject::MultiRenderObject(RStaticModel* model, StaticModelNode* modelNode, bool isDynamicObject/* = false*/)
	: RenderObject(model, modelNode, isDynamicObject)
{
	_type = CT_MULTI_RENDER_OBJECT;
}


void MultiRenderObject::calculateNewAABB()
{
	glm::vec3 newMin(FLT_MAX, FLT_MAX, FLT_MAX);
	glm::vec3 newMax(-FLT_MAX, -FLT_MAX, -FLT_MAX);

	int minInstances[3] = { -1, -1, -1 };
	int maxInstances[3] = { -1, -1, -1 };

	for (int i = 0; i < _instancesPositions.size(); ++i)
	{
		if (_instancesPositions[i].x < newMin.x)
		{
			newMin.x = _instancesPositions[i].x;
			minInstances[0] = i;
		}
		if (_instancesPositions[i].x > newMax.x)
		{
			newMax.x = _instancesPositions[i].x;
			maxInstances[0] = i;
		}

		if (_instancesPositions[i].y < newMin.y)
		{
			newMin.y = _instancesPositions[i].y;
			minInstances[1] = i;
		}
		if (_instancesPositions[i].y > newMax.y)
		{
			newMax.y = _instancesPositions[i].y;
			maxInstances[1] = i;
		}

		if (_instancesPositions[i].z < newMin.z)
		{
			newMin.z = _instancesPositions[i].z;
			minInstances[2] = i;
		}
		if (_instancesPositions[i].z > newMax.z)
		{
			newMax.z = _instancesPositions[i].z;
			maxInstances[2] = i;
		}
	}

	RStaticModel* model = _modelsDatas[0].model;
	glm::vec3 minFromModel = model->getAABB()->getMinCoords();
	glm::vec3 maxFromModel = model->getAABB()->getMaxCoords();

	minFromModel.x += _instancesPositions[minInstances[0]].x;
	minFromModel.y += _instancesPositions[minInstances[1]].y;
	minFromModel.z += _instancesPositions[minInstances[2]].z;

	maxFromModel.x += _instancesPositions[maxInstances[0]].x;
	maxFromModel.y += _instancesPositions[maxInstances[1]].y;
	maxFromModel.z += _instancesPositions[maxInstances[2]].z;

	_aabb.setSize(minFromModel, maxFromModel);
}


void MultiRenderObject::recreateInstancesPositionVBO()
{

	_instancesPositionsVBO = OGLDriver::getInstance().createVBO(_instancesPositions.size() * sizeof(glm::vec3));
	_instancesPositionsVBO->addVertexData(&_instancesPositions[0], _instancesPositions.size());
}
