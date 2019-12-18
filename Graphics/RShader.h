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
	SOLID_EMISSIVE_MATERIAL,
    CAR_PAINT_MATERIAL,
    MIRROR_MATERIAL,
    ALPHA_TEST_MATERIAL,
	PBR_MATERIAL,
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

    NUMBER_OF_SHADERS

};


class RShader : virtual public Resource
{
    friend class ResourceManager;

    private:
        GLuint _shaderID;
        int _textureLocation;

        void setNewShader(GLuint newId)
        {
            glDeleteProgram(_shaderID);
            _shaderID = newId;
        }

    public:
        RShader(std::string path, GLuint id)
            : Resource(RT_SHADER, path),  _shaderID(id), _textureLocation(0)
        {
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

        inline void setUniform(GLint location, int* values, int count = 1)
		{ glUniform1iv(location, count, values); }
		inline void setUniform(GLint location, float* values, int count = 1)
		{ glUniform1fv(location, count, values); }
		inline void setUniform(GLint location, int value)
		{ glUniform1i(location, value); }
		inline void setUniform(GLint location, float value)
		{ glUniform1f(location, value); }
		inline void setUniform(GLint location, glm::vec2* vectors, int count = 1)
		{ glUniform2fv(location, count, (GLfloat*)vectors); }
		inline void setUniform(GLint location, glm::vec3* vectors, int count = 1)
		{ glUniform3fv(location, count, (GLfloat*)vectors); }
		inline void setUniform(GLint location, glm::vec4* vectors, int count = 1)
		{ glUniform4fv(location, count, (GLfloat*)vectors); }
		inline void setUniform(GLint location, glm::vec2 vector)
		{ glUniform2f(location, vector.x, vector.y); }
		inline void setUniform(GLint location, glm::vec3 vector)
		{ glUniform3f(location, vector.x, vector.y, vector.z); }
		inline void setUniform(GLint location, glm::vec4 vector)
		{ glUniform4f(location, vector.x, vector.y, vector.z, vector.w); }
		inline void setUniform(GLint location, glm::mat3* matrices, int count = 1)
		{ glUniformMatrix4fv(location, count, GL_FALSE, (GLfloat*)matrices); }
		inline void setUniform(GLint location, glm::mat4* matrices, int count = 1)
		{ glUniformMatrix3fv(location, count, GL_FALSE, (GLfloat*)matrices); }
		inline void setUniform(GLint location, glm::mat3 matrix)
		{ glUniformMatrix3fv(location, 1, GL_FALSE, &matrix[0][0]); }
		inline void setUniform(GLint location, glm::mat4 matrix)
		{ glUniformMatrix4fv(location, 1, GL_FALSE, &matrix[0][0]); }


        inline void bindTexture(GLint location, RTexture* texture)
        {
            glActiveTexture(GL_TEXTURE0 + _textureLocation);
            texture->bind();
            setUniform(location, _textureLocation++);
        }

		inline GLint getUniformLocation(const char* name)
		{
			return glGetUniformLocation(_shaderID, name);
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
