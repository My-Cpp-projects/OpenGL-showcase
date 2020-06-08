



#include "shader_handling/shader_modify.h"

namespace shader_handling
{
	void setBool(GLuint programId, const std::string& name, bool value)
	{
		glUniform1i(glGetUniformLocation(programId, name.c_str()), (int)value);
	}
		
	void setInt(GLuint programId, const std::string& name, int value)
	{
		glUniform1i(glGetUniformLocation(programId, name.c_str()), value);
	}
		
	void setFloat(GLuint programId, const std::string& name, float value)
	{
		glUniform1f(glGetUniformLocation(programId, name.c_str()), value);
	}
		
	void setVec2(GLuint programId, const std::string& name, const glm::vec2& value)
	{
		glUniform2fv(glGetUniformLocation(programId, name.c_str()), 1, &value[0]);
	}

	void setVec2(GLuint programId, const std::string& name, float x, float y)
	{
		glUniform2f(glGetUniformLocation(programId, name.c_str()), x, y);
	}
		
	void setVec3(GLuint programId, const std::string& name, const glm::vec3& value)
	{
		glUniform3fv(glGetUniformLocation(programId, name.c_str()), 1, &value[0]);
	}

	void setVec3(GLuint programId, const std::string& name, float x, float y, float z)
	{
		glUniform3f(glGetUniformLocation(programId, name.c_str()), x, y, z);
	}
		
	void setVec4(GLuint programId, const std::string& name, const glm::vec4& value)
	{
		glUniform4fv(glGetUniformLocation(programId, name.c_str()), 1, &value[0]);
	}

	void setVec4(GLuint programId, const std::string& name, float x, float y, float z, float w)
	{
		glUniform4f(glGetUniformLocation(programId, name.c_str()), x, y, z, w);
	}
		
	void setMat2(GLuint programId, const std::string& name, const glm::mat2& mat)
	{
		glUniformMatrix2fv(glGetUniformLocation(programId, name.c_str()), 1, GL_FALSE, &mat[0][0]);
	}
		
	void setMat3(GLuint programId, const std::string& name, const glm::mat3& mat)
	{
		glUniformMatrix3fv(glGetUniformLocation(programId, name.c_str()), 1, GL_FALSE, &mat[0][0]);
	}
		
	void setMat4(GLuint programId, const std::string& name, const glm::mat4& mat)
	{
		glUniformMatrix4fv(glGetUniformLocation(programId, name.c_str()), 1, GL_FALSE, &mat[0][0]);
	}
}