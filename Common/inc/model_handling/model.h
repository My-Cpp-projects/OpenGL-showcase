



#ifndef MODEL_H
#define MODEL_H

#include "glfw/glfw3.h"
#include <assimp/scene.h>
#include <string>
#include <vector>
#include <memory>

#include "model_handling/mesh.h"

namespace model_handling
{
    class Model
    {
    public:
        Model(const std::string& path);

        void draw(GLuint shaderProgramId) const;

    private:
        void loadModel(const std::string& path);
        void processNode(aiNode* node, const aiScene* scene);
        std::unique_ptr<Mesh> processMesh(aiMesh* mesh, const aiScene* scene);
        std::vector<Texture> loadMaterialTextures(const aiMaterial const* mat, aiTextureType type, std::string typeName);

        std::vector<Texture> m_loadedTextures;
        std::vector<std::unique_ptr<Mesh>> m_meshes;
        std::string m_directory;
    };
}

#endif