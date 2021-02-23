#pragma once

#include "Test.h"
#include <memory>

#include "Renderer.h"
#include "imgui/imgui.h"
#include "PerspectiveCamera.h"

namespace test {
	class TestCamera : public Test
	{
	public:
		TestCamera(GLFWwindow*& window, float WindowResolutionX, float WindowResolutionY);
		~TestCamera();
			
		void OnUpdate(float deltaTime)  override;
		void OnRender() override;
		void OnImGuiRender() override;

	private:
		std::unique_ptr<VertexArray> m_VAO;
		//std::unique_ptr<IndexBuffer> m_IndexBuffer;
		std::unique_ptr<Shader> m_Shader;
		std::unique_ptr<Texture> m_Texture;
		std::unique_ptr<VertexBuffer> m_VertexBuffer;

		PerspectiveCamera m_Camera;


	};

	
	
}