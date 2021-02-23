#include "TestBasicTexture.h"
#include "VertexBufferLayout.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
namespace test {

	TestBasicTexture::TestBasicTexture(GLFWwindow*& window, float WindowResolutionX, float WindowResolutionY)
		:m_Camera(window, WindowResolutionX, WindowResolutionY),
		m_Light(glm::vec3(1.2f, 1.0f, 2.0f), glm::vec3(1.0f, 1.0f, 1.0f),0.2f,0.5f,1.0f),
		m_CubePosition(0.0f,0.0f,0.0f)
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
		m_cubeShader = std::make_unique<Shader>("res/shaders/TestBasicTexture/Cube.shader");
		m_lightShader = std::make_unique<Shader>("res/shaders/TestBasicTexture/Light.shader");
		m_Material = std::make_unique<TestBasicTexture_Material>("res/textures/TestBasicTexture/container2.png", "res/textures/TestBasicTexture/container2_specular.png",32);
		
		m_cubeShader->Bind();
		m_cubeShader->SetUniform1i("material.diffuseColorMap",0);
		m_cubeShader->SetUniform1i("material.specularColorMap", 1);
		
		m_Camera.SetCameraPos(glm::vec3(-4.0f, -1.2f,  5.7f));
		m_Camera.SetCameraTarget(glm::vec3( 0.75f,  0.18f, -0.63f));
	}

	TestBasicTexture::~TestBasicTexture()
	{

	}

	void TestBasicTexture::OnUpdate(float deltaTime)
	{
		m_Camera.ProcessKeybordInput(deltaTime);
	}

	void TestBasicTexture::OnRender() 
	{
		

		m_Material->diffuseColorMap.Bind(0);
		m_Material->specularColorMap.Bind(1);

		//glm::mat4 view = glm::mat4(1.0f); 
		glm::mat4 projection = glm::mat4(1.0f);
		glm::mat4 model = glm::mat4(1.0f);
		//model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(1.0f, 0.3f, 0.5f));
		projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
		//view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));

		m_cubeShader->Bind();
		m_cubeShader->SetUniformMat4f("model", glm::translate(model, m_CubePosition));
		m_cubeShader->SetUniformMat4f("view" ,  m_Camera.GetViewMatrix());
		m_cubeShader->SetUniformMat4f("projection", m_Camera.GetProjectionMatrix());
		m_cubeShader->SetUniform3v("light.color", m_Light.color);
		m_cubeShader->SetUniform3v("light.position", m_Light.position);
		m_cubeShader->SetUniform1f("light.ambientCoefficient",m_Light.ambientCoefficient);
		m_cubeShader->SetUniform1f("light.diffuseCoefficient",m_Light.diffuseCoefficient);
		m_cubeShader->SetUniform1f("light.specularCoefficient",m_Light.specularCoefficient);
		//m_cubeShader->SetUniform3v("material.ambientColor",m_Material.ambientColor);
		//m_cubeShader->SetUniform3v("material.diffuseColor", m_Material.diffuseColor);
		//m_cubeShader->SetUniform3v("material.specularColor", m_Material->specularColor);
		m_cubeShader->SetUniform1f("material.shininess",m_Material->shininess);
		m_cubeShader->SetUniform3v("viewPos", m_Camera.GetCameraPos());


		m_lightShader->Bind();
		m_lightShader->SetUniformMat4f("model", glm::translate(model, m_Light.position));
		m_lightShader->SetUniformMat4f("view", m_Camera.GetViewMatrix());
		m_lightShader->SetUniformMat4f("projection", m_Camera.GetProjectionMatrix());
		m_lightShader->SetUniform3v("lightColor", m_Light.color);

		Renderer::Get().DrawCube(*m_cubeVAO, *m_cubeShader);
		Renderer::Get().DrawCube(*m_lightVAO, *m_lightShader);
	}

	void TestBasicTexture::OnImGuiRender()
	{
		ImGui::SameLine();
		ImGui::Text("Average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::Text("Press ESC  to Escape the Mouse");
		ImGui::Text("Press HOME to Recapture the Mouse");

		ImGui::Text("CameraPos:    X= %f Y= %f Z= %f", m_Camera.GetCameraPos().x, m_Camera.GetCameraPos().y, m_Camera.GetCameraPos().z);
		ImGui::Text("CameraTarget: X=  %f Y=  %f Z= %f", m_Camera.GetCameraTarget().x, m_Camera.GetCameraTarget().y, m_Camera.GetCameraTarget().z);

		ImGui::SliderFloat3("Cube Position", (float*)&m_CubePosition, -10.0f, 10.0f);
		
		if (ImGui::CollapsingHeader("Light Setting"))
		{
			ImGui::SliderFloat3("Light Position",(float*)&m_Light.position,-10.0f,10.0f);
			ImGui::SliderFloat("Ambient Coefficient", &m_Light.ambientCoefficient, 0.0f, 1.0f);
			ImGui::SliderFloat("Diffuse Coefficient", &m_Light.diffuseCoefficient, 0.0f, 1.0f);
			ImGui::SliderFloat("specular Coefficient", &m_Light.specularCoefficient, 0.0f, 1.0f);

			ImGui::ColorEdit3("Light Color",(float*)&m_Light.color);
		}

		if (ImGui::CollapsingHeader("Material Setting"))
		{
			ImGui::SliderFloat("Shininess", &m_Material->shininess, 0.0f, 500.0f);
			
			//ImGui::ColorEdit3("Ambient Color", (float*)&m_Material.ambientColor);

			
			//ImGui::ColorEdit3("Diffuse Color", (float*)&m_Material.diffuseColor);

			
			//ImGui::ColorEdit3("Specular Color", (float*)&m_Material.specularColor);
		}
		
	}

}