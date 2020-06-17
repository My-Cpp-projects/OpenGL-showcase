



#include "texture_handling/texture_loader.h"
#include "image_loading/stb_image.h"

#include <iostream>

namespace texture_loader
{
	GLuint getTextureFromFile(const std::string& path,
							  const std::string& directory)
	{
		auto filename = directory + '/' + path;

		GLuint textureID;
		glGenTextures(1, &textureID);

		int width = -1;
		int height = -1;
		int nrChannels = -1;

		// OpenGL reads data from bottom to top
		stbi_set_flip_vertically_on_load(true);

		auto data = stbi_load(filename.c_str(), &width, &height, &nrChannels, 0);
		if(data)
		{
			GLenum format{};
			switch(nrChannels)
			{
				case 1:
					format = GL_RED;
					break;
				case 3:
					format = GL_RGB;
					break;
				case 4:
					format = GL_RGBA;
					break;
			}

			glBindTexture(GL_TEXTURE_2D, textureID);
			glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		}
		else
		{
			std::cerr << "Texture failed to load at path: " << path << std::endl;
		}

		stbi_image_free(data);
		return textureID;
	}

	GLuint loadCubeMap(const std::vector<std::string>& pathsToTextures)
	{
		stbi_set_flip_vertically_on_load(false);

		unsigned int textureID;
		glGenTextures(1, &textureID);
		glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

		int width = -1;
		int height = -1;
		int nrChannels = -1;
		for(size_t i = 0; i < pathsToTextures.size(); ++i)
		{
			auto data = stbi_load(pathsToTextures[i].c_str(), &width, &height, &nrChannels, 0);
			if(data)
			{
				GLenum format{};
				switch(nrChannels)
				{
					case 1:
						format = GL_RED;
						break;
					case 3:
						format = GL_RGB;
						break;
					case 4:
						format = GL_RGBA;
						break;
				}

				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
			}
			else
			{
				std::cerr << "Cubemap texture failed to load at path: " << pathsToTextures[i] << std::endl;
			}
			stbi_image_free(data);
		}

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

		return textureID;
	}
}