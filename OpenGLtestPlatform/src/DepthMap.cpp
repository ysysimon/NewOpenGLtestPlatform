#include "DepthMap.h"

const unsigned int DepthMap::SHADOW_HEIGHT = 1024;
const unsigned int DepthMap::SHADOW_WIDTH  = 1024;
DepthMap::DepthMap()
{
	DepthShader = std::make_unique<Shader>("res/shaders/Depth.shader");
	glGenTextures(1, &m_RendererID);
	glBindTexture(GL_TEXTURE_2D, m_RendererID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
		DepthMap::SHADOW_WIDTH, DepthMap::SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	GLfloat borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

	m_frameBuffer.Bind();
	m_frameBuffer.AttachTexture(GL_DEPTH_ATTACHMENT,m_RendererID);
	GLCall(glDrawBuffer(GL_NONE));
	GLCall(glReadBuffer(GL_NONE));
	m_frameBuffer.Unbind();
}

DepthMap::~DepthMap()
{
	GLCall(glDeleteTextures(1, &m_RendererID));
}

void DepthMap::PreProcess()
{
	if (IsShadow)
	{
		GLCall(glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT));
		//GLCall(glEnable(GL_CULL_FACE));
		//GLCall(glCullFace(GL_FRONT));
	}
	m_frameBuffer.Bind();
	glClear(GL_DEPTH_BUFFER_BIT);
}

void DepthMap::SetUniform(Camera& camera, glm::mat4& SpaceMatrix, glm::mat4& model)
{
	DepthShader->Bind();
	DepthShader->SetUniformMat4f("SpaceMatrix", SpaceMatrix);
	DepthShader->SetUniformMat4f("model", model);
}

void DepthMap::GenerateDepthMap(Camera& camera, glm::mat4& SpaceMatrix,std::vector<std::tuple<std::string, glm::vec3, float>>& ModelMatrixFactor,std::vector<std::function<void(Shader&)>>& DrawCalls)
{
	SetSCRsize(camera.GetWindowResolutionX(), camera.GetWindowResolutionY());
	this->PreProcess();
	for (int i = 0; i < DrawCalls.size(); i++)
	{
		auto& [Name, ModelScale, UniformModelScale] = ModelMatrixFactor[i];
		glm::mat4 temp = glm::scale(glm::mat4(1.0f), ModelScale * UniformModelScale);
		this->SetUniform(camera, SpaceMatrix, temp);
		DrawCalls[i](*(this->DepthShader));
	}
	this->PostProcess();
}

void DepthMap::PostProcess()
{
	m_frameBuffer.Unbind();
	GLCall(glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT));
	//GLCall(glCullFace(GL_BACK));
	//GLCall(glDisable(GL_CULL_FACE));
	GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
}

void DepthMap::SetSCRsize(float SCR_WIDTH, float SCR_HEIGHT)
{
	this->SCR_WIDTH = SCR_WIDTH;
	this->SCR_HEIGHT = SCR_HEIGHT;
}

void DepthMap::Bind(unsigned int slot)
{
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, m_RendererID);
}

void DepthMap::UnBind()
{
	glBindTexture(GL_TEXTURE_2D, 0);
}