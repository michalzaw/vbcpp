// Generated file
#include "MaterialLuaBindings.h"

#include "../../../Graphics/Material.h"


namespace MaterialLuaBindings
{
    void bind(sol::state* lua)
    {
        lua->new_usertype<Material>("Material",




            "name", &Material::name,
            "ambientColor", &Material::ambientColor,
            "diffuseColor", &Material::diffuseColor,
            "specularColor", &Material::specularColor,
            "emissiveColor", &Material::emissiveColor,
            "shininess", &Material::shininess,
            "transparency", &Material::transparency,
            "metalnessValue", &Material::metalnessValue,
            "roughnessValue", &Material::roughnessValue,
            "fixDisappearanceAlpha", &Material::fixDisappearanceAlpha,
            "alphaTestThreshold", &Material::alphaTestThreshold,
            "shadowmappingAlphaTestThreshold", &Material::shadowmappingAlphaTestThreshold
        );
    }
}
