#pragma once
#include <vector>
#include <memory>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Renderer.h"
#include "Material/Phong.h"

struct Vertex {
    Vertex(float p1, float p2, float p3, float n1, float n2, float n3, float t1, float t2);
    Vertex(float p1, float p2, float p3, float t1, float t2);
    Vertex() {};
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
    glm::vec3 Tangent;
};
struct TextureInfo
{
    std::string TexturePath;
    TextureType Type;
    std::string TextureName;
};

class Mesh {
public:

    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<std::unique_ptr<Texture>> Textures;

    Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices,const std::vector<TextureInfo>& textureInfos);
    Mesh(const std::vector<Vertex>& vertices);
    Mesh();
    void ImGuiRender();
    

    void DrawMesh(Shader& shader);

    std::unique_ptr<VertexArray> m_VAO;

private:
    //std::unique_ptr<VertexArray> m_VAO;
    std::unique_ptr<IndexBuffer> m_EBO;
    std::unique_ptr<VertexBuffer> m_VBO;

    void setupMesh();
};