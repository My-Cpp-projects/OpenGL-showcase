



#ifndef MESH_H
#define MESH_H

#include "glfw/glfw3.h"
#include <glm/glm.hpp>

#include <string>
#include <vector>


namespace model_handling
{
	struct Vertex
	{
		glm::vec3 m_position;
		glm::vec3 m_normal;
		glm::vec2 m_texCoords;
		glm::vec3 m_tangent;
		glm::vec3 m_bitangent;
	};

	struct Texture
	{
		unsigned int m_id{};
		std::string m_type;
		std::string m_path;
	};

	class Mesh
	{
	public:
		Mesh(std::vector<Vertex> vertices,
			 std::vector<unsigned int> indices,
			 std::vector<Texture> textures);
		~Mesh();

		void draw(GLuint shaderProgramId) const;

	private:
		void setupMesh();

		enum bufferPositions
		{
			VERTEX = 0,
			INDEX,

			COUNT
		};
		std::vector<GLuint> m_buffers;
		std::vector<Vertex> m_vertices;
		std::vector<unsigned int> m_indices;
		std::vector<Texture> m_textures;
		unsigned int m_vertexArrayObject;
	};
}

#endif
