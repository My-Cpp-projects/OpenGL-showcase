



#include "drawables/model.h"
#include "model_handling/model_loader.h"

#include "gl3w/GL/gl3w.h"
#include "glfw/glfw3.h"

namespace drawables
{
	Model::Model(const std::string& path)
	{
		model_handling::ModelLoader loader;
		m_meshes = loader.loadModel(path);
	}

	void Model::draw(const GLuint& shaderProgramId) const
	{
		for(const auto& mesh : m_meshes)
			mesh->draw(shaderProgramId);
	}
}
