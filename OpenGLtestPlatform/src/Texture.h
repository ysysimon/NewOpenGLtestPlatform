#pragma once

#include "Renderer.h"

enum TextureType {
	None,
	Diffuse,
	Specular,
	Height = 5,
	Normal = 6
};

static std::string GetTextureTypeName(TextureType textureType)
{
	switch (textureType)
	{
	case None:
		return "None";
		break;
	case Diffuse:
		return "Diffuse";
		break;
	case Specular:
		return "Specular";
		break;
	case Height:
		return "Normal";
		break;
	case Normal:
		return "Normal";
		break;
	default:
		break;
	}
}

class Texture
{
public:
	Texture(const std::string& path);
	Texture(const std::string& path, TextureType type,const std::string& TextureName);
	Texture() {};
	~Texture();

	void Bind(unsigned int slot = 0) const;
	void Unbind() const;
	void ImGuiRender();
	

	inline int GetWidth() const { return m_Width; }
	inline int GetHeight() const { return m_Height; }
	//inline int GetType() const { return Type; }
	inline std::string GetName() const { return m_TextureName; }
	inline std::string GetPath() const { return m_FilePath; }

	TextureType Type = TextureType::None;
	void ChangeTexture(const std::string& path);
	char TextBox_Buffer[100];
	std::string m_FilePath;
private:
	
	std::string m_TextureName = "";
	unsigned int m_RendererID;
	
	unsigned char* m_LocalBuffer;
	int m_Width, m_Height, m_BPP;

	
	void LoadTexture();
	

};

