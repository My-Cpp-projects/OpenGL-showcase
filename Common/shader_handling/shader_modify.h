



#ifndef SHADER_MODIFY_H
#define SHADER_MODIFY_H

#include "GL/gl3w.h"
#include <string>
#include <glm/glm.hpp>

namespace shader
{
	namespace modify
	{
		void setBool(GLuint programId, const std::string& name, bool value);
		void setInt(GLuint programId, const std::string& name, int value);
		void setFloat(GLuint programId, const std::string& name, float value);

		void setVec2(GLuint programId, const std::string& name, const glm::vec2& value);
		void setVec2(GLuint programId, const std::string& name, float x, float y);

		void setVec3(GLuint programId, const std::string& name, const glm::vec3& value);
		void setVec3(GLuint programId, const std::string& name, float x, float y, float z);

		void setVec4(GLuint programId, const std::string& name, const glm::vec4& value);
		void setVec4(GLuint programId, const std::string& name, float x, float y, float z, float w);

		void setMat2(GLuint programId, const std::string& name, const glm::mat2& mat);
		void setMat3(GLuint programId, const std::string& name, const glm::mat3& mat);
		void setMat4(GLuint programId, const std::string& name, const glm::mat4& mat);
		
	}
}

#endif /* SHADER_MODIFY_H */
