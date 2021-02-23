#pragma once
#include <unordered_map>
#include "Material.h"
#include "EnvironmentMap.h"

struct Refraction :
	public Material
{
	Refraction();
	void SetUniform(const glm::mat4& model, Camera& camera, const EnvironmentMap& envMap);
	void ImGuiRender()override;
	Shader& GetShader()const;
private:
	std::unique_ptr<Shader> RefractionShader;
	std::unordered_map<int, float> RefractiveIndex = { {0 ,1.00f},{1 ,1.33f},{2 ,1.309f},{3 ,1.52},{4 ,2.42f} };
	static const char* MaterialName[];
	int m_RefractiveIndexNum = 3;
};
