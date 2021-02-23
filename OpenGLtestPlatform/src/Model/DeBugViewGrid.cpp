#include "Model/DeBugViewGrid.h"

DebugViewGrid::DebugViewGrid()
{
	DeBugGridShader = std::make_unique<Shader>("res/shaders/DeBugGrid.shader");
}

DebugViewGrid::~DebugViewGrid()
{
}

void DebugViewGrid::SetUniform(float near_plane, float far_plane)
{
	DeBugGridShader->Bind();
	DeBugGridShader->SetUniform1f("near_plane", near_plane);
	DeBugGridShader->SetUniform1f("far_plane", far_plane);
	DeBugGridShader->SetUniform1i("depthMap", 0);
	DeBugGridShader->SetUniform1i("ProjectionType", this->projectionType);
}

void DebugViewGrid::SetUniform(float near_plane, float far_plane, ProjectionType projectionType)
{
	this->projectionType = projectionType;
	DeBugGridShader->Bind();
	DeBugGridShader->SetUniform1f("near_plane", near_plane);
	DeBugGridShader->SetUniform1f("far_plane", far_plane);
	DeBugGridShader->SetUniform1i("depthMap", 0);
	DeBugGridShader->SetUniform1i("ProjectionType", this->projectionType);
}


void DebugViewGrid::Draw()
{
	Renderer::Get().DrawGrid(*m_DefaultModels.DeBugViewGrid.m_VAO, *DeBugGridShader, m_DefaultModels.DeBugViewGrid.vertices.size());
}