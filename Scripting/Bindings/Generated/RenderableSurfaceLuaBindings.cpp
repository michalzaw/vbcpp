// Generated file
#include "RenderableSurfaceLuaBindings.h"

#include "../../../Graphics/RenderableSurface.h"


namespace RenderableSurfaceLuaBindings
{
    void bind(sol::state* lua)
    {
        lua->new_usertype<RenderableSurface>("RenderableSurface",
            sol::base_classes, sol::bases<Component>(),

            "addImage", &RenderableSurface::addImage,
            "addLabel", &RenderableSurface::addLabel,
            "removeObject", &RenderableSurface::removeObject,
            "removeAllObjects", &RenderableSurface::removeAllObjects,
            "setBackgroundColor", &RenderableSurface::setBackgroundColor,
            "getBackgroundColor", &RenderableSurface::getBackgroundColor,
            "getRenderedTexture", &RenderableSurface::getRenderedTexture



        );
    }
}
