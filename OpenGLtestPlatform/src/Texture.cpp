#include "Texture.h"

#include <iostream>
#include<stdlib.h>

#include "stb_image/stb_image.h"
#include "imgui/imgui.h"

Texture::Texture(const std::string& path)
	: m_RendererID(0),m_FilePath(path),m_LocalBuffer(nullptr),m_Width(0),m_Height(0),m_BPP(0)
{
	strcpy_s(TextBox_Buffer,path.size()+1, path.c_str());
	LoadTexture();
}
Texture::Texture(const std::string& path, TextureType type, const std::string& TextureName)
	: m_RendererID(0), m_FilePath(path), m_LocalBuffer(nullptr), m_Width(0), m_Height(0), m_BPP(0),Type(type),m_TextureName(TextureName)
{
	strcpy_s(TextBox_Buffer, path.size() + 1, path.c_str());
	LoadTexture();
}
void Texture::LoadTexture()
{
	stbi_set_flip_vertically_on_load(1);
	m_LocalBuffer = stbi_load(m_FilePath.c_str(), &m_Width, &m_Height, &m_BPP, 4);
	//Í¨µÀÊý
	if (m_LocalBuffer)
	{
		GLCall(glGenTextures(1, &m_RendererID));
		GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));

		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));

		if (Type == TextureType::Diffuse) //gamma
		{
			GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB_ALPHA, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_LocalBuffer));
		}
		else
		{
			GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_LocalBuffer));
		}

		glGenerateMipmap(GL_TEXTURE_2D);
		GLCall(glBindTexture(GL_TEXTURE_2D, 0));
		stbi_image_free(m_LocalBuffer);
		std::cout << "Load Texture Successed (" << Type << ") :" << m_TextureName << std::endl;
	}
	else
	{
		std::cout << "Failed to load texture (" << Type << ") :" << m_TextureName << std::endl;
	}
}
void Texture::ChangeTexture(const std::string& path)
{
	stbi_set_flip_vertically_on_load(1);
	m_LocalBuffer = stbi_load(path.c_str(), &m_Width, &m_Height, &m_BPP, 4);
	if (m_LocalBuffer)
	{
		GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));

		this->m_FilePath = path;

		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));

		if (Type == TextureType::Diffuse)
		{
			GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB_ALPHA, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_LocalBuffer));
		}
		else
		{
			GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_LocalBuffer));
		}
		glGenerateMipmap(GL_TEXTURE_2D);
		GLCall(glBindTexture(GL_TEXTURE_2D, 0));
		stbi_image_free(m_LocalBuffer);
		std::cout << "Load Texture Successed (" << Type << ") :" << m_TextureName << std::endl;
	}
	else
	{
		std::cout << "Failed to load texture (" << Type << ") :" << m_TextureName << std::endl;
	}
}

Texture::~Texture()
{
	GLCall(glDeleteTextures(1, &m_RendererID));
}

void Texture::Bind(unsigned int slot /*=0*/) const
{
	GLCall(glActiveTexture(GL_TEXTURE0 + slot));
	GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));
}

void Texture::Unbind() const
{
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}

void Texture::ImGuiRender()
{
	ImGui::Text((m_TextureName + ":").c_str());
	ImGui::SameLine();
	ImGui::InputText(("###" + std::to_string(m_RendererID)).c_str(),this->TextBox_Buffer, IM_ARRAYSIZE(TextBox_Buffer));
	
	
}