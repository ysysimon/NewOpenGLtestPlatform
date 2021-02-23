#pragma once
#include <memory>

#include "VertexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Texture.h"
#include "Mapping/Triangle.h"

class NormalTestGrid
{
public:
	NormalTestGrid();
	~NormalTestGrid();

	void Draw(Shader& shader);
private:
	std::unique_ptr<VertexBuffer> m_VBO;
	std::unique_ptr<VertexArray> m_VAO;
	std::unique_ptr<Texture> m_BrickWallTextureDiffuse;
	std::unique_ptr<Texture> m_BrickWallTextureNormal;

	Triangle triangle1;
	Triangle triangle2;
};
