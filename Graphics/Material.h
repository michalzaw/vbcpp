#ifndef MATERIAL_H_INCLUDED
#define MATERIAL_H_INCLUDED


#include <glm/glm.hpp>

#include "RShader.h"
#include "EnvironmentCaptureComponent.h"
#include "../Utils/RTexture.h"
#include <memory>
using namespace std;

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
        : diffuseTexture(NULL), normalmapTexture(NULL), glassTexture(NULL), metalicTexture(NULL), roughnessTexture(NULL), aoTexture(NULL), emissiveTexture(NULL),
		reflectionTexture1(EMT_GLOBAL), reflectionTexture2(EMT_GLOBAL),
        shininess(0), transparency(0), shader(SOLID_MATERIAL), emissiveColor(0, 0, 0, 0), fixDisappearanceAlpha(0.0) {}
        virtual ~Material()
        { }

    // ! Jakie parametry !
    std::string name;

    glm::vec4 ambientColor;
    glm::vec4 diffuseColor;
    glm::vec4 specularColor;
    glm::vec4 emissiveColor;

    RTexture* diffuseTexture;
    RTexture* normalmapTexture;
    RTexture* glassTexture;
	RTexture* metalicTexture;
	RTexture* roughnessTexture;
	RTexture* aoTexture;
	RTexture* emissiveTexture;

    EnvironmentMapType reflectionTexture1;
    EnvironmentMapType reflectionTexture2;

    float shininess;
    float transparency;

	float fixDisappearanceAlpha;

    glm::vec2 offset;
    glm::vec2 scale;

    std::string mirrorName;

    //RShader* _shader;
    ShaderType shader;

};


#endif // MATERIAL_H_INCLUDED
