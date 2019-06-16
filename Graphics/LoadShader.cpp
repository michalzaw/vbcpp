#include "LoadShader.h"

#include <sstream>


bool ShaderLoader::loadShaderCode(const char* fileName, std::string& code, const std::vector<std::string>& defines)
{
    std::ifstream file;

    file.open(fileName);
    if (file.is_open())
    {
        std::string line;
        bool mode = true;
        while (std::getline(file, line))
        {
            if (line.find("#include") == 0)
            {
                if (mode)
                {
                    int pos = line.find("\"") + 1;
                    int pos2 = line.rfind("\"");

                    std::string includedFileName = line.substr(pos, pos2 - pos);

                    std::string fileNameStr(fileName);
                    pos = fileNameStr.rfind("/");
                    std::string path = fileNameStr.substr(0, pos + 1) + includedFileName;

                    if (!loadShaderCode(path.c_str(), code, defines))
                    {
                        std::cout << "Can not open included shader file: " << path << "!\n";
                    }
                }
            }
            else if (line.find("#ifdef") == 0)
            {
                std::string def = line.substr(7, line.length() - 7);

                mode = false;
                for (int i = 0; i < defines.size(); ++i)
                {
                    if (defines[i] == def)
                    {
                        mode = true;
                        break;
                    }
                }
            }
            else if (line.find("#endif") == 0)
            {
                    mode = true;
            }
            else
            {
                if (mode)
                    code += "\n" + line;
            }
        }

        file.close();
    }
    else
    {
        file.close();

        return false;
    }

    //std::cout << code << std::endl;

    return true;
}


GLuint ShaderLoader::compileShader(ShaderType type, std::string& code)
{
    GLuint shaderID = glCreateShader(type);

	GLint result;
	GLint logLength;

	char const* shaderSource = code.c_str();
	glShaderSource(shaderID, 1, &shaderSource, NULL);
	glCompileShader(shaderID);

	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &result);
	glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &logLength);
	if (logLength > 0)
	{
		std::vector<char> shaderCompileLog(logLength + 1);
		glGetShaderInfoLog(shaderID, logLength, NULL, &shaderCompileLog[0]);
		std::cout << &shaderCompileLog[0] << std::endl;
	}

	return shaderID;
}


GLuint ShaderLoader::linkProgram(GLuint vertexShaderId, GLuint fragmentShaderId)
{
    GLuint programId = glCreateProgram();
	glAttachShader(programId, vertexShaderId);
	glAttachShader(programId, fragmentShaderId);
	glLinkProgram(programId);

	GLint result;
	GLint logLength;

	glGetProgramiv(programId, GL_LINK_STATUS, &result);
	glGetProgramiv(programId, GL_INFO_LOG_LENGTH, &logLength);
	if (logLength > 0)
	{
		std::vector<char> linkingProgramLog(logLength + 1);
		glGetProgramInfoLog(programId, logLength, NULL, &linkingProgramLog[0]);
		std::cout << &linkingProgramLog[0] << std::endl;
	}

	return programId;
}


GLuint ShaderLoader::loadShader(const char* vertexShaderFileName, const char* fragmentShaderFileName, const std::vector<std::string>& defines)
{
	std::string vertexShaderCode;
	std::string fragmentShaderCode;

	if (!loadShaderCode(vertexShaderFileName, vertexShaderCode, defines))
    {
        std::cout << "Can not open VertexShaderFile: " << vertexShaderFileName << "!\n";
    }
    if (!loadShaderCode(fragmentShaderFileName, fragmentShaderCode, defines))
    {
        std::cout << "Can not open FragmentShaderFile: " << fragmentShaderFileName << "!\n";
    }


	std::cout << "Compiling shader: " << vertexShaderFileName << "\n";
	GLuint vertexShaderId = compileShader(ST_VERTEX_SHADER, vertexShaderCode);

	std::cout << "Compiling shader: " << fragmentShaderFileName << "\n";
	GLuint fragmentShaderId = compileShader(ST_FRAGMENT_SHADER, fragmentShaderCode);


	std::cout << "Linking program\n";
    GLuint programId = linkProgram(vertexShaderId, fragmentShaderId);


	glDeleteShader(vertexShaderId);
	glDeleteShader(fragmentShaderId);


	return programId;
}
