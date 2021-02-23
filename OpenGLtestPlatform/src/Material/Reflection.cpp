#include "Reflection.h"

Reflection::Reflection()
{
	ReflectionShader = std::make_unique<Shader>("res/shaders/Reflection.shader");
}

void Reflection::SetUniform(const glm::mat4& model, Camera& camera, const EnvironmentMap& envMap)
{

	ReflectionShader->Bind();
	envMap.Bind();
	ReflectionShader->SetUniformMat4f("model", model);
	ReflectionShader->SetUniformMat4f("view", camera.GetViewMatrix());
	ReflectionShader->SetUniformMat4f("projection", camera.GetProjectionMatrix());

	ReflectionShader->SetUniform3v("cameraPos", camera.GetCameraPos());
	ReflectionShader->SetUniform1i("skybox", 0);

}

Shader& Reflection::GetShader()const
{
	return *ReflectionShader;
}