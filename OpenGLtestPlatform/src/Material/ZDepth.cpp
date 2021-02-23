#include "ZDepth.h"

const char* ZDepth::ProjectionType[] = { "Orthogonal(Non-Linear)","Perspective(Linear)" };

void ZDepth::GenerateDepthMap(Camera& camera, glm::mat4& SpaceMatrix, std::vector<std::tuple<std::string,glm::vec3, float>>& ModelMatrixFactor, std::vector<std::function<void(Shader&)>>& DrawCalls)
{
	m_DepthMap.GenerateDepthMap(camera, SpaceMatrix, ModelMatrixFactor,DrawCalls);
	m_DebugViewGrid.SetUniform(camera.GetNearPlane(), camera.GetFarPlane());
}

void ZDepth::Draw()
{
	m_DepthMap.Bind();
	m_DebugViewGrid.Draw();
}

void ZDepth::ImGuiRender()
{
	if (ImGui::TreeNode("Z-Depth Setting"))
	{
		ImGui::Combo("ProjectionType", &m_ProjectionTypeIndex, ZDepth::ProjectionType, 2);

		switch (m_ProjectionTypeIndex)
		{
		case 0:
			m_DebugViewGrid.SetProjectionType(ProjectionType::Orthogonal);
			break;
		case 1:
			m_DebugViewGrid.SetProjectionType(ProjectionType::Perspective);
			break;
		default:
			break;
		}

		ImGui::TreePop();
	}
}

Shader& ZDepth::GetShader() const
{
	return *dummyShader;
}