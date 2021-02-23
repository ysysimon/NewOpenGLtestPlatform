#pragma once

#include "Test.h"
#include <memory>

#include "Renderer.h"
#include "imgui/imgui.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "PerspectiveCamera.h"


namespace test {


	struct TestBasicLight_Material : testMaterial {
		TestBasicLight_Material(glm::vec3 ambientColor, glm::vec3 diffuseColor, glm::vec3 specularColor, float shininess)
		{
			this->ambientColor = ambientColor;
			this->diffuseColor = diffuseColor;
			this->specularColor = specularColor;
			this->shininess = shininess;
		}

		glm::vec3 ambientColor;
		glm::vec3 diffuseColor;
		glm::vec3 specularColor;
		float shininess;
	};
	

	struct TestBasicLight_Light : testLight {
		TestBasicLight_Light(glm::vec3 pos, glm::vec3 color, float ambientCoefficient, float diffuseCoefficient, float specularCoefficient)
		{
			this->position = pos;
			this->color = color;
			this->ambientCoefficient = ambientCoefficient;
			this->diffuseCoefficient = diffuseCoefficient;
			this->specularCoefficient = specularCoefficient;
		}


		glm::vec3 position;
		glm::vec3 color;

		float ambientCoefficient;
		float diffuseCoefficient;
		float specularCoefficient;
	};
	

	class TestBasicLight : public Test
	{
	public:
		TestBasicLight(GLFWwindow*& window, float WindowResolutionX, float WindowResolutionY);
		~TestBasicLight();
			
		void OnUpdate(float deltaTime)  override;
		void OnRender() override;
		void OnImGuiRender() override;

	private:
		std::unique_ptr<VertexArray> m_cubeVAO;
		std::unique_ptr<VertexArray> m_lightVAO;
		//std::unique_ptr<IndexBuffer> m_IndexBuffer;
		std::unique_ptr<Shader> m_cubeShader;
		std::unique_ptr<Shader> m_lightShader;
		//std::unique_ptr<Texture> m_Texture;
		std::unique_ptr<VertexBuffer> m_VertexBuffer;
		glm::vec3 m_CubePosition;
		
		PerspectiveCamera m_Camera;

		TestBasicLight_Light m_Light;
		TestBasicLight_Material m_Material;

	};

	
	
}