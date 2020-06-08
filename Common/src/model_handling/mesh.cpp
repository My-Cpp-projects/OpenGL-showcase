



#include <glm/gtc/matrix_transform.hpp>
#include "gl3w/GL/gl3w.h"

#include "model_handling/mesh.h"
#include "shader_handling/shader_modify.h"

namespace model_handling
{
	Mesh::Mesh(std::vector<Vertex> vertices,
			   std::vector<unsigned int> indices,
			   std::vector<Texture> textures)
		: m_vertices(vertices)
		, m_indices(indices)
		, m_textures(textures)
		, m_buffers(COUNT)
	{
		setupMesh();
	}

	Mesh::~Mesh()
	{
		glDeleteBuffers(m_buffers.size(), &m_buffers.front());
		glDeleteVertexArrays(1, &m_vertexArrayObject);
	}

	void Mesh::draw(GLuint shaderProgramId) const
	{
		unsigned int diffuseNr = 1;
		unsigned int specularNr = 1;
		unsigned int normalNr = 1;
		unsigned int heightNr = 1;

		for(unsigned int i = 0; i < m_textures.size(); ++i)
		{
			glActiveTexture(GL_TEXTURE0 + i);
			// retrieve texture number (the N in diffuse_textureN)
			std::string number;
			auto name = m_textures[i].m_type;
			if(name == "texture_diffuse")
				number = std::to_string(diffuseNr++);
			else if(name == "texture_specular")
				number = std::to_string(specularNr++);
			else if(name == "texture_normal")
				number = std::to_string(normalNr++);
			else if(name == "texture_height")
				number = std::to_string(heightNr++);

			shader::modify::setInt(shaderProgramId, "material." + name + number, i);
			glBindTexture(GL_TEXTURE_2D, m_textures[i].m_id);
		}

		glBindVertexArray(m_vertexArrayObject);
		glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		glActiveTexture(GL_TEXTURE0);
	}

	void Mesh::setupMesh()
	{
		glGenVertexArrays(1, &m_vertexArrayObject);
		glGenBuffers(m_buffers.size(), &m_buffers.front());

		glBindVertexArray(m_vertexArrayObject);

		glBindBuffer(GL_ARRAY_BUFFER, m_buffers[bufferPositions::VERTEX]);
		glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(Vertex), &m_vertices[0], GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_buffers[bufferPositions::INDEX]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(unsigned int), &m_indices[0], GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), nullptr);

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, m_normal));

		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, m_texCoords));

		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, m_tangent));

		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, m_bitangent));

		glBindVertexArray(0);
	}
}