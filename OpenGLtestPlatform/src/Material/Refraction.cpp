#include "Refraction.h"

//const std::unordered_map<int, float> Refraction::RefractiveIndex = { {0 ,1.00f},{1 ,1.33f},{2 ,1.309f},{3 ,1.52},{4 ,2.42f} };
const char* Refraction::MaterialName[] = {"Air","Water","Ice","Glass","Diamond"};

Refraction::Refraction()
{
	RefractionShader = std::make_unique<Shader>("res/shaders/Refraction.shader");
}

void Refraction::SetUniform(const glm::mat4& model, Camera& camera, const EnvironmentMap& envMap)
{
	RefractionShader->Bind();
	envMap.Bind();
	RefractionShader->SetUniformMat4f("model", model);
	RefractionShader->SetUniformMat4f("view", camera.GetViewMatrix());
	RefractionShader->SetUniformMat4f("projection", camera.GetProjectionMatrix());

	RefractionShader->SetUniform3v("cameraPos", camera.GetCameraPos());
	RefractionShader->SetUniform1i("skybox", 0);
	RefractionShader->SetUniform1f("RefractiveIndex",Refraction::RefractiveIndex[m_RefractiveIndexNum]);
}

void Refraction::ImGuiRender()
{
	if (ImGui::TreeNode("Material Refraction Setting"))
	{
		ImGui::Combo("Refractive Index", &m_RefractiveIndexNum, Refraction::MaterialName, 5);
		ImGui::TreePop();
	}

}

Shader& Refraction::GetShader()const
{
	return *RefractionShader;
}