



#ifndef TEXTURE_LOADER
#define TEXTURE_LOADER

#include "GL/gl3w.h"
#include <string>

namespace texture_loader
{
	GLuint getTextureFromFile(const std::string& path,
							  const std::string& directory);
}

#endif // !TEXTURE_LOADER

