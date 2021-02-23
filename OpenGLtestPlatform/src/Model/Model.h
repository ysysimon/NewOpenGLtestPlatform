#pragma once
#include <vector>
#include <string>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Mesh.h"


class Model
{
public:
    Model(const std::string& path);
    void Draw(Shader& shader);
    
    void ImGuiRender();

private:
    std::vector<Mesh> meshes;
    
    std::string directory;
    
    std::string m_FilePath;
    bool FlipUVs = false;
    bool FlipUVs_Befor;
    bool UseNormalMap = true;
    bool UseNormalMap_Befor;

    void loadModel(const std::string& path);
    void processNode(aiNode* node, const aiScene* scene);
    Mesh processMesh(aiMesh* mesh, const aiScene* scene);
    std::vector<TextureInfo> loadMaterialTextures(aiMaterial* mat, aiTextureType type,
        std::string typeName, aiString meshName);
};
