



#include "drawables/skybox.h"
#include "texture_handling/texture_loader.h"

namespace drawables
{
	Skybox::Skybox(const std::vector<std::string>& pathsToTextures)
	{
		m_cubemapTexture = texture_loader::loadCubeMap(pathsToTextures);

		glGenVertexArrays(1, &m_vao);
		glBindVertexArray(m_vao);
	}

	Skybox::~Skybox()
	{
		glDeleteBuffers(1, &m_vao);
	}

	void Skybox::draw(const GLuint& shaderProgramId) const
	{
		glUseProgram(shaderProgramId);
		glBindVertexArray(m_vao);
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_cubemapTexture);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		glBindVertexArray(0);
	}
}
