#ifndef MATERIAL_H_INCLUDED
#define MATERIAL_H_INCLUDED


#include <glm/glm.hpp>

#include "Shader.h"
#include <memory>
using namespace std;

#include "../Utils/RTexture.h"

class Material
{
    public:
        Material()
        : diffuseTexture(0), normalmapTexture(0), shininess(0), transparency(0), _shader(0), _diffTex(0) {}
        virtual ~Material()
        {
            if(_diffTex)
            {
                //_diffTex->drop();
                //_diffTex = 0;
            }
 }

    // ! Jakie parametry !
    std::string name;

    glm::vec4 ambientColor;
    glm::vec4 diffuseColor;
    glm::vec4 specularColor;

    RTexture* _diffTex;

    GLuint diffuseTexture;
    GLuint normalmapTexture;

    float shininess;
    float transparency;

    glm::vec2 offset;
    glm::vec2 scale;

    /*
    shared_ptr<Shader> getShader()
    {
        return (!_shader.expired() ? _shader.lock() : 0);
    }

    void setShader(shared_ptr<Shader>& s)
    {
        _shader = s;
    }
    */

    void setDiffuseText(RTexture* tex)
    {
        if (_diffTex)
            _diffTex->drop();

        tex->grab();
        _diffTex = tex;

    }

    RTexture* getDiffTex()
    {
        return _diffTex;
    }

    Shader* _shader;

    protected:
        //weak_ptr<Shader> _shader;
        //Shader* _shader;

};


#endif // MATERIAL_H_INCLUDED
