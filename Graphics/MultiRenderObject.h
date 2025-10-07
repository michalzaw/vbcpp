#ifndef MULTIRENDEROBJECT_H_INCLUDED
#define MULTIRENDEROBJECT_H_INCLUDED


#include "RenderObject.h"


class MultiRenderObject : public Component
{
	VBCPP_COMPONENT(MultiRenderObject, CT_MULTI_RENDER_OBJECT)

	private:
		RenderObject* _renderObject;

		std::vector<glm::vec3> _instancesPositions;
		VBO* _instancesPositionsVBO;

		AABB _aabb;
		bool _isCalculatedAABB;

		void calculateNewAABB();

	public:
		MultiRenderObject(RenderObject* renderObject);
		~MultiRenderObject();

		inline RenderObject* getRenderObject() { return _renderObject; }

		inline std::vector<glm::vec3>& getInstancesPositions() { return _instancesPositions; }
		inline VBO* getInstancesPositionVBO() { return _instancesPositionsVBO; }

		AABB* getAABB();

		void recreateInstancesPositionVBO();

		void changedTransform() override;

};


#endif // MULTIRENDEROBJECT_H_INCLUDED
