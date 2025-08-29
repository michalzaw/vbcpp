#include "RenderObjectLuaBindings.h"

#include "../../../Graphics/RenderObject.h"


namespace RenderObjectLuaBindings
{
    void bind(sol::state* lua)
    {
        lua->new_usertype<RenderObject>("RenderObject",
            sol::base_classes, sol::bases<Component>(),

            "getModel", &RenderObject::getModel,
            "getModelRootNode", &RenderObject::getModelRootNode,
            "updateLocalMaterialFromModel", &RenderObject::updateLocalMaterialFromModel,
            "replaceMaterialsByName", &RenderObject::replaceMaterialsByName,
            "addMirrorMaterial", &RenderObject::addMirrorMaterial,
            "getMirrorMaterials", &RenderObject::getMirrorMaterials,
            "setCastShadows", &RenderObject::setCastShadows,
            "isCastShadows", &RenderObject::isCastShadows,
            "setDynamicObject", &RenderObject::setDynamicObject,
            "isDynamicObject", &RenderObject::isDynamicObject,
            "setIsHighlighted", &RenderObject::setIsHighlighted,
            "isHighlighted", &RenderObject::isHighlighted,
            "setIsRenderObjectId", &RenderObject::setIsRenderObjectId,
            "isRenderObjectId", &RenderObject::isRenderObjectId,
            "getAABB", &RenderObject::getAABB,
            "getNumberOfLod", &RenderObject::getNumberOfLod,
            "changedTransform", &RenderObject::changedTransform,

            "getModelNodeByName", sol::resolve<ModelNode*(std::string, int)>(&RenderObject::getModelNodeByName),

            "setModel", sol::overload(
                sol::resolve<void(RStaticModel*, int)>(&RenderObject::setModel),
                sol::resolve<void(RStaticModel*, StaticModelNode*, int)>(&RenderObject::setModel),
                sol::resolve<void(RStaticModel*, const std::vector<std::string>&, StaticModelNode*, int)>(&RenderObject::setModel)
            )
        );
    }
}
