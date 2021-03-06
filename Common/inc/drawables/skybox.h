



#ifndef SKYBOX_H
#define SKYBOX_H

#include "abstract_drawable.h"

#include "GL/gl3w.h"
#include <string>
#include <vector>

namespace drawables
{
	class Skybox : public AbstractDrawable
	{
	public:
		Skybox(const std::vector<std::string>& pathsToTextures);
		virtual ~Skybox();

		virtual void draw(const GLuint& shaderProgramId) const;

	private:
		GLuint m_cubemapTexture;
		GLuint m_vao;
	};
}

#endif // !SKYBOX_H
