



#ifndef SHADER_PROGRAM_H
#define SHADER_PROGRAM_H

#include "GL/gl3w.h"
#include "glfw/glfw3.h"
#include <glm/glm.hpp>
#include <string>
#include <vector>

namespace shader_handling
{
	class ShaderProgram
	{
	public:
		ShaderProgram();
		virtual ~ShaderProgram();

		ShaderProgram& attachShader(const std::string& pathToShader, const GLenum& shaderType);
		void linkProgram(bool deleteShaders = true);

		GLuint getProgramId() const;
		void use() const;

		ShaderProgram& setBool(const std::string& name, bool value);
		ShaderProgram& setInt(const std::string& name, int value);
		ShaderProgram& setFloat(const std::string& name, float value);

		ShaderProgram& setVec2(const std::string& name, const glm::vec2& value);
		ShaderProgram& setVec2(const std::string& name, float x, float y);

		ShaderProgram& setVec3(const std::string& name, const glm::vec3& value);
		ShaderProgram& setVec3(const std::string& name, float x, float y, float z);

		ShaderProgram& setVec4(const std::string& name, const glm::vec4& value);
		ShaderProgram& setVec4(const std::string& name, float x, float y, float z, float w);

		ShaderProgram& setMat2(const std::string& name, const glm::mat2& mat);
		ShaderProgram& setMat3(const std::string& name, const glm::mat3& mat);
		ShaderProgram& setMat4(const std::string& name, const glm::mat4& mat);

	private:
		GLuint m_programId;
		std::vector<GLuint> m_shaders;
	};
}

#endif // !SHADER_PROGRAM_H

