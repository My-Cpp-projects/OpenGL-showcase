



#include "texture_handling/texture_loader.h"
#include "image_loading/stb_image.h"

#include <iostream>

namespace texture_loader
{
	GLuint getTextureFromFile(const std::string& path, const std::string& directory)
	{
		auto filename = directory + '/' + path;

		GLuint textureID;
		glGenTextures(1, &textureID);

		int width = -1;
		int height = -1;
		int nrComponents = -1;

		// OpenGL reads data from bottom to top
		stbi_set_flip_vertically_on_load(true);

		auto data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
		if(data)
		{
			GLenum format{};
			switch(nrComponents)
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
}