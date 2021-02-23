#pragma once
#include <memory>
#include <functional>

#include"Model/DefaultModels.h"
#include "Shader.h"
#include "FrameBuffer.h"
#include "Bloom.h"

class HDR
{
public:
	HDR(float SCR_WIDTH, float SCR_HEIGHT);
	~HDR();
	
	void PreProcess();
	void PostProcess();
	
	void DrawHDRresult();
	void ImGuiRender();

	void SetSCRsize(float SCR_WIDTH, float SCR_HEIGHT);
private:
	unsigned int m_ColorBufferRendererID; //color Buffer
	unsigned int m_DepthBufferRendererID; //depth Buffer
	unsigned int m_BloomBufferRendererID; //bloom Buffer
	FrameBuffer m_frameBuffer;
	std::unique_ptr<Shader> m_HDRShader;
	std::unique_ptr<Bloom> m_Bloom;
	DefaultModels m_DefaultModels;

	bool IDT;
	bool RRT;
	bool ODT;
	bool EOTF;
	bool enableBloom;
	float exposure;

	float SCR_WIDTH , SCR_HEIGHT;
	void SetUniform();
};
