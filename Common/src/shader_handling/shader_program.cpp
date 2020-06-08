



#include "shader_handling/shader_program.h"
#include "shader_handling/shader_set_up.h"
#include "shader_handling/shader_modify.h"

namespace shader_handling
{
	ShaderProgram::ShaderProgram()
		: m_programId(-1)
	{

	}

	ShaderProgram::~ShaderProgram()
	{
		glDeleteProgram(m_programId);
	}

	ShaderProgram& ShaderProgram::attachShader(const std::string& pathToShader, const GLenum& shaderType)
	{
		m_shaders.emplace_back(shader_handling::loadFromFile(pathToShader, shaderType));
		return *this;
	}

	void ShaderProgram::linkProgram(bool deleteShaders)
	{
		m_programId = shader_handling::linkProgramFromShaders(m_shaders, deleteShaders);
		if(deleteShaders)
			m_shaders.clear();
	}

	GLuint ShaderProgram::getProgramId() const
	{
		return m_programId;
	}

	ShaderProgram& ShaderProgram::setBool(const std::string& name, bool value)
	{
		shader_handling::setBool(m_programId, name, value);
		return *this;
	}

	ShaderProgram& ShaderProgram::setInt(const std::string& name, int value)
	{
		shader_handling::setInt(m_programId, name, value);
		return *this;
	}

	ShaderProgram& ShaderProgram::setFloat(const std::string& name, float value)
	{
		shader_handling::setFloat(m_programId, name, value);
		return *this;
	}

	ShaderProgram& ShaderProgram::setVec2(const std::string& name, const glm::vec2& value)
	{
		shader_handling::setVec2(m_programId, name, value);
		return *this;
	}

	ShaderProgram& ShaderProgram::setVec2(const std::string& name, float x, float y)
	{
		shader_handling::setVec2(m_programId, name, x, y);
		return *this;
	}

	ShaderProgram& ShaderProgram::setVec3(const std::string& name, const glm::vec3& value)
	{
		shader_handling::setVec3(m_programId, name, value);
		return *this;
	}

	ShaderProgram& ShaderProgram::setVec3(const std::string& name, float x, float y, float z)
	{
		shader_handling::setVec3(m_programId, name, x, y, z);
		return *this;
	}

	ShaderProgram& ShaderProgram::setVec4(const std::string& name, const glm::vec4& value)
	{
		shader_handling::setVec4(m_programId, name, value);
		return *this;
	}

	ShaderProgram& ShaderProgram::setVec4(const std::string& name, float x, float y, float z, float w)
	{
		shader_handling::setVec4(m_programId, name, x, y, z, w);
		return *this;
	}

	ShaderProgram& ShaderProgram::setMat2(const std::string& name, const glm::mat2& mat)
	{
		shader_handling::setMat2(m_programId, name, mat);
		return *this;
	}

	ShaderProgram& ShaderProgram::setMat3(const std::string& name, const glm::mat3& mat)
	{
		shader_handling::setMat3(m_programId, name, mat);
		return *this;
	}

	ShaderProgram& ShaderProgram::setMat4(const std::string& name, const glm::mat4& mat)
	{
		shader_handling::setMat4(m_programId, name, mat);
		return *this;
	}
}
