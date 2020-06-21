



#include "shader_handling/shader_modify.h"

namespace shader_handling
{
	void setBool(GLuint programId, const std::string& name, bool value)
	{
		glProgramUniform1i(programId, glGetUniformLocation(programId, name.c_str()), (int)value);
	}
		
	void setInt(GLuint programId, const std::string& name, int value)
	{
		glProgramUniform1i(programId, glGetUniformLocation(programId, name.c_str()), value);
	}
		
	void setFloat(GLuint programId, const std::string& name, float value)
	{
		glProgramUniform1f(programId, glGetUniformLocation(programId, name.c_str()), value);
	}
		
	void setVec2(GLuint programId, const std::string& name, const glm::vec2& value)
	{
		glProgramUniform2fv(programId, glGetUniformLocation(programId, name.c_str()), 1, &value[0]);
	}
		
	void setVec3(GLuint programId, const std::string& name, const glm::vec3& value)
	{
		glProgramUniform3fv(programId, glGetUniformLocation(programId, name.c_str()), 1, &value[0]);
	}
		
	void setVec4(GLuint programId, const std::string& name, const glm::vec4& value)
	{
		glProgramUniform4fv(programId, glGetUniformLocation(programId, name.c_str()), 1, &value[0]);
	}
		
	void setMat2(GLuint programId, const std::string& name, const glm::mat2& mat)
	{
		glProgramUniformMatrix2fv(programId, glGetUniformLocation(programId, name.c_str()), 1, GL_FALSE, &mat[0][0]);
	}
		
	void setMat3(GLuint programId, const std::string& name, const glm::mat3& mat)
	{
		glProgramUniformMatrix3fv(programId, glGetUniformLocation(programId, name.c_str()), 1, GL_FALSE, &mat[0][0]);
	}
		
	void setMat4(GLuint programId, const std::string& name, const glm::mat4& mat)
	{
		glProgramUniformMatrix4fv(programId, glGetUniformLocation(programId, name.c_str()), 1, GL_FALSE, &mat[0][0]);
	}
}