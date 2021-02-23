#include "HDR.h"

HDR::HDR(float SCR_WIDTH, float SCR_HEIGHT)
	:SCR_WIDTH(SCR_WIDTH), SCR_HEIGHT(SCR_HEIGHT), IDT(false),RRT(true), ODT(true), EOTF(true), enableBloom(true),exposure(1.0f)
{
	m_HDRShader = std::make_unique<Shader>("res/shaders/HDR.shader");
	m_HDRShader->Bind();
	m_HDRShader->SetUniform1i("hdrBuffer", 0);
	m_HDRShader->SetUniform1i("bloomBlur", 1);

	GLCall(glGenTextures(1, &m_ColorBufferRendererID));
	GLCall(glBindTexture(GL_TEXTURE_2D, m_ColorBufferRendererID));
	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGBA, GL_FLOAT, NULL));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);  // clamp to the edge as the blur filter would otherwise sample repeated texture values!
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	GLCall(glGenTextures(1, &m_BloomBufferRendererID));
	GLCall(glBindTexture(GL_TEXTURE_2D, m_BloomBufferRendererID));
	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGBA, GL_FLOAT, NULL));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);  
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	GLCall(glGenRenderbuffers(1, &m_DepthBufferRendererID));
	GLCall(glBindRenderbuffer(GL_RENDERBUFFER, m_DepthBufferRendererID));
	GLCall(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, SCR_WIDTH, SCR_HEIGHT));

	m_frameBuffer.Bind();
	m_frameBuffer.AttachTexture(GL_COLOR_ATTACHMENT0, m_ColorBufferRendererID);
	m_frameBuffer.AttachTexture(GL_COLOR_ATTACHMENT1, m_BloomBufferRendererID);
	m_frameBuffer.AttachRenderbufferObj(GL_DEPTH_ATTACHMENT, m_DepthBufferRendererID);
	// tell OpenGL which color attachments we'll use (of this framebuffer) for rendering 
	unsigned int attachments[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
	GLCall(glDrawBuffers(2, attachments));
	m_frameBuffer.CheckFramebufferStatus();
	m_frameBuffer.Unbind();

	m_Bloom = std::make_unique<Bloom>(m_BloomBufferRendererID, SCR_WIDTH, SCR_HEIGHT);
}

HDR::~HDR()
{
	GLCall(glDeleteTextures(1, &m_ColorBufferRendererID));
	GLCall(glDeleteRenderbuffers(1, &m_DepthBufferRendererID));
}

void HDR::SetSCRsize(float SCR_WIDTH, float SCR_HEIGHT)
{
	this->SCR_WIDTH = SCR_WIDTH;
	this->SCR_HEIGHT = SCR_HEIGHT;
}

void HDR::PreProcess()
{
	this->m_frameBuffer.Bind();
	GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
}

void HDR::PostProcess()
{
	this->m_frameBuffer.Unbind();
	GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
}

void HDR::SetUniform()
{
	m_HDRShader->Bind();
	m_HDRShader->SetUniform1i("IDT", IDT);
	m_HDRShader->SetUniform1i("RRT", RRT);
	m_HDRShader->SetUniform1i("ODT", ODT);
	m_HDRShader->SetUniform1i("EOTF", EOTF);
	m_HDRShader->SetUniform1i("enableBloom", enableBloom);
	//m_HDRShader->SetUniform1f("exposure", exposure);
	GLCall(glActiveTexture(GL_TEXTURE0));
	//GLCall(glBindTexture(GL_TEXTURE_2D, m_BloomBufferRendererID));
	GLCall(glBindTexture(GL_TEXTURE_2D, m_ColorBufferRendererID));

	GLCall(glActiveTexture(GL_TEXTURE1));
	GLCall(glBindTexture(GL_TEXTURE_2D, m_Bloom->GetBlurResultRenderID()));

	//m_HDRShader->SetUniform1f("exposure",);
}

void HDR::DrawHDRresult()
{
	m_Bloom->BloomGenerate(*m_DefaultModels.DeBugViewGrid.m_VAO, m_DefaultModels.DeBugViewGrid.vertices.size());
	SetUniform();
	Renderer::Get().DrawGrid(*m_DefaultModels.DeBugViewGrid.m_VAO, *m_HDRShader, m_DefaultModels.DeBugViewGrid.vertices.size());
}

void HDR::ImGuiRender()
{
	if (ImGui::CollapsingHeader("Color management"))
	{
		ImGui::Checkbox("IDT(sRGB to ACEScg)", &IDT);
		ImGui::SameLine();
		ImGui::Checkbox("RRT(ACES S-Curve Tone Mapping)", &RRT);
		ImGui::SameLine();
		ImGui::Checkbox("ODT(ACEScg to sRGB)", &ODT);
		ImGui::SameLine();
		ImGui::Checkbox("EOTF(gamma 2.2 correct)", &EOTF);
		ImGui::SliderFloat("Exposure", &exposure, 0.0f, 1.0f);
	}

	if (ImGui::CollapsingHeader("Bloom Setting"))
	{
		ImGui::Checkbox("Enable Bloom", &enableBloom);
		ImGui::SliderInt("Glow Intensity",m_Bloom->GetAmountAddress(), 0, 100);
		m_Bloom->MakeAmountSafe();
	}
}