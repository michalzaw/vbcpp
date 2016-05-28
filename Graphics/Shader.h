#ifndef SHADER_H_INCLUDED
#define SHADER_H_INCLUDED

#include <GLEW/glew.h>
#include <glm/glm.hpp>


enum ShaderType
{
    SOLID_MATERIAL,
    NOTEXTURE_MATERIAL
};


class Shader
{
    private:
        GLuint _shaderID;

        int _textureLocation;

    public:
        Shader(GLuint ID) : _shaderID(ID), _textureLocation(0) {}
        ~Shader() {glDeleteProgram(_shaderID);}

        void Enable()
        {
            glUseProgram(_shaderID);

            _textureLocation = 0;
        }
        void Disable()
        {
            glUseProgram(0);
        }

        inline void SetUniform(char* Name, int* Values, int Count = 1)
		{ glUniform1iv(glGetUniformLocation(_shaderID, Name), Count, Values); }
		inline void SetUniform(char* Name, float* Values, int Count = 1)
		{ glUniform1fv(glGetUniformLocation(_shaderID, Name), Count, Values); }
		inline void SetUniform(char* Name, int Value)
		{ glUniform1i(glGetUniformLocation(_shaderID, Name), Value); }
		inline void SetUniform(char* Name, float Value)
		{ glUniform1f(glGetUniformLocation(_shaderID, Name), Value); }
		inline void SetUniform(char* Name, glm::vec2* Vectors, int Count = 1)
		{ glUniform2fv(glGetUniformLocation(_shaderID, Name), Count, (GLfloat*)Vectors); }
		inline void SetUniform(char* Name, glm::vec3* Vectors, int Count = 1)
		{ glUniform3fv(glGetUniformLocation(_shaderID, Name), Count, (GLfloat*)Vectors); }
		inline void SetUniform(char* Name, glm::vec4* Vectors, int Count = 1)
		{ glUniform4fv(glGetUniformLocation(_shaderID, Name), Count, (GLfloat*)Vectors); }
		inline void SetUniform(char* Name, glm::vec2 Vector)
		{ glUniform2f(glGetUniformLocation(_shaderID, Name), Vector.x, Vector.y); }
		inline void SetUniform(char* Name, glm::vec3 Vector)
		{ glUniform3f(glGetUniformLocation(_shaderID, Name), Vector.x, Vector.y, Vector.z); }
		inline void SetUniform(char* Name, glm::vec4 Vector)
		{ glUniform4f(glGetUniformLocation(_shaderID, Name), Vector.x, Vector.y, Vector.z, Vector.w); }
		inline void SetUniform(char* Name, glm::mat3* Matrices, int Count = 1)
		{ glUniformMatrix4fv(glGetUniformLocation(_shaderID, Name), Count, GL_FALSE, (GLfloat*)Matrices); }
		inline void SetUniform(char* Name, glm::mat4* Matrices, int Count = 1)
		{ glUniformMatrix3fv(glGetUniformLocation(_shaderID, Name), Count, GL_FALSE, (GLfloat*)Matrices); }
		inline void SetUniform(char* Name, glm::mat3 Matrix)
		{ glUniformMatrix3fv(glGetUniformLocation(_shaderID, Name), 1, GL_FALSE, &Matrix[0][0]); }
		inline void SetUniform(char* Name, glm::mat4 Matrix)
		{ glUniformMatrix4fv(glGetUniformLocation(_shaderID, Name), 1, GL_FALSE, &Matrix[0][0]); }


        inline void BindTexture2D(char* Name, GLuint TextureID)
        {
            glActiveTexture(GL_TEXTURE0 + _textureLocation);
            glBindTexture(GL_TEXTURE_2D, TextureID);
            SetUniform(Name, _textureLocation++);
        }

};


#endif // SHADER_H_INCLUDED
