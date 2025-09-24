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


void MultiRenderObject::recreateInstancesPositionVBO()
{

	_instancesPositionsVBO = OGLDriver::getInstance().createVBO(_instancesPositions.size() * sizeof(glm::vec3));
	_instancesPositionsVBO->addVertexData(&_instancesPositions[0], _instancesPositions.size());
}
