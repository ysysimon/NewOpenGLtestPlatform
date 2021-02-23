#include "Ground.h"

Ground::Ground()
{
	m_Texture = std::make_unique<Texture>("res/textures/Default_Texture.png", TextureType::Diffuse,"Ground Diffuse");
}

Ground::~Ground()
{
}

void Ground::Draw(Shader& shader)
{
	m_Texture->Bind();
	shader.SetUniform1i("Diffuse1", 0);
	shader.SetUniform1i("Specular1", 0);
	Renderer::Get().DrawGrid(*m_DefaultModels.Grid.m_VAO, shader,m_DefaultModels.Grid.vertices.size());
}