#ifndef MULTIRENDEROBJECT_H_INCLUDED
#define MULTIRENDEROBJECT_H_INCLUDED


#include "RenderObject.h"


class MultiRenderObject : public RenderObject
{
	VBCPP_COMPONENT(MultiRenderObject, CT_MULTI_RENDER_OBJECT)

	private:
		std::vector<glm::vec3> _instancesPositions;
		VBO* _instancesPositionsVBO;

		void calculateNewAABB() override;

	public:
		MultiRenderObject(RStaticModel* model = nullptr, bool isDynamicObject = false);
		MultiRenderObject(RStaticModel* model, const std::vector<std::string>& nodesToSkip, bool isDynamicObject = false);
		MultiRenderObject(RStaticModel* model, StaticModelNode* modelNode, bool isDynamicObject = false);

		inline std::vector<glm::vec3>& getInstancesPositions() { return _instancesPositions; }
		inline VBO* getInstancesPositionVBO() { return _instancesPositionsVBO; }

		void recreateInstancesPositionVBO();

};


#endif // MULTIRENDEROBJECT_H_INCLUDED
