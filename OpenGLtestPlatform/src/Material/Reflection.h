#pragma once
#include "Material.h"
#include "EnvironmentMap.h"

struct Reflection :
	public Material
{
	Reflection();
	void SetUniform(const glm::mat4& model, Camera& camera,const EnvironmentMap& envMap);
	std::unique_ptr<Shader> ReflectionShader;

	Shader& GetShader() const;

};

