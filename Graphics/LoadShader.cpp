#include "LoadShader.h"


GLuint LoadShader(const char* VertexShaderFileName, const char* FragmentShaderFileName)
{
	GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	std::ifstream File;


	File.open(VertexShaderFileName);
	std::string VertexShaderCode;
	if (File.is_open())
	{
		std::string Line;
		while (std::getline(File, Line))
		{
			VertexShaderCode += "\n" + Line;
		}

		File.close();
	}
	else
	{
		std::cout << "Can not open VertexShaderFile: " << VertexShaderFileName << "\n!";

		File.close();

		return 0;
	}


	File.open(FragmentShaderFileName);
	std::string FragmentShaderCode;
	if (File.is_open())
	{
		std::string Line;
		while (std::getline(File, Line))
		{
			FragmentShaderCode += "\n" + Line;
		}

		File.close();
	}
	else
	{
		std::cout << "Can not open FragmentShaderFile: " << VertexShaderFileName << "\n!";

		File.close();

		return 0;
	}


	GLint Result;
	GLint LogLength;


	std::cout << "Compiling shader: " << VertexShaderFileName << "\n";

	char const* VertexShaderSource = VertexShaderCode.c_str();
	glShaderSource(VertexShaderID, 1, &VertexShaderSource, NULL);
	glCompileShader(VertexShaderID);

	glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &LogLength);
	if (LogLength > 0)
	{
		std::vector<char> VertexShaderCompileLog(LogLength + 1);
		glGetShaderInfoLog(VertexShaderID, LogLength, NULL, &VertexShaderCompileLog[0]);
		std::cout << &VertexShaderCompileLog[0] << std::endl;
	}


	std::cout << "Compiling shader: " << FragmentShaderFileName << "\n";

	char const* FragmentShaderSource = FragmentShaderCode.c_str();
	glShaderSource(FragmentShaderID, 1, &FragmentShaderSource, NULL);
	glCompileShader(FragmentShaderID);

	glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &LogLength);
	if (LogLength > 0)
	{
		std::vector<char> FragmentShaderCompileLog(LogLength + 1);
		glGetShaderInfoLog(FragmentShaderID, LogLength, NULL, &FragmentShaderCompileLog[0]);
		std::cout << &FragmentShaderCompileLog[0] << std::endl;
	}


	std::cout << "Linking program\n";

	GLuint ProgramID = glCreateProgram();
	glAttachShader(ProgramID, VertexShaderID);
	glAttachShader(ProgramID, FragmentShaderID);
	glLinkProgram(ProgramID);

	glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
	glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &LogLength);
	if (LogLength > 0)
	{
		std::vector<char> LinkingProgramLog(LogLength + 1);
		glGetProgramInfoLog(ProgramID, LogLength, NULL, &LinkingProgramLog[0]);
		std::cout << &LinkingProgramLog[0] << std::endl;
	}


	glDeleteShader(VertexShaderID);
	glDeleteShader(FragmentShaderID);


	return ProgramID;
}
