



#ifndef ABSTRACT_DRAWABLE
#define ABSTRACT_DRAWABLE

#include "shader_handling/shader_program.h"

class AbstractDrawable
{
public:
	virtual void draw(unsigned int shaderProgramId = 0) const = 0;

	virtual ~AbstractDrawable() = default;
};

#endif // !ABSTRACT_DRAWABLE

