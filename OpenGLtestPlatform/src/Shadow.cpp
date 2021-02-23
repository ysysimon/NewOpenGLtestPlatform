#include "Shadow.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
Shadow::Shadow()
{
    m_ShadowDepthMap.IsShadow = true;
}

Shadow::~Shadow()
{
}

void Shadow::GenerateShadowMap(Camera& camera,DistantLight& DistantLight,std::vector<std::tuple<std::string, glm::vec3, float>>& ModelMatrixFactor, std::vector<std::function<void(Shader&)>>& DrawCalls)
{
    //render depth of scene to texture (from light's perspective)  
    glm::mat4 lightProjection, lightView;
    glm::mat4 lightSpaceMatrix;
    lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, camera.GetNearPlane(), camera.GetFarPlane());
    lightView = glm::lookAt(-(DistantLight.direction), glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
    lightSpaceMatrix = lightProjection * lightView;

    //m_ShadowDepthMap.IsShadow = true;
    m_ShadowDepthMap.GenerateDepthMap(camera, lightSpaceMatrix, ModelMatrixFactor, DrawCalls);

}

void Shadow::ImGuiRender()
{
}

void Shadow::BindShadowMap(unsigned int slot)
{
    this->m_ShadowDepthMap.Bind(slot);
}