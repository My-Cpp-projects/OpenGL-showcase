



#ifndef MODEL_H
#define MODEL_H

#include "drawables/abstract_drawable.h"
#include "drawables/mesh.h"

#include "GL/gl3w.h"
#include <string>
#include <vector>

namespace drawables
{
    class Model : public AbstractDrawable
    {
    public:
        Model(const std::string& path);
        virtual ~Model() = default;

        void draw(const GLuint& shaderProgramId) const;

    private:
        std::vector<drawables::Mesh_uptr> m_meshes;
    };
}

#endif