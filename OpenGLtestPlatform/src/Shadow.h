#pragma once
#include <functional>
#include "DepthMap.h"
#include "LightCaster/LightCaster.h"

class Shadow
{
public:
	Shadow();
	~Shadow();

	void ImGuiRender();
	void GenerateShadowMap(Camera& camera, DistantLight& DistantLight, std::vector<std::tuple<std::string, glm::vec3, float>>& ModelMatrixFactor, std::vector<std::function<void(Shader&)>>& DrawCalls);
	void BindShadowMap(unsigned int slot = 0);
private:
	DepthMap m_ShadowDepthMap;
};
