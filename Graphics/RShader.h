#ifndef SHADER_H_INCLUDED
#define SHADER_H_INCLUDED

#include <GLEW/glew.h>
#include <glm/glm.hpp>

#include "../Utils/Resource.h"
#include "../Utils/RTexture.h"

#include <iostream>

enum ShaderType
{
    SOLID_MATERIAL,
    NOTEXTURE_MATERIAL,
    NORMALMAPPING_MATERIAL,
    ALPHA_TEST_MATERIAL,
    TREE_MATERIAL,
    GRASS_MATERIAL,
    SKY_MATERIAL,
    GLASS_MATERIAL,
    CAR_PAINT,

    GUI_IMAGE_SHADER,
    GUI_LABEL_SHADER,

    DEBUG_SHADER,

    SHADOWMAP_SHADER,
    SHADOWMAP_ALPHA_TEST_SHADER,

    NUMBER_OF_SHADERS

};


enum UniformName
{
    UNIFORM_MVP,
    UNIFORM_MODEL_MATRIX,
    UNIFORM_NORMAL_MATRIX,
    UNIFORM_DIFFUSE_TEXTURE,
    UNIFORM_NOTMALMAP_TEXTURE,
    UNIFORM_ALPHA_TEXTURE,
    UNIFORM_ENVIRONMENTMAP_TEXTURE,
    UNIFORM_GLASS_TEXTURE,
    UNIFORM_MATERIAL_AMBIENT_COLOR,
    UNIFORM_MATERIAL_DIFFUSE_COLOR,
    UNIFORM_MATERIAL_SPECULAR_COLOR,
    UNIFORM_MATERIAL_SPECULAR_POWER,
    UNIFORM_MATERIAL_TRANSPARENCY,
    UNIFORM_CAMERA_POSITION,
    UNIFORM_LIGHT_SPACE_MATRIX_1,
    UNIFORM_LIGHT_SPACE_MATRIX_2,
    UNIFORM_LIGHT_SPACE_MATRIX_3,
    UNIFORM_SHADOW_MAP_1,
    UNIFORM_SHADOW_MAP_2,
    UNIFORM_SHADOW_MAP_3,
    UNIFORM_GRASS_COLOR,
    UNIFORM_HEIGHTMAP,
    UNIFORM_GRASS_DENSITY,
    UNIFORM_GRASS_MIN,
    UNIFORM_GRASS_WIDTH,
    UNIFORM_WIND_DIRECTION,
    UNIFORM_GUI_VERTICES_TRANSFORM_MATRIX,
    UNIFORM_GUI_TEXCOORDS_TRANSFORM_MATRIX,
    UNIFORM_GUI_COLOR,
    UNIFORM_DAY_NIGHT_RATIO,

    NUMBER_OF_UNIFORMS
};


class RShader : virtual public Resource
{
    friend class ResourceManager;

    private:
        GLuint _shaderID;
        int _textureLocation;

        const char* _uniformsNames[NUMBER_OF_UNIFORMS];
        GLint _uniformsLocations[NUMBER_OF_UNIFORMS];

        void setNewShader(GLuint newId)
        {
            glDeleteProgram(_shaderID);
            _shaderID = newId;
        }

    public:
        RShader(std::string path, GLuint id)
            : Resource(RT_SHADER, path),  _shaderID(id), _textureLocation(0)
        {
            _uniformsNames[UNIFORM_MVP] = "MVP";
            _uniformsNames[UNIFORM_MODEL_MATRIX] = "ModelMatrix";
            _uniformsNames[UNIFORM_NORMAL_MATRIX] = "NormalMatrix";
            _uniformsNames[UNIFORM_DIFFUSE_TEXTURE] = "Texture";
            _uniformsNames[UNIFORM_NOTMALMAP_TEXTURE] = "NormalmapTexture";
            _uniformsNames[UNIFORM_ALPHA_TEXTURE] = "AlphaTexture";
            _uniformsNames[UNIFORM_ENVIRONMENTMAP_TEXTURE] = "environmentMap";
            _uniformsNames[UNIFORM_GLASS_TEXTURE] = "glassTexture";
            _uniformsNames[UNIFORM_MATERIAL_AMBIENT_COLOR] = "matAmbient";
            _uniformsNames[UNIFORM_MATERIAL_DIFFUSE_COLOR] = "matDiffuse";
            _uniformsNames[UNIFORM_MATERIAL_SPECULAR_COLOR] = "matSpecular";
            _uniformsNames[UNIFORM_MATERIAL_SPECULAR_POWER] = "SpecularPower";
            _uniformsNames[UNIFORM_MATERIAL_TRANSPARENCY] = "Transparency";
            _uniformsNames[UNIFORM_CAMERA_POSITION] = "CameraPosition";
            _uniformsNames[UNIFORM_LIGHT_SPACE_MATRIX_1] = "LightSpaceMatrix[0]";
            _uniformsNames[UNIFORM_LIGHT_SPACE_MATRIX_2] = "LightSpaceMatrix[1]";
            _uniformsNames[UNIFORM_LIGHT_SPACE_MATRIX_3] = "LightSpaceMatrix[2]";
            _uniformsNames[UNIFORM_SHADOW_MAP_1] = "ShadowMap[0]";
            _uniformsNames[UNIFORM_SHADOW_MAP_2] = "ShadowMap[1]";
            _uniformsNames[UNIFORM_SHADOW_MAP_3] = "ShadowMap[2]";
            _uniformsNames[UNIFORM_GRASS_COLOR] = "grassColor";
            _uniformsNames[UNIFORM_HEIGHTMAP] = "heightmap";
            _uniformsNames[UNIFORM_GRASS_DENSITY] = "grassDensity";
            _uniformsNames[UNIFORM_GRASS_MIN] = "min";
            _uniformsNames[UNIFORM_GRASS_WIDTH] = "width";
            _uniformsNames[UNIFORM_WIND_DIRECTION] = "d";
            _uniformsNames[UNIFORM_GUI_VERTICES_TRANSFORM_MATRIX] = "VerticesTransformMatrix";
            _uniformsNames[UNIFORM_GUI_TEXCOORDS_TRANSFORM_MATRIX] = "TexCoordTransformMatrix";
            _uniformsNames[UNIFORM_GUI_COLOR] = "color";
            _uniformsNames[UNIFORM_DAY_NIGHT_RATIO] = "dayNightRatio";

            for (int i = 0; i < NUMBER_OF_UNIFORMS; ++i)
            {
                _uniformsLocations[i] = glGetUniformLocation(_shaderID, _uniformsNames[i]);
            }

            std::cout << "RShader: Konstruktor: " << _shaderID <<  "\n";
        }

        virtual ~RShader()
        {
            std::cout << "RShader: Destruktor: " << _shaderID << "\n";
            glDeleteProgram(_shaderID);
        }

        void enable()
        {
            glUseProgram(_shaderID);

            _textureLocation = 0;
        }
        void disable()
        {
            glUseProgram(0);
        }

        inline void resetTextureLocation()
        {
            _textureLocation = 0;
        }

        inline void setUniform(UniformName uniform, int* values, int count = 1)
		{ glUniform1iv(_uniformsLocations[uniform], count, values); }
		inline void setUniform(UniformName uniform, float* values, int count = 1)
		{ glUniform1fv(_uniformsLocations[uniform], count, values); }
		inline void setUniform(UniformName uniform, int value)
		{ glUniform1i(_uniformsLocations[uniform], value); }
		inline void setUniform(UniformName uniform, float value)
		{ glUniform1f(_uniformsLocations[uniform], value); }
		inline void setUniform(UniformName uniform, glm::vec2* vectors, int count = 1)
		{ glUniform2fv(_uniformsLocations[uniform], count, (GLfloat*)vectors); }
		inline void setUniform(UniformName uniform, glm::vec3* vectors, int count = 1)
		{ glUniform3fv(_uniformsLocations[uniform], count, (GLfloat*)vectors); }
		inline void setUniform(UniformName uniform, glm::vec4* vectors, int count = 1)
		{ glUniform4fv(_uniformsLocations[uniform], count, (GLfloat*)vectors); }
		inline void setUniform(UniformName uniform, glm::vec2 vector)
		{ glUniform2f(_uniformsLocations[uniform], vector.x, vector.y); }
		inline void setUniform(UniformName uniform, glm::vec3 vector)
		{ glUniform3f(_uniformsLocations[uniform], vector.x, vector.y, vector.z); }
		inline void setUniform(UniformName uniform, glm::vec4 vector)
		{ glUniform4f(_uniformsLocations[uniform], vector.x, vector.y, vector.z, vector.w); }
		inline void setUniform(UniformName uniform, glm::mat3* matrices, int count = 1)
		{ glUniformMatrix4fv(_uniformsLocations[uniform], count, GL_FALSE, (GLfloat*)matrices); }
		inline void setUniform(UniformName uniform, glm::mat4* matrices, int count = 1)
		{ glUniformMatrix3fv(_uniformsLocations[uniform], count, GL_FALSE, (GLfloat*)matrices); }
		inline void setUniform(UniformName uniform, glm::mat3 matrix)
		{ glUniformMatrix3fv(_uniformsLocations[uniform], 1, GL_FALSE, &matrix[0][0]); }
		inline void setUniform(UniformName uniform, glm::mat4 matrix)
		{ glUniformMatrix4fv(_uniformsLocations[uniform], 1, GL_FALSE, &matrix[0][0]); }


        inline void bindTexture(UniformName uniform, RTexture* texture)
        {
            glActiveTexture(GL_TEXTURE0 + _textureLocation);
            texture->bind();
            setUniform(uniform, _textureLocation++);
        }

        inline void getUniformOffset(const char** names, unsigned int count, int* offsets)
        {
            GLuint uniformIndices[count];
            glGetUniformIndices(_shaderID, count, names, uniformIndices);

            glGetActiveUniformsiv(_shaderID, count, uniformIndices, GL_UNIFORM_OFFSET, offsets);
        }

        inline void getUniformArrayStride(const char** names, unsigned int count, int* arrayStrides)
        {
            GLuint uniformIndices[count];
            glGetUniformIndices(_shaderID, count, names, uniformIndices);

            glGetActiveUniformsiv(_shaderID, count, uniformIndices, GL_UNIFORM_ARRAY_STRIDE, arrayStrides);
        }

        inline void getUniformMatrixStride(const char** names, unsigned int count, int* matrixStrides)
        {
            GLuint uniformIndices[count];
            glGetUniformIndices(_shaderID, count, names, uniformIndices);

            glGetActiveUniformsiv(_shaderID, count, uniformIndices, GL_UNIFORM_MATRIX_STRIDE, matrixStrides);
        }

        inline void setUniformBlockBinding(const char* blocksname, unsigned int location)
        {
            GLuint index = glGetUniformBlockIndex(_shaderID, blocksname);
            glUniformBlockBinding(_shaderID, index, location);
        }

};


#endif // SHADER_H_INCLUDED
/*
=======
#ifndef SHADER_H_INCLUDED
#define SHADER_H_INCLUDED

#include <GLEW/glew.h>
#include <glm/glm.hpp>

#include <iostream>

#include "../Utils/Resource.h"


enum ShaderType
{
    SOLID_MATERIAL,
    NOTEXTURE_MATERIAL,
    NORMALMAPPING_MATERIAL
};


class RShader : virtual public Resource
{
    private:
        GLuint _shaderID;

        int _textureLocation;

    public:
        RShader(std::string path, GLuint ID) : Resource(path),  _shaderID(ID), _textureLocation(0) {  std::cout << "RShader: Konstruktor: " << _shaderID <<  "\n"; }
        virtual ~RShader()
        {
            std::cout << "RShader: Destruktor: " << _shaderID << "\n";
            glDeleteProgram(_shaderID);
        }

        void Enable()
        {
            glUseProgram(_shaderID);

            _textureLocation = 0;
        }
        void Disable()
        {
            glUseProgram(0);
        }

        inline void setUniform(char* name, int* values, int count = 1)
		{ glUniform1iv(glGetUniformLocation(_shaderID, name), count, values); }
		inline void setUniform(char* name, float* values, int count = 1)
		{ glUniform1fv(glGetUniformLocation(_shaderID, name), count, values); }
		inline void setUniform(char* name, int value)
		{ glUniform1i(glGetUniformLocation(_shaderID, name), value); }
		inline void setUniform(char* name, float value)
		{ glUniform1f(glGetUniformLocation(_shaderID, name), value); }
		inline void setUniform(char* name, glm::vec2* vectors, int count = 1)
		{ glUniform2fv(glGetUniformLocation(_shaderID, name), count, (GLfloat*)vectors); }
		inline void setUniform(char* name, glm::vec3* vectors, int count = 1)
		{ glUniform3fv(glGetUniformLocation(_shaderID, name), count, (GLfloat*)vectors); }
		inline void setUniform(char* name, glm::vec4* vectors, int count = 1)
		{ glUniform4fv(glGetUniformLocation(_shaderID, name), count, (GLfloat*)vectors); }
		inline void setUniform(char* name, glm::vec2 vector)
		{ glUniform2f(glGetUniformLocation(_shaderID, name), vector.x, vector.y); }
		inline void setUniform(char* name, glm::vec3 vector)
		{ glUniform3f(glGetUniformLocation(_shaderID, name), vector.x, vector.y, vector.z); }
		inline void setUniform(char* name, glm::vec4 vector)
		{ glUniform4f(glGetUniformLocation(_shaderID, name), vector.x, vector.y, vector.z, vector.w); }
		inline void setUniform(char* name, glm::mat3* matrices, int count = 1)
		{ glUniformmatrix4fv(glGetUniformLocation(_shaderID, name), count, GL_FALSE, (GLfloat*)matrices); }
		inline void setUniform(char* name, glm::mat4* matrices, int count = 1)
		{ glUniformmatrix3fv(glGetUniformLocation(_shaderID, name), count, GL_FALSE, (GLfloat*)matrices); }
		inline void setUniform(char* name, glm::mat3 matrix)
		{ glUniformmatrix3fv(glGetUniformLocation(_shaderID, name), 1, GL_FALSE, &matrix[0][0]); }
		inline void setUniform(char* name, glm::mat4 matrix)
		{ glUniformmatrix4fv(glGetUniformLocation(_shaderID, name), 1, GL_FALSE, &matrix[0][0]); }


        inline void BindTexture2D(char* name, GLuint TextureID)
        {
            glActiveTexture(GL_TEXTURE0 + _textureLocation);
            glBindTexture(GL_TEXTURE_2D, TextureID);
            setUniform(name, _textureLocation++);
        }

};


#endif // SHADER_H_INCLUDED
>>>>>>> resman:Graphics/RShader.h
*/
