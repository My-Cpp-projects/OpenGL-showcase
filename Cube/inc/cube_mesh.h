



#ifndef CUBE_MESH
#define CUBE_MESH

#include <vector>
#include "gl3w/GL/gl3w.h"

struct CubeMesh
{
	static const std::vector<GLfloat> vertices;
	static const std::vector<GLfloat> texture_coordinates;
};

#endif // !CUBE_MESH

