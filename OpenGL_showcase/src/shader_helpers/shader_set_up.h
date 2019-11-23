



#ifndef SHADER_LOADER_H
#define SHADER_LOADER_H

#include "GL/gl3w.h"
#include <string>

namespace shader
{
	namespace set_up
	{
		GLuint loadFromFile(const std::string& filePath, const GLenum& shaderType);
		GLuint loadFromString(const std::string& sourceCode, const GLenum& shaderType);
		GLuint linkProgramFromShaders(std::vector<GLuint>& shaders, bool delete_shaders);
	}
}

#endif /* SHADER_LOADER_H */
