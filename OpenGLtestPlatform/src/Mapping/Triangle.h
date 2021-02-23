#pragma once
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

class Triangle
{
public:
	Triangle(glm::vec3 positions[3], glm::vec2 uvs[3]);
	Triangle();
	~Triangle();

	inline glm::vec3 GetPos1() { return pos1; }
	inline glm::vec3 GetPos2() { return pos2; }
	inline glm::vec3 GetPos3() { return pos3; }

	inline glm::vec2 GetUV1() { return uv1; }
	inline glm::vec2 GetUV2() { return uv2; }
	inline glm::vec2 GetUV3() { return uv3; }

	inline glm::vec3 GetTtangent() { return Ttangent; }
	inline glm::vec3 GetBtangent() { return Btangent; }

private:
	glm::vec3 pos1;
	glm::vec3 pos2;
	glm::vec3 pos3;

	glm::vec2 uv1;
	glm::vec2 uv2;
	glm::vec2 uv3;

	glm::vec3 Ttangent;
	glm::vec3 Btangent;

	void CalculateTBN();
};
