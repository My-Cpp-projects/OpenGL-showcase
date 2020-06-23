



#include "model_handling/model_loader.h"
#include "drawables/mesh.h"
#include "texture_handling/texture_loader.h"

#include "gl3w/GL/gl3w.h"
#include "glfw/glfw3.h"
#include <glm/glm.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <iostream>

namespace model_handling
{
	std::vector<drawables::Mesh_uptr> ModelLoader::loadModel(const std::string& path)
	{
		Assimp::Importer importer;
		const auto scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

		std::vector<std::unique_ptr<drawables::Mesh>> retVal;
		if(not scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || not scene->mRootNode)
		{
			std::cerr << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
			return retVal;
		}

		// retrieve the directory path of the filepath
		m_directory = path.substr(0, path.find_last_of('/'));
	
		processNode(scene->mRootNode, scene, retVal);
		return retVal;
	}

	void ModelLoader::processNode(aiNode* node, const aiScene* scene, std::vector<drawables::Mesh_uptr>& output)
	{
		// process each mesh located at the current node
		for(unsigned int i = 0; i < node->mNumMeshes; ++i)
		{
			auto mesh = scene->mMeshes[node->mMeshes[i]];
			output.emplace_back(processMesh(mesh, scene));
		}
		// recursively process each of the children nodes
		for(unsigned int i = 0; i < node->mNumChildren; ++i)
		{
			processNode(node->mChildren[i], scene, output);
		}
	}

	drawables::Mesh_uptr ModelLoader::processMesh(aiMesh* mesh, const aiScene* scene)
	{
		std::vector<drawables::Vertex> vertices;
		std::vector<unsigned int> indices;
		std::vector<drawables::Texture> textures;

		// walk through each of the mesh's vertices
		for(unsigned int i = 0; i < mesh->mNumVertices; ++i)
		{
			drawables::Vertex vertex;
			glm::vec3 tmp_vector;
			// positions
			tmp_vector.x = mesh->mVertices[i].x;
			tmp_vector.y = mesh->mVertices[i].y;
			tmp_vector.z = mesh->mVertices[i].z;
			vertex.m_position = tmp_vector;
			// normals
			tmp_vector.x = mesh->mNormals[i].x;
			tmp_vector.y = mesh->mNormals[i].y;
			tmp_vector.z = mesh->mNormals[i].z;
			vertex.m_normal = tmp_vector;
			// texture coordinates
			if(mesh->mTextureCoords[0])
			{
				glm::vec2 vec;
				// a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't 
				// use models where a vertex can have multiple texture coordinates so we always take the first set (0).
				vec.x = mesh->mTextureCoords[0][i].x;
				vec.y = mesh->mTextureCoords[0][i].y;
				vertex.m_texCoords = vec;
			}
			else
				vertex.m_texCoords = glm::vec2(0.0f, 0.0f);
			// tangent
			tmp_vector.x = mesh->mTangents[i].x;
			tmp_vector.y = mesh->mTangents[i].y;
			tmp_vector.z = mesh->mTangents[i].z;
			vertex.m_tangent = tmp_vector;
			// bitangent
			tmp_vector.x = mesh->mBitangents[i].x;
			tmp_vector.y = mesh->mBitangents[i].y;
			tmp_vector.z = mesh->mBitangents[i].z;
			vertex.m_bitangent = tmp_vector;
			vertices.push_back(vertex);
		}
		// now walk through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
		for(unsigned int i = 0; i < mesh->mNumFaces; ++i)
		{
			aiFace face = mesh->mFaces[i];
			for(unsigned int j = 0; j < face.mNumIndices; ++j)
				indices.push_back(face.mIndices[j]);
		}
		// process materials
		auto material = scene->mMaterials[mesh->mMaterialIndex];
		// assume a convention for sampler names in the shaders. Each diffuse texture should be named
		// as 'texture_diffuseN' where N is a sequential number ranging from 1 to MAX_SAMPLER_NUMBER. 
		// Same applies to other texture as the following list summarizes:
		// diffuse: texture_diffuseN
		// specular: texture_specularN
		// normal: texture_normalN

		// 1. diffuse maps
		std::vector<drawables::Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
		// 2. specular maps
		std::vector<drawables::Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
		// 3. normal maps
		std::vector<drawables::Texture> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
		textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
		// 4. height maps
		std::vector<drawables::Texture> heightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
		textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

		// return a mesh object created from the extracted mesh data
		return std::make_unique<drawables::Mesh>(vertices, indices, textures);
	}

	std::vector<drawables::Texture> ModelLoader::loadMaterialTextures(const aiMaterial const* mat, aiTextureType type, std::string typeName)
	{
		std::vector<drawables::Texture> textures;
		for(unsigned int i = 0; i < mat->GetTextureCount(type); ++i)
		{
			aiString str;
			mat->GetTexture(type, i, &str);
			// check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
			bool skip = false;
			for(const auto& tex : m_loadedTextures)
			{
				if(std::strcmp(tex.m_path.data(), str.C_Str()) == 0)
				{
					textures.emplace_back(tex);
					skip = true; // a texture with the same filepath has already been loaded, continue to next one.
					break;
				}
			}

			if(not skip)
			{   // if texture hasn't been loaded already, load it
				drawables::Texture texture;

				texture.m_id = texture_loader::getTextureFromFile(str.C_Str(), this->m_directory);
				texture.m_type = typeName;
				texture.m_path = str.C_Str();
				textures.emplace_back(texture);
				m_loadedTextures.emplace_back(texture);
			}
		}
		return textures;
	}
}