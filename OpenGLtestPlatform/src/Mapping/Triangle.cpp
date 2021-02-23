#include "Triangle.h"

Triangle::Triangle()
{
}
Triangle::~Triangle()
{
}

Triangle::Triangle(glm::vec3 positions[3], glm::vec2 uvs[3])
{
	pos1 = positions[0];
	pos2 = positions[1];
	pos3 = positions[2];

	uv1 = uvs[0];
	uv2 = uvs[1];
	uv3 = uvs[2];

	CalculateTBN();
}

void Triangle::CalculateTBN()
{
	glm::vec3 edge1 = pos2 - pos1;
	glm::vec3 edge2 = pos3 - pos1;
	glm::vec2 deltaUV1 = uv2 - uv1;
	glm::vec2 deltaUV2 = uv3 - uv1;

	float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

	Ttangent.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
	Ttangent.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
	Ttangent.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
	Ttangent = glm::normalize(Ttangent);

	Btangent.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
	Btangent.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
	Btangent.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);
	Btangent = glm::normalize(Btangent);
}