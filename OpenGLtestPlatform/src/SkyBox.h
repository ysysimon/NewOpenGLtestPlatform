#pragma once
#include <memory>
#include "Renderer.h"
#include "EnvironmentMap.h"
#include "Camera.h"

class SkyBox
{
public:
	SkyBox(const std::string& path, const std::vector<std::string>& faces);
	~SkyBox();

	bool m_ShowSkyBox;

	void Draw(Camera& camera);

	inline EnvironmentMap& GetEnvironmentMap() const{
		return (*m_EnvironmentMap);
	}

	void ImGuiRender();
private:
	std::unique_ptr<EnvironmentMap> m_EnvironmentMap;
	std::unique_ptr<VertexBuffer> m_VBO;
	std::unique_ptr<VertexArray> m_VAO;
	std::unique_ptr<Shader> m_Shader;
	
	
};
