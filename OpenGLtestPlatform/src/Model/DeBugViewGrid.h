#pragma once
#include <memory>
#include "Shader.h"
#include "Model/DefaultModels.h"

enum ProjectionType
{
	Orthogonal,
	Perspective
};

class DebugViewGrid
{
public:
	DebugViewGrid();
	~DebugViewGrid();

	void SetUniform(float near_plane,float far_plane);
	void SetUniform(float near_plane, float far_plane, ProjectionType projectionType);
	void Draw();

	inline void SetProjectionType(ProjectionType Type)
	{
		this->projectionType = Type;
	}

private:
	std::unique_ptr<Shader> DeBugGridShader;
	DefaultModels m_DefaultModels;
	enum ProjectionType projectionType = ProjectionType::Perspective;
};
