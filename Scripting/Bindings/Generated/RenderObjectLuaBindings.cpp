// Generated file
#include "RenderObjectLuaBindings.h"

#include "../../../Graphics/RenderObject.h"


namespace RenderObjectLuaBindings
{
    void bind(sol::state* lua)
    {
        lua->new_usertype<RenderObject>("RenderObject",
            sol::base_classes, sol::bases<Component>(),

            "getModelRootNode", &RenderObject::getModelRootNode,
            "setCastShadows", &RenderObject::setCastShadows,
            "isCastShadows", &RenderObject::isCastShadows,
            "setDynamicObject", &RenderObject::setDynamicObject,
            "isDynamicObject", &RenderObject::isDynamicObject,
            "setIsHighlighted", &RenderObject::setIsHighlighted,
            "isHighlighted", &RenderObject::isHighlighted,
            "setIsRenderObjectId", &RenderObject::setIsRenderObjectId,
            "isRenderObjectId", &RenderObject::isRenderObjectId,
            "getNumberOfLod", &RenderObject::getNumberOfLod,

            "getModelNodeByName", sol::resolve<ModelNode*(std::string, int)>(&RenderObject::getModelNodeByName)


        );
    }
}
