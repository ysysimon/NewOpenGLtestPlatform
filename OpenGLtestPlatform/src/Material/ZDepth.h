#pragma once
#include <functional>
#include "Material.h"
#include "DepthMap.h"
#include  "Camera.h"
#include "Model/DeBugViewGrid.h"

struct ZDepth
	:public Material
{
	void GenerateDepthMap(Camera& camera, glm::mat4& SpaceMatrix, std::vector<std::tuple<std::string,glm::vec3, float>>& ModelMatrixFactor, std::vector<std::function<void(Shader&)>>& DrawCalls);
	void Draw();
	void ImGuiRender();

	
private:
	DepthMap m_DepthMap;
	DebugViewGrid m_DebugViewGrid;

	std::unique_ptr<Shader> dummyShader;
	Shader& GetShader()const override;

	static const char* ProjectionType[];
	int m_ProjectionTypeIndex = 1;
};