#include "LightCaster.h"
#include "imgui/imgui.h"

	//const enum LightType BaseLight::lightType = LightType::Base;
	BaseLight::BaseLight(glm::vec3 color, float ambientCoefficient, float diffuseCoefficient, float specularCoefficient)
		:color(color),ambientCoefficient(ambientCoefficient),diffuseCoefficient(diffuseCoefficient),specularCoefficient(specularCoefficient)
	{
	}
	BaseLight::BaseLight() {};

	//const enum LightType DistantLight::lightType = LightType::Distant;
	DistantLight::DistantLight(glm::vec3 direction, glm::vec3 color, float ambientCoefficient, float diffuseCoefficient, float specularCoefficient)
		:direction(direction)
	{
		this->color = color;
		this->ambientCoefficient = ambientCoefficient;
		this->diffuseCoefficient = diffuseCoefficient;
		this->specularCoefficient = specularCoefficient;
	}
	void DistantLight::ImGuiRender()
	{
		if (ImGui::TreeNode("Distant Light"))
		{
			ImGui::Checkbox("Enable Distant Light", &EnableDistantLight);
			ImGui::SliderFloat3("Light Direction", (float*)&direction, -50.0f, 50.0f);
			ImGui::SliderFloat("Ambient Coefficient", &ambientCoefficient, 0.0f, 1.0f);
			ImGui::SliderFloat("Diffuse Coefficient", &diffuseCoefficient, 0.0f, 1.0f);
			ImGui::SliderFloat("specular Coefficient", &specularCoefficient, 0.0f, 1.0f);
			ImGui::ColorEdit3("Light Color", (float*)&color);

			ImGui::TreePop();
		}
	}
	
	//const enum LightType PointLight::lightType = LightType::Point;
	const char* PointLight::attenuationDistance[] = {"7","13","20","32","50","65","100","160","200","325"};
	const float PointLight::K = 1.0f;
	PointLight::PointLight(glm::vec3 position, glm::vec3 color, float ambientCoefficient, float diffuseCoefficient, float specularCoefficient, float linear, float quadratic)
		:position(position),linear(linear),quadratic(quadratic)
	{
		this->color = color;
		this->ambientCoefficient = ambientCoefficient;
		this->diffuseCoefficient = diffuseCoefficient;
		this->specularCoefficient = specularCoefficient;
	}
	void PointLight::SetAttenuationDistance(int caseNum)
	{
		switch (caseNum)
		{
		default:
			break;
		case 0:
			this->linear = 0.7f;
			this->quadratic = 1.8f;
			break;
		case 1:
			this->linear = 0.35f;
			this->quadratic = 0.44f;
			break;
		case 2:
			this->linear = 0.22f;
			this->quadratic = 0.20f;
			break;
		case 3:
			this->linear = 0.14f;
			this->quadratic = 0.07f;
			break;
		case 4:
			this->linear = 0.09f;
			this->quadratic = 0.032f;
			break;
		case 5:
			this->linear = 0.07f;
			this->quadratic = 0.017f;
			break;
		case 6:
			this->linear = 0.045f;
			this->quadratic = 0.0075f;
			break;
		case 7:
			this->linear = 0.027f;
			this->quadratic = 0.0028f;
			break;
		case 8:
			this->linear = 0.022f;
			this->quadratic = 0.0019f;
			break;
		case 9:
			this->linear = 0.014f;
			this->quadratic = 0.0007f;
			break;
		}
	}
	void PointLight::ImGuiRender()
	{
		if (ImGui::TreeNode("Point Light"))
		{
			ImGui::Checkbox("Enable Point Light", &EnablePointLight);
			ImGui::SameLine();
			ImGui::Checkbox("Hide Point Light", &HidePointLight);
			ImGui::SliderFloat3("Light Position", (float*)&position, -10.0f, 10.0f);
			//ImGui::Combo("m Attenuation", &PointLightAttenuationNum, PointLight::attenuationDistance, 10);
			//SetAttenuationDistance(PointLightAttenuationNum);
			ImGui::SliderFloat("Ambient Coefficient", &ambientCoefficient, 0.0f, 1.0f);
			ImGui::SliderFloat("Diffuse Coefficient", &diffuseCoefficient, 0.0f, 1.0f);
			ImGui::SliderFloat("specular Coefficient", &specularCoefficient, 0.0f, 1.0f);
			ImGui::ColorEdit3("Light Color", (float*)&color);
			ImGui::SliderFloat("Light intensity", &intensity, 0.0f, 1000.0f);

			ImGui::TreePop();
		}
	}

	//const enum LightType Spotlight::lightType = LightType::Spot;
	SpotLight::SpotLight(glm::vec3 position, glm::vec3 color, float ambientCoefficient, float diffuseCoefficient, float specularCoefficient, float cutoffAngle, float outerCutOffAngle,glm::vec3 spotDirection)
		:position(position), cutoffAngle(cutoffAngle),outerCutOffAngle(outerCutOffAngle),spotDirection(spotDirection)
	{
		this->color = color;
		this->ambientCoefficient = ambientCoefficient;
		this->diffuseCoefficient = diffuseCoefficient;
		this->specularCoefficient = specularCoefficient;
	}
	void SpotLight::ImGuiRender()
	{
		if (ImGui::TreeNode("Spot Light"))
		{
			ImGui::Checkbox("Enable Spot Light", &EnableSpotLight);
			ImGui::SameLine();
			ImGui::Checkbox("CutOff Angle Safe Lock", &SpotLightCutOffAngleSafeLock);
			ImGui::SliderFloat("Ambient Coefficient", &ambientCoefficient, 0.0f, 1.0f);
			ImGui::SliderFloat("Diffuse Coefficient", &diffuseCoefficient, 0.0f, 1.0f);
			ImGui::SliderFloat("specular Coefficient", &specularCoefficient, 0.0f, 1.0f);
			ImGui::ColorEdit3("Light Color", (float*)&color);
			float currentCutoffAngle = cutoffAngle;
			ImGui::SliderFloat("CutOff Angle", &cutoffAngle, 0.0f, 90.0f);
			if (SpotLightCutOffAngleSafeLock && (cutoffAngle >outerCutOffAngle))
			{
				cutoffAngle = currentCutoffAngle;
			}
			float currentOuterCutOffAngle = outerCutOffAngle;
			ImGui::SliderFloat("Out CutOff Angle", &outerCutOffAngle, 0.0f, 90.0f);
			if (SpotLightCutOffAngleSafeLock && (cutoffAngle > outerCutOffAngle))
			{
				outerCutOffAngle = currentOuterCutOffAngle;
			}
			ImGui::Text("Spot Pos:    X= %f Y= %f Z= %f", position.x, position.y, position.z);
			ImGui::Text("Spot Direction: X=  %f Y=  %f Z= %f", spotDirection.x, spotDirection.y,spotDirection.z);

			ImGui::TreePop();
		}
	}
