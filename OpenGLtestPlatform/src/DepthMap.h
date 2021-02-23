#pragma once
#include <memory>
#include <functional>
#include "Renderer.h"
#include "FrameBuffer.h"
#include  "Camera.h"


class DepthMap
{
public:
	DepthMap();
	~DepthMap();

	void GenerateDepthMap(Camera& camera, glm::mat4& SpaceMatrix, std::vector<std::tuple<std::string, glm::vec3, float>>& ModelMatrixFactor, std::vector<std::function<void(Shader&)>>& DrawCalls);
	
	
	void SetSCRsize(float SCR_WIDTH, float SCR_HEIGHT);
	void Bind(unsigned int slot = 0);
	void UnBind();

	inline unsigned int GetRendererID()
	{
		return m_RendererID;
	}

	inline Shader& GetShader()
	{
		return *DepthShader;
	}

	/*inline void SetProjectionType(bool Type)
	{
		this->m_ProjectionType = Type;
	}*/

	bool IsShadow = false;
private:
	void PreProcess();
	void PostProcess();
	void SetUniform(Camera& camera, glm::mat4& SpaceMatrix, glm::mat4& model);

	unsigned int m_RendererID;
	FrameBuffer m_frameBuffer;
	std::unique_ptr<Shader> DepthShader;

	static const unsigned int SHADOW_WIDTH,SHADOW_HEIGHT;
	float SCR_WIDTH = 1024,SCR_HEIGHT = 1024;
	//bool m_ProjectionType = true;
};
