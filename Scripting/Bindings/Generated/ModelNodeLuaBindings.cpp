// Generated file
#include "ModelNodeLuaBindings.h"

#include "../../../Graphics/ModelNode.h"


namespace ModelNodeLuaBindings
{
    void bind(sol::state* lua)
    {
        lua->new_usertype<ModelNode>("ModelNode",

            "getName", &ModelNode::getName,
            "getTransformNode", &ModelNode::getTransformNode,
            "getTransform", &ModelNode::getTransform,
            //"getMesh", &ModelNode::getMesh,
            "getMeshesCount", &ModelNode::getMeshesCount,
            "getParent", &ModelNode::getParent,
            "getChildren", &ModelNode::getChildren,
            "getChildrenCount", &ModelNode::getChildrenCount,

            // added manually
            "getMeshMaterial", [](ModelNode& modelNode, unsigned int index) { return modelNode.getMesh(index)->material; }

        );
    }
}
