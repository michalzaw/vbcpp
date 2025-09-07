#ifndef MATERIAL_H_INCLUDED
#define MATERIAL_H_INCLUDED


#include <glm/glm.hpp>

#include "RShader.h"
#include "EnvironmentCaptureComponent.h"
#include "../Utils/RTexture.h"
#include <memory>
using namespace std;

#include "../Scripting/Utils/LuaMacros.h"

#include "../Utils/RTexture.h"


enum EnvironmentMapType
{
    EMT_LOCAL,
    EMT_GLOBAL
};


class Material
{
    public:
        Material()
        : diffuseTexture(NULL), normalmapTexture(NULL), glassTexture(NULL), metalicTexture(NULL), roughnessTexture(NULL), aoTexture(NULL), opacityMaskTexture(NULL), emissiveTexture(NULL),
		reflectionTexture1(EMT_GLOBAL), reflectionTexture2(EMT_GLOBAL),
        shininess(0), transparency(0), shader(SOLID_MATERIAL), emissiveColor(0, 0, 0, 0), fixDisappearanceAlpha(0.0),
		metalnessValue(0.0f), roughnessValue(0.9f), requireSeparateInstance(false){}
        virtual ~Material()
        { }

    // ! Jakie parametry !
    LUAV std::string name;

    LUAV glm::vec4 ambientColor;
    LUAV glm::vec4 diffuseColor;
    LUAV glm::vec4 specularColor;
    LUAV glm::vec4 emissiveColor;

    RTexture* diffuseTexture;
    RTexture* normalmapTexture;
    RTexture* glassTexture;
	RTexture* metalicTexture;
	RTexture* roughnessTexture;
	RTexture* aoTexture;
    RTexture* opacityMaskTexture;
	RTexture* emissiveTexture;

    EnvironmentMapType reflectionTexture1;
    EnvironmentMapType reflectionTexture2;

    LUAV float shininess;
    LUAV float transparency;

    LUAV float metalnessValue;
    LUAV float roughnessValue;

    LUAV float fixDisappearanceAlpha;
    LUAV float alphaTestThreshold;
    LUAV float shadowmappingAlphaTestThreshold;

    glm::vec2 offset;
    glm::vec2 scale;

    std::string mirrorName;

    bool requireSeparateInstance;

    //RShader* _shader;
    ShaderType shader;

};


#endif // MATERIAL_H_INCLUDED
