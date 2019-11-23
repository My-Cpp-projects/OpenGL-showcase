



#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include "shader_set_up.h"

std::string shaderTypeToString(GLenum shaderType);
std::string extractFileContent(const std::string& filePath);

namespace shader
{
	namespace set_up
	{
		GLuint loadFromFile(const std::string& filePath, const GLenum& shaderType)
		{
			GLuint result = 0;

			std::string fileContent = extractFileContent(filePath);
			if(fileContent.empty())
			{
				printf("Shader file content is empty. filePath: %s\n", filePath.c_str());
				return result;
			}

			result = loadFromString(fileContent, shaderType);

			if(0 == result)
			{
				printf("Failed to compile shader: %s\n", filePath.c_str());
			}

			return result;
		}

		GLuint loadFromString(const std::string& sourceCode, const GLenum& shaderType)
		{
			GLuint result = glCreateShader(shaderType);

			const GLchar* const s = sourceCode.c_str();
			glShaderSource(result, 1, &s, nullptr);

			glCompileShader(result);

			GLint status = 0;
			glGetShaderiv(result, GL_COMPILE_STATUS, &status);

			if(!status)
			{
				const short logSize = 4096;
				char buffer[logSize];
				glGetShaderInfoLog(result, logSize, nullptr, buffer);
				printf("Didn't manage to compile shader of type: %s, error: %s ",
					   shaderTypeToString(shaderType).c_str(),
					   buffer);
				glDeleteShader(result);
				return 0;
			}

			return result;
		}

		GLuint linkProgramFromShaders(std::vector<GLuint>& shaders, bool delete_shaders)
		{
			GLuint program = glCreateProgram();

			for(auto& shader : shaders)
			{
				glAttachShader(program, shader);
			}

			glLinkProgram(program);

			GLint status;
			glGetProgramiv(program, GL_LINK_STATUS, &status);

			if(!status)
			{
				const short logSize = 4096;
				char buffer[logSize];
				glGetProgramInfoLog(program, logSize, nullptr, buffer);
				glDeleteProgram(program);
				printf("Didn't manage to compile shaders into program. Reason:\n%s", buffer);
				return 0;
			}

			if(delete_shaders)
			{
				for(auto& shader : shaders)
				{
					glDeleteShader(shader);
				}
			}

			return program;
		}
	}
}

std::string shaderTypeToString(GLenum shaderType)
{
	std::string retVal{};
	switch (shaderType)
	{
		case GL_VERTEX_SHADER:
			retVal = "GL_VERTEX_SHADER";
			break;
		case GL_FRAGMENT_SHADER:
			retVal = "GL_FRAGMENT_SHADER";
			break;
		case GL_COMPUTE_SHADER:
			retVal = "GL_COMPUTE_SHADER";
			break;		
		case GL_TESS_CONTROL_SHADER:
			retVal = "GL_TESS_CONTROL_SHADER";
			break;		
		case GL_TESS_EVALUATION_SHADER:
			retVal = "GL_TESS_EVALUATION_SHADER";
			break;	
		case GL_GEOMETRY_SHADER:
			retVal = "GL_GEOMETRY_SHADER";
			break;
		default:
			retVal = "INVALID_SHADER_TYPE";
			break;
	}

	return retVal;
}

std::string extractFileContent(const std::string& filePath)
{
	std::ifstream file;

	file.open(filePath, std::ios::in);
	if(not file)
	{
		printf("File cannot be open/found: : %s\nReturning empty string\n", filePath.c_str());
		return std::string{};
	}

	// Retrieve contents of file
	std::stringstream fileStream;
	fileStream << file.rdbuf();
	file.close();
	std::string fileContent = fileStream.str();

	return fileContent;
}