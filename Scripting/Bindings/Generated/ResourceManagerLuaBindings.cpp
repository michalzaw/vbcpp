// Generated file
#include "ResourceManagerLuaBindings.h"

#include "../../../Utils/ResourceManager.h"


namespace ResourceManagerLuaBindings
{
    void bind(sol::state* lua)
    {
        lua->new_usertype<ResourceManager>("ResourceManager",

            "loadTexture", &ResourceManager::loadTexture,
            "loadDefaultWhiteTexture", &ResourceManager::loadDefaultWhiteTexture,
            "loadOneColorTexture", &ResourceManager::loadOneColorTexture,
            "loadFont", &ResourceManager::loadFont,
            "loadSound", &ResourceManager::loadSound



        );
    }
}
