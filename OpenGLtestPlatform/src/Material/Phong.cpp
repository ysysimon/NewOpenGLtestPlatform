#include <iostream>
#include <typeinfo> 
#include "Phong.h"


Phong::Phong()
{
	
	this->PhongShader = std::make_unique<Shader>("res/shaders/Phong.shader");

	
}

void Phong::ImGuiRender()
{
	if (ImGui::TreeNode("Material Phong Setting"))
	{
		ImGui::SliderFloat("Shininess", &(this->shininess), 0.0f, 500.0f);
		ImGui::Checkbox("Transform Texture Color Space", &transformColorSpace);

		ImGui::TreePop();
	}
}

void Phong::SetUniform(const glm::mat4& model, Camera& camera,DistantLight& DistantLight, PointLight& PointLight, SpotLight& SpotLight)
{
	glm::mat4 lightProjection, lightView;
	glm::mat4 lightSpaceMatrix;
	lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, camera.GetNearPlane(), camera.GetFarPlane());
	lightView = glm::lookAt(-(DistantLight.direction), glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
	lightSpaceMatrix = lightProjection * lightView;

	PhongShader->Bind();
	PhongShader->SetUniformMat4f("model", model/*glm::translate(model, m_CubePosition)*/);
	PhongShader->SetUniformMat4f("view", camera.GetViewMatrix());
	PhongShader->SetUniformMat4f("projection", camera.GetProjectionMatrix());
	PhongShader->SetUniformMat4f("lightSpaceMatrix", lightSpaceMatrix);

	PhongShader->SetUniform3v("viewPos", camera.GetCameraPos());
	PhongShader->SetUniform1f("material.shininess", shininess);

	PhongShader->SetUniform1i("shadowMap", 5);

	PhongShader->SetUniform1i("EnablePointLight", PointLight.EnablePointLight);
	PhongShader->SetUniform1i("EnableDistantLight", DistantLight.EnableDistantLight);
	PhongShader->SetUniform1i("EnableSpotLight", SpotLight.EnableSpotLight);

	PhongShader->SetUniform1i("transformColorSpace", transformColorSpace);
	if (DistantLight.EnableDistantLight)
	{
		SetLightUniform(DistantLight, camera);
	}

	if (PointLight.EnablePointLight)
	{
		SetLightUniform(PointLight, camera);
	}

	if (SpotLight.EnableSpotLight)
	{
		SetLightUniform(SpotLight, camera);
	}
}
void Phong::SetLightUniform(BaseLight& Light, Camera& camera)
{
	/*std::cout << Light.GetType() << std::endl;*/
	PhongShader->Bind();
	BaseLight* bLight = &Light;
	DistantLight* dLight = dynamic_cast<DistantLight*>(bLight);
	PointLight* pLight = dynamic_cast<PointLight*>(bLight);
	SpotLight* sLight = dynamic_cast<SpotLight*>(bLight);
	switch (Light.GetType())
	{
	case 0:
		std::cout << "Should Not Pass Light Type 'Base'" << std::endl;
		break;
	case 1:
		/*std::cout << "Distant" << std::endl;*/
		PhongShader->SetUniform3v("distantLight.color", dLight->color);
		PhongShader->SetUniform1f("distantLight.ambientCoefficient", dLight->ambientCoefficient);
		PhongShader->SetUniform1f("distantLight.diffuseCoefficient", dLight->diffuseCoefficient);
		PhongShader->SetUniform1f("distantLight.specularCoefficient", dLight->specularCoefficient);
		PhongShader->SetUniform3v("distantLight.direction", dLight->direction);
		break;
	case 2:
		/*std::cout << "Point" << std::endl;*/
		PhongShader->SetUniform3v("pointLight.color", pLight->color);
		PhongShader->SetUniform1f("pointLight.ambientCoefficient", pLight->ambientCoefficient);
		PhongShader->SetUniform1f("pointLight.diffuseCoefficient", pLight->diffuseCoefficient);
		PhongShader->SetUniform1f("pointLight.specularCoefficient", pLight->specularCoefficient);
		PhongShader->SetUniform3v("pointLight.position", pLight->position);
		PhongShader->SetUniform1f("pointLight.intensity", pLight->intensity);
		//PhongShader->SetUniform1f("pointLight.K", PointLight::K);
		//PhongShader->SetUniform1f("pointLight.linear", pLight->linear);
		//PhongShader->SetUniform1f("pointLight.quadratic", pLight->quadratic);
		break;
	case 3:
		/*std::cout << "Spot" << std::endl;*/
		PhongShader->SetUniform3v("spotLight.color", sLight->color);
		PhongShader->SetUniform1f("spotLight.ambientCoefficient", sLight->ambientCoefficient);
		PhongShader->SetUniform1f("spotLight.diffuseCoefficient", sLight->diffuseCoefficient);
		PhongShader->SetUniform1f("spotLight.specularCoefficient", sLight->specularCoefficient);
		PhongShader->SetUniform3v("spotLight.position", camera.GetCameraPos());
		sLight->position = camera.GetCameraPos();
		PhongShader->SetUniform1f("spotLight.cutoffAngleCos", glm::cos(glm::radians(sLight->cutoffAngle)));
		PhongShader->SetUniform1f("spotLight.outerCutOffAngleCos", glm::cos(glm::radians(sLight->outerCutOffAngle)));
		PhongShader->SetUniform3v("spotLight.spotDirection", camera.GetCameraTarget());
		sLight->spotDirection = camera.GetCameraTarget();
		break;
	default:
		std::cout << "Light Type Error" << std::endl;
		break;
	}

}

Shader& Phong::GetShader()const
{
	return *PhongShader;
}