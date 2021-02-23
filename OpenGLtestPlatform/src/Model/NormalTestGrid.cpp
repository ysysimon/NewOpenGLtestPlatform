#include "NormalTestGrid.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "VertexBufferLayout.h"

NormalTestGrid::NormalTestGrid()
{
	m_BrickWallTextureDiffuse = std::make_unique<Texture>("res/textures/NormalTestGrid/brickwall.jpg", TextureType::Diffuse,"Brick Wall Diffuse");
	m_BrickWallTextureNormal  = std::make_unique<Texture>("res/textures/NormalTestGrid/brickwall_normal.jpg", TextureType::Normal,"Brick Wall Normal");

	// positions
	glm::vec3 pos1(-1.0, 1.0, 0.0);
	glm::vec3 pos2(-1.0, -1.0, 0.0);
	glm::vec3 pos3(1.0, -1.0, 0.0);
	glm::vec3 pos4(1.0, 1.0, 0.0);
	// texture coordinates
	glm::vec2 uv1(0.0, 1.0);
	glm::vec2 uv2(0.0, 0.0);
	glm::vec2 uv3(1.0, 0.0);
	glm::vec2 uv4(1.0, 1.0);
	// normal vector
	glm::vec3 nm(0.0, 0.0, 1.0);

	glm::vec3 positions1[] = { pos1 ,pos2, pos3 };
	glm::vec2 uvs1[] = { uv1, uv2, uv3 };

	this->triangle1 = Triangle(positions1 , uvs1);
	
	glm::vec3 positions2[] = { pos1 ,pos3, pos4 };
	glm::vec2 uvs2[] = { uv1, uv3, uv4 };
	this->triangle2 = Triangle(positions2, uvs2);

	glm::vec3 Ttangent1 = triangle1.GetTtangent();
	glm::vec3 Ttangent2 = triangle2.GetTtangent();

	float quadVertices[] = {
		// Positions                 // normal         // TexCoords         //Tangent
		pos1.x, pos1.y, pos1.z,    nm.x, nm.y, nm.z,      uv1.x, uv1.y,    Ttangent1.x,Ttangent1.y,Ttangent1.z,
		pos2.x, pos2.y, pos2.z,    nm.x, nm.y, nm.z,      uv2.x, uv2.y,    Ttangent1.x,Ttangent1.y,Ttangent1.z,
		pos3.x, pos3.y, pos3.z,    nm.x, nm.y, nm.z,      uv3.x, uv3.y,    Ttangent1.x,Ttangent1.y,Ttangent1.z,
								   				       					   
		pos1.x, pos1.y, pos1.z,    nm.x, nm.y, nm.z,      uv1.x, uv1.y,    Ttangent2.x,Ttangent2.y,Ttangent2.z,
		pos3.x, pos3.y, pos3.z,    nm.x, nm.y, nm.z,      uv3.x, uv3.y,    Ttangent2.x,Ttangent2.y,Ttangent2.z,
		pos4.x, pos4.y, pos4.z,    nm.x, nm.y, nm.z,      uv4.x, uv4.y,    Ttangent2.x,Ttangent2.y,Ttangent2.z
	};

	m_VBO = std::make_unique<VertexBuffer>(&quadVertices[0],sizeof(quadVertices));
	VertexBufferLayout layout;
	layout.Push<float>(3);
	layout.Push<float>(3);
	layout.Push<float>(2);
	layout.Push<float>(3);

	m_VAO = std::make_unique<VertexArray>();
	m_VAO->AddBuffer(*m_VBO, layout);
}

NormalTestGrid::~NormalTestGrid()
{
}

void NormalTestGrid::Draw(Shader& shader)
{
	shader.Bind();
	m_BrickWallTextureDiffuse->Bind(0);
	shader.SetUniform1i("Diffuse1",0);
	m_BrickWallTextureNormal->Bind(6);
	shader.SetUniform1i("Normal",6);

	Renderer::Get().DrawTRIANGLE(*m_VAO, shader, 6);

	m_BrickWallTextureNormal->Unbind();
}