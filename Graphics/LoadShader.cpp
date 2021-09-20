#include "LoadShader.h"

#include <sstream>

#include "../Utils/Logger2.h"


std::string ShaderLoader::replaceConstatnsInLine(std::string line, const std::unordered_map<std::string, std::string>& constants)
{
    for (std::pair<std::string, std::string> element : constants)
    {
        const std::string& constName = "{" + element.first + "}";
        const std::string& constValue = element.second;

        size_t pos = line.find(constName);
        if (pos != std::string::npos)
        {
            line.replace(pos, constName.size(), constValue);
        }
    }

    return line;
}


bool ShaderLoader::loadShaderCode(const char* fileName, std::string& code, const std::vector<std::string>& defines, const std::unordered_map<std::string, std::string>& constants)
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

                    if (!loadShaderCode(path.c_str(), code, defines, constants))
                    {
                        LOG_ERROR("Can not open included shader file: " + path + "!");
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
                {
                    line = replaceConstatnsInLine(line, constants);
                    code += "\n" + line;
                }
            }
        }

        file.close();
    }
    else
    {
        file.close();

        return false;
    }

    //LOG_DEBUG(code);

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
        LOG_INFO(&shaderCompileLog[0]);
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
        LOG_INFO(&linkingProgramLog[0]);
	}

	return programId;
}


GLuint ShaderLoader::loadShader(const char* vertexShaderFileName, const char* fragmentShaderFileName, const std::vector<std::string>& defines,
                                const std::unordered_map<std::string, std::string>& constants)
{
	std::string vertexShaderCode;
	std::string fragmentShaderCode;

	if (!loadShaderCode(vertexShaderFileName, vertexShaderCode, defines, constants))
    {
        LOG_ERROR("Can not open VertexShaderFile: " + std::string(vertexShaderFileName) + "!");
    }
    if (!loadShaderCode(fragmentShaderFileName, fragmentShaderCode, defines, constants))
    {
        LOG_ERROR("Can not open FragmentShaderFile: " + std::string(fragmentShaderFileName) + "!");
    }


    LOG_INFO("Compiling shader: " + std::string(vertexShaderFileName));
	GLuint vertexShaderId = compileShader(ST_VERTEX_SHADER, vertexShaderCode);

    LOG_INFO("Compiling shader: " + std::string(fragmentShaderFileName));
	GLuint fragmentShaderId = compileShader(ST_FRAGMENT_SHADER, fragmentShaderCode);


    LOG_INFO("Linking program");
    GLuint programId = linkProgram(vertexShaderId, fragmentShaderId);


	glDeleteShader(vertexShaderId);
	glDeleteShader(fragmentShaderId);


	return programId;
}
