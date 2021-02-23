#include<iostream>

#include "imgui/imgui.h"

#include "Model.h"
#include "Material/Phong.h"

Model::Model(const std::string& path)
    :m_FilePath(path)
{
    loadModel(path);
    FlipUVs_Befor = FlipUVs;
    UseNormalMap_Befor = UseNormalMap;
}


void Model::loadModel(const std::string& path)
{
    if (!path.size())
    {
        std::cout << "Model Loader Initialized" << std::endl;
        return;
    }
    Assimp::Importer import;
    const aiScene* scene;
    if (FlipUVs)
    {
        scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
    }
    else
    {
        scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_CalcTangentSpace);
    }
    

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
        std::cout << "Load Model Failed...Try to use '/' instead of '\\'" << std::endl;
        return;
    }
    std::cout << "Loading Model From (" << path << ") ...Succeed" << std::endl;
    directory = path.substr(0, path.find_last_of('/'));

    processNode(scene->mRootNode, scene);
}
void Model::processNode(aiNode* node, const aiScene* scene)
{
    // 处理节点所有的网格（如果有的话）
    for (unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(processMesh(mesh, scene));
    }
    // 接下来对它的子节点重复这一过程
    for (unsigned int i = 0; i < node->mNumChildren; i++)
    {
        processNode(node->mChildren[i], scene);
    }
}
Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene)
{
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<TextureInfo> textureInfos;

    // 处理顶点
    for (unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        Vertex vertex;
        // 处理顶点位置、法线和纹理坐标
        glm::vec3 vector;
        vector.x = mesh->mVertices[i].x;
        vector.y = mesh->mVertices[i].y;
        vector.z = mesh->mVertices[i].z;
        vertex.Position = vector;

        if (mesh->HasNormals())
        {
            vector.x = mesh->mNormals[i].x;
            vector.y = mesh->mNormals[i].y;
            vector.z = mesh->mNormals[i].z;
            vertex.Normal = vector;
        }
        

        if (mesh->mTextureCoords[0]) // 网格是否有纹理坐标？
        {
            glm::vec2 vec;
            vec.x = mesh->mTextureCoords[0][i].x;
            vec.y = mesh->mTextureCoords[0][i].y;
            vertex.TexCoords = vec;

            vector.x = mesh->mTangents[i].x;
            vector.y = mesh->mTangents[i].y;
            vector.z = mesh->mTangents[i].z;
            vertex.Tangent = vector;
        }
        else
            vertex.TexCoords = glm::vec2(0.0f, 0.0f);

        vertices.push_back(vertex);
    }
    // 处理索引
    for (unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }
    // 处理贴图
    if (mesh->mMaterialIndex >= 0)
    {
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
        std::vector<TextureInfo> diffuseMapsInfo = loadMaterialTextures(material,
            aiTextureType_DIFFUSE, " Diffuse", mesh->mName);
        textureInfos.insert(textureInfos.end(), diffuseMapsInfo.begin(), diffuseMapsInfo.end());
        std::vector<TextureInfo> specularMapsInfo = loadMaterialTextures(material,
            aiTextureType_SPECULAR, " Specular", mesh->mName);
        textureInfos.insert(textureInfos.end(), specularMapsInfo.begin(), specularMapsInfo.end());

        if (UseNormalMap)
        {
            std::vector<TextureInfo> normalMapsInfo = loadMaterialTextures(material,
                aiTextureType_HEIGHT, " Normal", mesh->mName);
            textureInfos.insert(textureInfos.end(), normalMapsInfo.begin(), normalMapsInfo.end());
        }
        
    }

    return Mesh(vertices, indices, textureInfos);
}
std::vector<TextureInfo> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName,aiString meshName)
{
    std::vector<TextureInfo> textureInfos;
    if (mat->GetTextureCount(type))
    {
        for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
        {
            TextureInfo textureInfo;
            aiString str;
            mat->GetTexture(type, i, &str);
            textureInfo.TextureName = meshName.C_Str() + typeName;
            textureInfo.Type = TextureType(type);
            textureInfo.TexturePath = directory + '/' + str.C_Str();
            textureInfos.push_back(textureInfo);
            //Texture texture(directory + '/'+ str.C_Str());
            //std::cout << directory + '/' + str.C_Str() << std::endl;
        }
    }
    else
    {
        TextureInfo textureInfo;
        textureInfo.TextureName = meshName.C_Str() + typeName;
        textureInfo.Type = TextureType::Diffuse;
        textureInfo.TexturePath = "res/textures/Default_Texture.png";
        textureInfos.push_back(textureInfo);
    }
    
    return textureInfos;
}

void Model::ImGuiRender()
{
    ImGui::Checkbox("Flip UV", &FlipUVs);
    if (FlipUVs!=FlipUVs_Befor)
    {
        FlipUVs_Befor = FlipUVs;
        loadModel(m_FilePath);
        std::cout << "Fliping UV" << std::endl;
    }
    ImGui::SameLine();
    ImGui::Checkbox("Use Normal Mapping", &UseNormalMap);
    if (UseNormalMap != UseNormalMap_Befor)
    {
        UseNormalMap_Befor = UseNormalMap;
        loadModel(m_FilePath);
        std::cout << "Using Normal Mapping" << std::endl;
    }
    for (int i = 0; i < this->meshes.size(); i++)
    {
        meshes[i].ImGuiRender();
    }
    //ImGui::SameLine();
    if (ImGui::Button("Update Texture"))
    {

        std::cout << "Now Changing Texture" << std::endl;
        for (int i = 0; i < this->meshes.size(); i++)
        {   
            for (int j = 0; j < meshes[i].Textures.size(); j++)
            {
                meshes[i].Textures[j]->ChangeTexture(meshes[i].Textures[j]->TextBox_Buffer);
            }
        }
        std::cout << "Change Texture Succeed" << std::endl;
    }
}

void Model::Draw(Shader& shader)
{
    for (unsigned int i = 0; i < meshes.size(); i++)
        meshes[i].DrawMesh(shader);
}