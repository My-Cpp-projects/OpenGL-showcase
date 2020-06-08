



#ifndef CUBE_SHOWCASE_h
#define CUBE_SHOWCASE_h

#include "scene/showcase_base.h"

#include "GL/gl3w.h"
#include "glfw/glfw3.h"
#include <glm/glm.hpp>
#include <string>
#include <vector>
#include <memory>

namespace model_handling
{
	class Model;
}

namespace shader_handling
{
	class ShaderProgram;
}

class CubeShowcase : public ShowcaseBase
{
public:
	CubeShowcase(const std::string& sceneName);
	virtual ~CubeShowcase();
	void run() override;

private:
	virtual void setupWindow() override;
	virtual void loadModels() override;
	virtual void setupInitialPositions() override;
	virtual void loadShaders() override;
	virtual void setupShaders() override;
	
	void cleanup();

	enum shaderPrograms
	{
		CUBE = 0,
		LIGHT,
		
		SHADER_COUNT
	};

	enum buffer
	{
		// UBO-s
		VP_MATRICES = 0,
		SUN_LIGHT,

		BUFFER_COUNT
	};

	std::string m_showcaseName;
	GLFWwindow* m_window;

	std::vector<std::unique_ptr<shader_handling::ShaderProgram>> m_shaderPrograms;
	std::vector<GLuint> m_buffers;

	std::vector<glm::vec3> m_cubePositions;
	glm::vec3 m_sunPosition;
	glm::vec3 m_pointLightPosition;
	glm::vec3 m_lightColor;
	glm::vec3 m_sunlightDir;

	std::unique_ptr<model_handling::Model> m_cube;
	std::unique_ptr<model_handling::Model> m_sun;
};

#endif // !CUBE_SHOWCASE_h
