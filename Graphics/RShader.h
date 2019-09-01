#ifndef SHADER_H_INCLUDED
#define SHADER_H_INCLUDED

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "../Utils/Resource.h"
#include "../Utils/RTexture.h"

#include <iostream>

enum ShaderType
{
    SOLID_MATERIAL,
    NOTEXTURE_MATERIAL,
    NORMALMAPPING_MATERIAL,
    CAR_PAINT_MATERIAL,
    MIRROR_MATERIAL,
    ALPHA_TEST_MATERIAL,
    TREE_MATERIAL,
    GRASS_MATERIAL,
    SKY_MATERIAL,
    GLASS_MATERIAL,

    GUI_IMAGE_SHADER,
    GUI_LABEL_SHADER,

    DEBUG_SHADER,
    EDITOR_AXIS_SHADER,

    SHADOWMAP_SHADER,
    SHADOWMAP_ALPHA_TEST_SHADER,

    MIRROR_SOLID_MATERIAL,
    MIRROR_ALPHA_TEST_MATERIAL,
    MIRROR_GLASS_MATERIAL,

    BLUR_SHADER_MSAA,
    BLUR_SHADER,
    QUAD_SHADER,

    NUMBER_OF_SHADERS

};


enum UniformName
{
    UNIFORM_MVP,
    UNIFORM_VP,
    UNIFORM_MODEL_MATRIX,
    UNIFORM_NORMAL_MATRIX,
    UNIFORM_DIFFUSE_TEXTURE,
    UNIFORM_NOTMALMAP_TEXTURE,
    UNIFORM_ALPHA_TEXTURE,
    UNIFORM_ENVIRONMENTMAP_TEXTURE,
    UNIFORM_ENVIRONMENTMAP_2_TEXTURE,
    UNIFORM_ENVIRONMENTMAP_MATRIX,
    UNIFORM_ENVIRONMENTMAP_2_MATRIX,
    UNIFORM_GLASS_TEXTURE,
    UNIFORM_MATERIAL_AMBIENT_COLOR,
    UNIFORM_MATERIAL_DIFFUSE_COLOR,
    UNIFORM_MATERIAL_SPECULAR_COLOR,
    UNIFORM_MATERIAL_EMISSIVE_COLOR,
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
    UNIFORM_DEBUG_VERTEX_INDEX_1,
    UNIFORM_DEBUG_VERTEX_INDEX_2,
    UNIFORM_DEBUG_VERTEX_INDEX_3,
    UNIFORM_DEBUG_VERTEX_INDEX_4,
    UNIFORM_DEBUG_VERTEX_INDEX_5,
    UNIFORM_DEBUG_VERTEX_INDEX_6,
    UNIFORM_DEBUG_VERTEX_INDEX_7,
    UNIFORM_DEBUG_VERTEX_INDEX_8,
    UNIFORM_BLUR_IS_HORIZONTAL,
    UNIFORM_BLOOM_TEXTURE,
    UNIFORM_BLOOM_RATIO,
    UNIFORM_TONEMAPPING_EXPOSURE,
	UNIFORM_POSTPROCESS_TEXTURE_1,
	UNIFORM_POSTPROCESS_TEXTURE_2,
	UNIFORM_POSTPROCESS_TEXTURE_3,
	UNIFORM_POSTPROCESS_TEXTURE_4,

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
            _uniformsNames[UNIFORM_VP] = "VP";
            _uniformsNames[UNIFORM_MODEL_MATRIX] = "ModelMatrix";
            _uniformsNames[UNIFORM_NORMAL_MATRIX] = "NormalMatrix";
            _uniformsNames[UNIFORM_DIFFUSE_TEXTURE] = "Texture";
            _uniformsNames[UNIFORM_NOTMALMAP_TEXTURE] = "NormalmapTexture";
            _uniformsNames[UNIFORM_ALPHA_TEXTURE] = "AlphaTexture";
            _uniformsNames[UNIFORM_ENVIRONMENTMAP_TEXTURE] = "environmentMap";
            _uniformsNames[UNIFORM_ENVIRONMENTMAP_2_TEXTURE] = "environmentMap2";
            _uniformsNames[UNIFORM_ENVIRONMENTMAP_MATRIX] = "environmentMap1Rotation";
            _uniformsNames[UNIFORM_ENVIRONMENTMAP_2_MATRIX] = "environmentMap2Rotation";
            _uniformsNames[UNIFORM_GLASS_TEXTURE] = "glassTexture";
            _uniformsNames[UNIFORM_MATERIAL_AMBIENT_COLOR] = "matAmbient";
            _uniformsNames[UNIFORM_MATERIAL_DIFFUSE_COLOR] = "matDiffuse";
            _uniformsNames[UNIFORM_MATERIAL_SPECULAR_COLOR] = "matSpecular";
            _uniformsNames[UNIFORM_MATERIAL_EMISSIVE_COLOR] = "matEmissive";
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
            _uniformsNames[UNIFORM_DEBUG_VERTEX_INDEX_1] = "indices[0]";
            _uniformsNames[UNIFORM_DEBUG_VERTEX_INDEX_2] = "indices[1]";
            _uniformsNames[UNIFORM_DEBUG_VERTEX_INDEX_3] = "indices[2]";
            _uniformsNames[UNIFORM_DEBUG_VERTEX_INDEX_4] = "indices[3]";
            _uniformsNames[UNIFORM_DEBUG_VERTEX_INDEX_5] = "indices[4]";
            _uniformsNames[UNIFORM_DEBUG_VERTEX_INDEX_6] = "indices[5]";
            _uniformsNames[UNIFORM_DEBUG_VERTEX_INDEX_7] = "indices[6]";
            _uniformsNames[UNIFORM_DEBUG_VERTEX_INDEX_8] = "indices[7]";
            _uniformsNames[UNIFORM_BLUR_IS_HORIZONTAL] = "isHorizontal";
            _uniformsNames[UNIFORM_BLOOM_TEXTURE] = "BloomTexture";
            _uniformsNames[UNIFORM_BLOOM_RATIO] = "BloomRatio";
            _uniformsNames[UNIFORM_TONEMAPPING_EXPOSURE] = "exposure";
			_uniformsNames[UNIFORM_POSTPROCESS_TEXTURE_1] = "texture1";
			_uniformsNames[UNIFORM_POSTPROCESS_TEXTURE_2] = "texture2";
			_uniformsNames[UNIFORM_POSTPROCESS_TEXTURE_3] = "texture3";
			_uniformsNames[UNIFORM_POSTPROCESS_TEXTURE_4] = "texture4";

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
            GLuint* uniformIndices = new GLuint[count];
            glGetUniformIndices(_shaderID, count, names, uniformIndices);

            glGetActiveUniformsiv(_shaderID, count, uniformIndices, GL_UNIFORM_OFFSET, offsets);

			delete[] uniformIndices;
        }

        inline void getUniformArrayStride(const char** names, unsigned int count, int* arrayStrides)
        {
            GLuint* uniformIndices = new GLuint[count];
            glGetUniformIndices(_shaderID, count, names, uniformIndices);

            glGetActiveUniformsiv(_shaderID, count, uniformIndices, GL_UNIFORM_ARRAY_STRIDE, arrayStrides);

			delete[] uniformIndices;
        }

        inline void getUniformMatrixStride(const char** names, unsigned int count, int* matrixStrides)
        {
            GLuint* uniformIndices = new GLuint[count];
            glGetUniformIndices(_shaderID, count, names, uniformIndices);

            glGetActiveUniformsiv(_shaderID, count, uniformIndices, GL_UNIFORM_MATRIX_STRIDE, matrixStrides);

			delete[] uniformIndices;
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
