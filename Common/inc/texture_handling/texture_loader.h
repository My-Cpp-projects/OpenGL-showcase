



#ifndef TEXTURE_LOADER
#define TEXTURE_LOADER

#include <string>
#include "GL/gl3w.h"

namespace texture_loader
{
	GLuint getTextureFromFile(const std::string& path,
							  const std::string& directory);
}

#endif // !TEXTURE_LOADER

