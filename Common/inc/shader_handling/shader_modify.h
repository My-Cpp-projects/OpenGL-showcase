



#ifndef SHADER_MODIFY_H
#define SHADER_MODIFY_H

#include "GL/gl3w.h"
#include <glm/glm.hpp>
#include <string>

namespace shader_handling
{
	void setBool(GLuint programId, const std::string& name, bool value);
	void setInt(GLuint programId, const std::string& name, int value);
	void setFloat(GLuint programId, const std::string& name, float value);

	void setVec2(GLuint programId, const std::string& name, const glm::vec2& value);
	void setVec3(GLuint programId, const std::string& name, const glm::vec3& value);
	void setVec4(GLuint programId, const std::string& name, const glm::vec4& value);

	void setMat2(GLuint programId, const std::string& name, const glm::mat2& mat);
	void setMat3(GLuint programId, const std::string& name, const glm::mat3& mat);
	void setMat4(GLuint programId, const std::string& name, const glm::mat4& mat);
}

#endif /* SHADER_MODIFY_H */
