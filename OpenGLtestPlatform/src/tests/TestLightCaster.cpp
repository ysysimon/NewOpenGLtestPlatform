#include "TestLightCaster.h"
#include "VertexBufferLayout.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
namespace test {

	TestLightCaster::TestLightCaster(GLFWwindow*& window, float WindowResolutionX, float WindowResolutionY)
		:m_Camera(window, WindowResolutionX, WindowResolutionY),
		m_PointLight(glm::vec3(-0.2f, -1.0f, -8.0f), glm::vec3(1.0f, 1.0f, 1.0f), 0.2f, 0.5f, 1.0f, 0.045f, 0.0075f),
		m_DistantLight(glm::vec3(-5.0f, -0.0f, -7.426f), glm::vec3(0.349f, 0.255f, 0.165f), 0.2f, 0.5f, 1.0f),
		m_Spotlight(m_Camera.GetCameraPos(),glm::vec3(1.0f, 1.0f, 1.0f), 0.2f, 0.5f, 1.0f, 12.5f, 17.5f,m_Camera.GetCameraTarget()),
		m_CubePosition(0.0f,0.0f,0.0f), m_PointLightAttenuationNum(6),
		m_EnablePointLight(true), m_EnableDistantLight(true),m_EnableSpotLight(true),
		m_SpotLightCutOffAngleSafeLock(true)
	{
		float vertices[] = {
			// positions          // normals           // texture coords
			-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
			 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
			 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
			 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
			-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
			-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

			-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
			 0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
			 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
			 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
			-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
			-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

			-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
			-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
			-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
			-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
			-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
			-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

			 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
			 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
			 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
			 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
			 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
			 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

			-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
			 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
			 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
			 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
			-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

			-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
			 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
			 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
			 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
			-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
			-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
		};

		
		m_VertexBuffer = std::make_unique<VertexBuffer>(vertices, sizeof(vertices));
		VertexBufferLayout layout;
		layout.Push<float>(3);
		layout.Push<float>(3);
		layout.Push<float>(2);
		

		m_cubeVAO = std::make_unique<VertexArray>();
		m_cubeVAO->AddBuffer(*m_VertexBuffer, layout);

		m_lightVAO = std::make_unique<VertexArray>();
		m_lightVAO->AddBuffer(*m_VertexBuffer, layout);

		/*m_IndexBuffer = std::make_unique<IndexBuffer>(indices, 36);*/
		m_cubeShader = std::make_unique<Shader>("res/shaders/TestLightCaster/CubeMultiLight.shader");
		m_lightShader = std::make_unique<Shader>("res/shaders/TestLightCaster/Light.shader");
		m_Material = std::make_unique<TestLightCaster_Material>("res/textures/TestLightCaster/container2.png", "res/textures/TestLightCaster/container2_specular.png",32);
		
		m_cubeShader->Bind();
		m_cubeShader->SetUniform1i("material.diffuseColorMap",0);
		m_cubeShader->SetUniform1i("material.specularColorMap", 1);
		
		m_Camera.SetCameraPos(glm::vec3(-4.0f, -1.2f,  5.7f));
		m_Camera.SetCameraTarget(glm::vec3( 0.75f,  0.18f, -0.63f));
	}
	
	TestLightCaster::~TestLightCaster()
	{

	}

	void TestLightCaster::OnUpdate(float deltaTime)
	{
		m_Camera.ProcessKeybordInput(deltaTime);
	}

	void TestLightCaster::OnRender() 
	{
		

		m_Material->diffuseColorMap.Bind(0);
		m_Material->specularColorMap.Bind(1);

		glm::mat4 projection = glm::mat4(1.0f);
		glm::mat4 model = glm::mat4(1.0f);
		projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
		

		m_cubeShader->Bind();
		//m_cubeShader->SetUniformMat4f("model", glm::translate(model, m_CubePosition));
		m_cubeShader->SetUniformMat4f("view", m_Camera.GetViewMatrix());
		m_cubeShader->SetUniformMat4f("projection", m_Camera.GetProjectionMatrix());

		m_cubeShader->SetUniform3v("viewPos", m_Camera.GetCameraPos());
		m_cubeShader->SetUniform1f("material.shininess", m_Material->shininess);

		m_cubeShader->SetUniform1i("EnablePointLight", m_EnablePointLight); 
		m_cubeShader->SetUniform1i("EnableDistantLight", m_EnableDistantLight);
		m_cubeShader->SetUniform1i("EnableSpotLight", m_EnableSpotLight);
		if (m_EnableDistantLight)
		{
			m_cubeShader->SetUniform3v("distantLight.color", m_DistantLight.color);
			m_cubeShader->SetUniform1f("distantLight.ambientCoefficient", m_DistantLight.ambientCoefficient);
			m_cubeShader->SetUniform1f("distantLight.diffuseCoefficient", m_DistantLight.diffuseCoefficient);
			m_cubeShader->SetUniform1f("distantLight.specularCoefficient", m_DistantLight.specularCoefficient);
			m_cubeShader->SetUniform3v("distantLight.direction", m_DistantLight.direction);
		}
		
		if (m_EnablePointLight)
		{
			m_cubeShader->SetUniform3v("pointLight.color", m_PointLight.color);
			m_cubeShader->SetUniform1f("pointLight.ambientCoefficient", m_PointLight.ambientCoefficient);
			m_cubeShader->SetUniform1f("pointLight.diffuseCoefficient", m_PointLight.diffuseCoefficient);
			m_cubeShader->SetUniform1f("pointLight.specularCoefficient", m_PointLight.specularCoefficient);
			m_cubeShader->SetUniform3v("pointLight.position", m_PointLight.position);
			m_cubeShader->SetUniform1f("pointLight.K", PointLight::K);
			m_cubeShader->SetUniform1f("pointLight.linear", m_PointLight.linear);
			m_cubeShader->SetUniform1f("pointLight.quadratic", m_PointLight.quadratic);
		}
		
		if (m_EnableSpotLight)
		{
			m_cubeShader->SetUniform3v("spotLight.color", m_Spotlight.color);
			m_cubeShader->SetUniform1f("spotLight.ambientCoefficient", m_Spotlight.ambientCoefficient);
			m_cubeShader->SetUniform1f("spotLight.diffuseCoefficient", m_Spotlight.diffuseCoefficient);
			m_cubeShader->SetUniform1f("spotLight.specularCoefficient", m_Spotlight.specularCoefficient);
			m_cubeShader->SetUniform3v("spotLight.position", m_Camera.GetCameraPos());
			m_Spotlight.position = m_Camera.GetCameraPos();
			m_cubeShader->SetUniform1f("spotLight.cutoffAngleCos", glm::cos(glm::radians(m_Spotlight.cutoffAngle)));
			m_cubeShader->SetUniform1f("spotLight.outerCutOffAngleCos", glm::cos(glm::radians(m_Spotlight.outerCutOffAngle)));
			m_cubeShader->SetUniform3v("spotLight.spotDirection", m_Camera.GetCameraTarget());
			m_Spotlight.spotDirection = m_Camera.GetCameraTarget();
		}

		

		glm::vec3 cubePositions[] = {
			glm::vec3(0.0f,  0.0f,  0.0f),
			glm::vec3(2.0f,  5.0f, -15.0f),
			glm::vec3(-1.5f, -2.2f, -2.5f),
			glm::vec3(-3.8f, -2.0f, -12.3f),
			glm::vec3(2.4f, -0.4f, -3.5f),
			glm::vec3(-1.7f,  3.0f, -7.5f),
			glm::vec3(1.3f, -2.0f, -2.5f),
			glm::vec3(1.5f,  2.0f, -2.5f),
			glm::vec3(1.5f,  0.2f, -1.5f),
			glm::vec3(-1.3f,  1.0f, -1.5f)
		};

		for (unsigned int i = 0; i < 10; i++)
		{
			model = glm::mat4(1.0f);
			model = glm::translate(model, cubePositions[i]);
			float angle = 20.0f * i;
			model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			m_cubeShader->SetUniformMat4f("model", model);


			Renderer::Get().DrawCube(*m_cubeVAO, *m_cubeShader);
		}


		if (m_EnablePointLight)
		{
			model = glm::mat4(1.0f);
			m_lightShader->Bind();
			m_lightShader->SetUniformMat4f("model", glm::translate(model, m_PointLight.position));
			m_lightShader->SetUniformMat4f("view", m_Camera.GetViewMatrix());
			m_lightShader->SetUniformMat4f("projection", m_Camera.GetProjectionMatrix());
			m_lightShader->SetUniform3v("lightColor", m_PointLight.color);
			Renderer::Get().DrawCube(*m_lightVAO, *m_lightShader);
		}
		

		

		
		
		
		//renderer.DrawCube(*m_cubeVAO, *m_cubeShader);
	}


		



	void TestLightCaster::OnImGuiRender()
	{
		ImGui::SameLine();
		ImGui::Text("Average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::Text("Press ESC  to Escape the Mouse");
		ImGui::Text("Press HOME to Recapture the Mouse");

		ImGui::Text("CameraPos:    X= %f Y= %f Z= %f", m_Camera.GetCameraPos().x, m_Camera.GetCameraPos().y, m_Camera.GetCameraPos().z);
		ImGui::Text("CameraTarget: X=  %f Y=  %f Z= %f", m_Camera.GetCameraTarget().x, m_Camera.GetCameraTarget().y, m_Camera.GetCameraTarget().z);

		//ImGui::SliderFloat3("Cube Position", (float*)&m_CubePosition, -10.0f, 10.0f);
		
		if (ImGui::CollapsingHeader("Light Setting"))
		{
			if (ImGui::TreeNode("Point Light"))
			{
				ImGui::Checkbox("Enable Point Light", &m_EnablePointLight);
				ImGui::SliderFloat3("Light Position", (float*)&m_PointLight.position, -10.0f, 10.0f);
				ImGui::Combo("m Attenuation", &m_PointLightAttenuationNum, PointLight::attenuationDistance, 10);
				m_PointLight.SetAttenuationDistance(m_PointLightAttenuationNum);
				ImGui::SliderFloat("Ambient Coefficient", &m_PointLight.ambientCoefficient, 0.0f, 1.0f);
				ImGui::SliderFloat("Diffuse Coefficient", &m_PointLight.diffuseCoefficient, 0.0f, 1.0f);
				ImGui::SliderFloat("specular Coefficient", &m_PointLight.specularCoefficient, 0.0f, 1.0f);
				ImGui::ColorEdit3("Light Color", (float*)&m_PointLight.color);

				ImGui::TreePop();
			}
			if (ImGui::TreeNode("Distant Light"))
			{
				ImGui::Checkbox("Enable Distant Light", &m_EnableDistantLight);
				ImGui::SliderFloat3("Light Direction", (float*)&m_DistantLight.direction, -50.0f, 50.0f);
				ImGui::SliderFloat("Ambient Coefficient", &m_DistantLight.ambientCoefficient, 0.0f, 1.0f);
				ImGui::SliderFloat("Diffuse Coefficient", &m_DistantLight.diffuseCoefficient, 0.0f, 1.0f);
				ImGui::SliderFloat("specular Coefficient", &m_DistantLight.specularCoefficient, 0.0f, 1.0f);
				ImGui::ColorEdit3("Light Color", (float*)&m_DistantLight.color);

				ImGui::TreePop();
			}
			if (ImGui::TreeNode("Spot Light"))
			{
				ImGui::Checkbox("Enable Spot Light", &m_EnableSpotLight);
				ImGui::SameLine();
				ImGui::Checkbox("CutOff Angle Safe Lock", &m_SpotLightCutOffAngleSafeLock);
				ImGui::SliderFloat("Ambient Coefficient", &m_Spotlight.ambientCoefficient, 0.0f, 1.0f);
				ImGui::SliderFloat("Diffuse Coefficient", &m_Spotlight.diffuseCoefficient, 0.0f, 1.0f);
				ImGui::SliderFloat("specular Coefficient", &m_Spotlight.specularCoefficient, 0.0f, 1.0f);
				ImGui::ColorEdit3("Light Color", (float*)&m_Spotlight.color); 
				float currentCutoffAngle = m_Spotlight.cutoffAngle;
				ImGui::SliderFloat("CutOff Angle", &m_Spotlight.cutoffAngle, 0.0f, 90.0f);
				if (m_SpotLightCutOffAngleSafeLock && (m_Spotlight.cutoffAngle > m_Spotlight.outerCutOffAngle))
				{
					m_Spotlight.cutoffAngle = currentCutoffAngle;
				}
				float currentOuterCutOffAngle = m_Spotlight.outerCutOffAngle;
				ImGui::SliderFloat("Out CutOff Angle", &m_Spotlight.outerCutOffAngle, 0.0f, 90.0f);
				if (m_SpotLightCutOffAngleSafeLock && (m_Spotlight.cutoffAngle > m_Spotlight.outerCutOffAngle))
				{
					m_Spotlight.outerCutOffAngle = currentOuterCutOffAngle;
				}
				ImGui::Text("Spot Pos:    X= %f Y= %f Z= %f", m_Spotlight.position.x, m_Spotlight.position.y, m_Spotlight.position.z);
				ImGui::Text("Spot Direction: X=  %f Y=  %f Z= %f", m_Spotlight.spotDirection.x, m_Spotlight.spotDirection.y, m_Spotlight.spotDirection.z);

				ImGui::TreePop();
			}
			
		}

		if (ImGui::CollapsingHeader("Material Setting"))
		{
			ImGui::SliderFloat("Shininess", &m_Material->shininess, 0.0f, 500.0f);
			
			//ImGui::ColorEdit3("Ambient Color", (float*)&m_Material.ambientColor);

			
			//ImGui::ColorEdit3("Diffuse Color", (float*)&m_Material.diffuseColor);

			
			//ImGui::ColorEdit3("Specular Color", (float*)&m_Material.specularColor);
		}
		
	}

	void TestLightCaster::SetLightUniform()
	{

	}

}