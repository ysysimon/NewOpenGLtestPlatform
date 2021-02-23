#include <iostream>
#include "Mesh.h"
#include "VertexBufferLayout.h"


Vertex::Vertex(float p1, float p2, float p3, float n1, float n2, float n3, float t1, float t2)
    :Position(glm::vec3(p1,p2,p3)),Normal(glm::vec3(n1,n2,n3)),TexCoords(glm::vec2(t1,t2))
{
}

Vertex::Vertex(float p1, float p2, float p3, float t1, float t2)
    : Position(glm::vec3(p1, p2, p3)),Normal(glm::vec3(0, 0, 0)), TexCoords(glm::vec2(t1, t2))
{
}

Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, const std::vector<TextureInfo>& textureInfos)
{
    this->vertices = vertices;
    this->indices = indices;

    //bool flag = true;
    for (int i = 0; i < textureInfos.size(); i++)
    {
        /*for (int j = 0; j < Textures.size(); j++)
        {
            if (textureInfos[i].TexturePath == (*(Textures[j])).GetPath())
            {
                flag = false;
                this->Textures.push_back(std::move(Textures[j]));
            }
        }*/
        this->Textures.push_back(std::make_unique<Texture>(textureInfos[i].TexturePath, textureInfos[i].Type, textureInfos[i].TextureName));
    }

    setupMesh();
}

Mesh::Mesh(const std::vector<Vertex>& vertices)
{
    this->vertices = vertices;

    setupMesh();
}

Mesh::Mesh()
{
}

void Mesh::DrawMesh(Shader& shader)
{
    unsigned int diffuseNr = 1;
    unsigned int specularNr = 1;

    for (int i = 0; i < this->Textures.size(); i++)
    {

        std::string number;
        TextureType type = (*(Textures[i])).Type;
        switch (type)
        {
        case None:
            break;
        case Diffuse:
            number = std::to_string(diffuseNr++);
            break;
        case Specular:
            number = std::to_string(specularNr++);
            break;
        case Height:
            number = "";
            break;
        case Normal:
            break;
        default:
            std::cout << "Texture Error: " << (*(Textures[i])).GetName() << std::endl;
            return;
            break;
        }

        (*(Textures[i])).Bind(i);
        shader.SetUniform1i((GetTextureTypeName(type) + number), i);
    }

    Renderer::Get().Draw(*m_VAO,*m_EBO,shader);
}
void Mesh::setupMesh()
{
    m_VBO = std::make_unique<VertexBuffer>(&vertices[0], vertices.size() * sizeof(Vertex));
    VertexBufferLayout layout;
    layout.Push<float>(3);
    layout.Push<float>(3);
    layout.Push<float>(2);
    layout.Push<float>(3);

    m_VAO = std::make_unique<VertexArray>();
    m_VAO->AddBuffer(*m_VBO, layout);

    if (!this->indices.empty())
    {
        m_EBO = std::make_unique<IndexBuffer>(&indices[0], this->indices.size());
    }
    
}

void Mesh::ImGuiRender()
{
    for (int i = 0; i < this->Textures.size(); i++)
    {
        Textures[i]->ImGuiRender();
    }
    
}