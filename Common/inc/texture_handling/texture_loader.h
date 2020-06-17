



#ifndef TEXTURE_LOADER
#define TEXTURE_LOADER

#include "GL/gl3w.h"
#include <string>
#include <vector>

namespace texture_loader
{
	GLuint getTextureFromFile(const std::string& path,
							  const std::string& directory);

	GLuint loadCubeMap(const std::vector<std::string>& pathsToTextures);
}

#endif // !TEXTURE_LOADER

