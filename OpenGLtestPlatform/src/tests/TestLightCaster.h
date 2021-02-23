#pragma once

#include "Test.h"
#include <memory>

#include "Renderer.h"
#include "imgui/imgui.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "PerspectiveCamera.h"
#include "LightCaster/LightCaster.h"

namespace test {
		

	 struct TestLightCaster_Material : testMaterial {
		 TestLightCaster_Material(const std::string& diffusePath, const std::string& specularPath, float shininess)
			:diffuseColorMap(diffusePath), specularColorMap(specularPath)
		{
			//this->specularColor = specularColor;
			this->shininess = shininess;
		}

		Texture diffuseColorMap;
		Texture specularColorMap;
		float shininess;
	};

	class TestLightCaster : public Test
	{
	public:
		TestLightCaster(GLFWwindow*& window, float WindowResolutionX, float WindowResolutionY);
		~TestLightCaster();
		
		void OnUpdate(float deltaTime)  override;
		void OnRender() override;
		void OnImGuiRender() override;

		void SetLightUniform();
	private:
		std::unique_ptr<VertexArray> m_cubeVAO;
		std::unique_ptr<VertexArray> m_lightVAO;
		std::unique_ptr<IndexBuffer> m_IndexBuffer;
		std::unique_ptr<Shader> m_cubeShader;
		std::unique_ptr<Shader> m_lightShader;
		std::unique_ptr<VertexBuffer> m_VertexBuffer;
		glm::vec3 m_CubePosition;
		
		PerspectiveCamera m_Camera;

		PointLight m_PointLight;
		DistantLight m_DistantLight;
		SpotLight m_Spotlight;
		std::unique_ptr<TestLightCaster_Material> m_Material;

		int m_PointLightAttenuationNum;

		bool m_EnablePointLight;
		bool m_EnableDistantLight;
		bool m_EnableSpotLight;

		bool m_SpotLightCutOffAngleSafeLock;

	};

	
	
}