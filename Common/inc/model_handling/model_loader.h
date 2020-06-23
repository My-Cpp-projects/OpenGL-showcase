



#ifndef MODEL_LOADER
#define MODEL_LOADER

#include "drawables/mesh.h"

#include "GL/gl3w.h"
#include "glfw/glfw3.h"
#include <assimp/scene.h>
#include <string>
#include <vector>

namespace model_handling
{
    class ModelLoader
    {
    public:
        ModelLoader() = default;
        virtual ~ModelLoader() = default;

        std::vector<drawables::Mesh_uptr> loadModel(const std::string& path);

    private:
        void processNode(aiNode* node, const aiScene* scene, std::vector<drawables::Mesh_uptr>& output);
        drawables::Mesh_uptr processMesh(aiMesh* mesh, const aiScene* scene);
        std::vector<drawables::Texture> loadMaterialTextures(const aiMaterial const* mat, aiTextureType type, std::string typeName);

        std::vector<drawables::Texture> m_loadedTextures;
        std::string m_directory;
    };
}

#endif // !MODEL_LOADER

