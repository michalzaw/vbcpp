#ifndef GUIRENDERLISTELEMENT_H_INCLUDED
#define GUIRENDERLISTELEMENT_H_INCLUDED


#include <list>

#include <glm/glm.hpp>

#include "../Graphics/VBO.h"
#include "../Graphics/RShader.h"

#include "../Utils/RTexture.h"


class GUIRenderListElement
{
    private:
        VBO*            _vbo;
        unsigned int    _firstVertex;

        glm::mat4       _verticesTransformMatrix;
        glm::mat4       _texCoordTransformMatrix;

        glm::vec4       _color;
        RTexture*       _texture;

        ShaderType      _shaderType;

        std::vector<std::pair<std::string, float>> _customUniforms;

    public:
        GUIRenderListElement(VBO* vbo, unsigned int firstVertex,
                             glm::mat4 verticesTransformMatrix, glm::mat4 texCoordTransformMatrix,
                             glm::vec4 color, RTexture* texture,
                             ShaderType _shaderType,
                             const std::vector<std::pair<std::string, float>>& customUniforms = {});

        ~GUIRenderListElement();


        inline VBO* getVBO()
        { return _vbo; }

        inline unsigned int getFirstVertex()
        { return _firstVertex; }

        inline glm::mat4 getVerticesTransformMatrix()
        { return _verticesTransformMatrix; }

        inline glm::mat4 getTexCoordTransformMatrix()
        { return _texCoordTransformMatrix; }

        inline glm::vec4 getColor()
        { return _color; }

        inline RTexture* getTexture()
        { return _texture; }

        inline ShaderType getShaderType()
        { return _shaderType; }

        inline const std::vector<std::pair<std::string, float>>& getCustomUniforms()
        { return _customUniforms; }

};


typedef std::list<GUIRenderListElement> GUIRenderList;


#endif // GUIRENDERLISTELEMENT_H_INCLUDED
