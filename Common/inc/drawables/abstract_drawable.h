



#ifndef ABSTRACT_DRAWABLE
#define ABSTRACT_DRAWABLE

#include "GL/gl3w.h"

namespace drawables
{
	class AbstractDrawable
	{
	public:
		virtual void draw(const GLuint& shaderProgramId = 0) const = 0;

		virtual ~AbstractDrawable() = default;
	};
}

#endif // !ABSTRACT_DRAWABLE

