



#ifndef SHADER_LOADER_H
#define SHADER_LOADER_H

#include "GL/gl3w.h"
#include "glfw/glfw3.h"
#include <string>
#include <vector>

namespace shader_handling
{
	GLuint loadFromString(const std::string& sourceCode, const GLenum& shaderType);

	GLuint loadFromFile(const std::string& filePath, const GLenum& shaderType);

	GLuint linkProgramFromShaders(std::vector<GLuint>& shaders, bool delete_shaders);
}

#endif /* SHADER_LOADER_H */
