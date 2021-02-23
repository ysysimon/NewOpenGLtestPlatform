#include "SkyBox.h"
#include "VertexBufferLayout.h"
#include "imgui/imgui.h"

SkyBox::SkyBox(const std::string& path, const std::vector<std::string>& faces)
    :m_ShowSkyBox(true)
{
    float skyboxVertices[] = {
        // positions          
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f
    };

    m_VBO = std::make_unique<VertexBuffer>(skyboxVertices,sizeof(skyboxVertices));
    VertexBufferLayout layout;
    layout.Push<float>(3);

    m_VAO = std::make_unique<VertexArray>();
    m_VAO->AddBuffer(*m_VBO,layout);

    m_Shader = std::make_unique<Shader>("res/shaders/SkyBox.shader");
    m_EnvironmentMap = std::make_unique<EnvironmentMap>(path, faces);
}

SkyBox::~SkyBox()
{
}

void SkyBox::Draw(Camera& camera)
{
    GLCall(glDepthFunc(GL_LEQUAL));
    m_Shader->Bind();
    m_EnvironmentMap->Bind();
    m_Shader->SetUniformMat4f("view", glm::mat4(glm::mat3(camera.GetViewMatrix())));
    m_Shader->SetUniformMat4f("projection", camera.GetProjectionMatrix());
    m_Shader->SetUniform1i("skybox",0);
    m_Shader->SetUniform1i("ShowSkyBox", m_ShowSkyBox);

    
    Renderer::Get().DrawCube(*m_VAO, *m_Shader);

    GLCall(glDepthMask(GL_LESS));
}

void SkyBox::ImGuiRender()
{
    ImGui::Checkbox("Show Environment Map",&m_ShowSkyBox);
}