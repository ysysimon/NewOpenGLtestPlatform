#include "TestModelImport.h"
#include "VertexBufferLayout.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace test {

	const char* TestModelImport::MaterialType[] = { "Phong","Reflection","Refraction","Depth" };


	TestModelImport::TestModelImport(GLFWwindow*& window, float WindowResolutionX, float WindowResolutionY)
		:m_Camera(window, WindowResolutionX, WindowResolutionY),
		m_PointLight(glm::vec3(-0.2f, -1.0f, -8.0f), glm::vec3(1.0f, 1.0f, 1.0f), 0.2f, 0.5f, 1.0f, 0.045f, 0.0075f),
		m_DistantLight(glm::vec3(-5.0f, -0.0f, -7.426f), glm::vec3(0.349f, 0.255f, 0.165f), 0.2f, 0.5f, 1.0f),
		m_SpotLight(m_Camera.GetCameraPos(),glm::vec3(1.0f, 1.0f, 1.0f), 0.2f, 0.5f, 1.0f, 12.5f, 17.5f,m_Camera.GetCameraTarget()),
		m_DefaultModels(), m_model(""), /*m_modelScale(1.0f, 1.0f, 1.0f),*/
		m_SkyBox("res/textures/EnvMap/skybox/", faces),
		m_selectMaterial(new Phong()), m_HDR(m_Camera.GetWindowResolutionX(),m_Camera.GetWindowResolutionY())
	{
		m_lightShader = std::make_unique<Shader>("res/shaders/TestModelImport/Light.shader");

		m_Camera.SetCameraPos(glm::vec3(-4.0f, -1.2f,  5.7f));
		m_Camera.SetCameraTarget(glm::vec3( 0.75f,  0.18f, -0.63f));

		PushDrawCall<Model>(m_model,"Import Model");
		PushDrawCall<Ground>(m_Ground,"Ground");
		PushDrawCall<NormalTestGrid>(m_NormalTestGrid,"Normal Test Grid");
	}
	
	TestModelImport::~TestModelImport()
	{
		delete m_selectMaterial;
	}

	void TestModelImport::OnUpdate(float deltaTime)
	{
		m_Camera.ProcessKeybordInput(deltaTime);
	}

	void TestModelImport::OnRender() 
	{
		
		//m_DefaultModelMatrix = glm::scale(m_modelM, m_modelScale * m_UniformModelScale);
		
		m_HDR.PreProcess();

		MaterialToDraw(m_MaterialTypeIndex);

		//Render Point Light Cube
		if (m_PointLight.EnablePointLight)
		{
			m_DefaultModelMatrix = glm::mat4(1.0f);
			m_lightShader->Bind();
			m_lightShader->SetUniformMat4f("model", glm::translate(m_DefaultModelMatrix, m_PointLight.position));
			m_lightShader->SetUniformMat4f("view", m_Camera.GetViewMatrix());
			m_lightShader->SetUniformMat4f("projection", m_Camera.GetProjectionMatrix());
			m_lightShader->SetUniform3v("lightColor", m_PointLight.color);

			if (!m_PointLight.HidePointLight)
			{
				Renderer::Get().DrawCube(*m_DefaultModels.Cube.m_VAO, *m_lightShader);
			}
		}

		m_SkyBox.Draw(m_Camera);
		m_HDR.PostProcess();
		m_HDR.DrawHDRresult();
		
		
	}


		



	void TestModelImport::OnImGuiRender()
	{
		ImGui::SameLine();
		ImGui::Text("Average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::Text("Press ESC  to Escape the Mouse");
		ImGui::Text("Press HOME to Recapture the Mouse");

		ImGui::Text("CameraPos:    X= %f Y= %f Z= %f", m_Camera.GetCameraPos().x, m_Camera.GetCameraPos().y, m_Camera.GetCameraPos().z);
		ImGui::Text("CameraTarget: X=  %f Y=  %f Z= %f", m_Camera.GetCameraTarget().x, m_Camera.GetCameraTarget().y, m_Camera.GetCameraTarget().z);

		m_SkyBox.ImGuiRender();
		m_HDR.ImGuiRender();
		
		if (ImGui::CollapsingHeader("Light Setting"))
		{
			m_DistantLight.ImGuiRender();
			m_PointLight.ImGuiRender();
			m_SpotLight.ImGuiRender();
		}

		if (ImGui::CollapsingHeader("Material Setting"))
		{
			ImGui::Combo("Material Type", &m_MaterialTypeIndex, TestModelImport::MaterialType, 4);
			
			if (m_MaterialTypeIndex != m_MaterialTypeIndex_LastTime)
			{
				m_MaterialTypeIndex_LastTime = m_MaterialTypeIndex;
				SelectMaterial(m_MaterialTypeIndex);
			}
			if (m_selectMaterial)
			{
				m_selectMaterial->ImGuiRender();
			}
			
		}
		

		if (ImGui::CollapsingHeader("Model Setting"))
		{
			
			

			ImGui::Text("Import Model Path:");
			ImGui::SameLine();
			ImGui::InputText("###Model_Path", this->Model_Path_TextBox_Buffer, IM_ARRAYSIZE(Model_Path_TextBox_Buffer));
			ImGui::SameLine();
			if (ImGui::Button("Import"))
			{
				m_model = Model(Model_Path_TextBox_Buffer);
			}
			else
			{
				
				m_model.ImGuiRender();
			}
		}
		
		for (int i = 0; i < ModelMatrixFactor.size(); i++)
		{
			auto& [Name, ModelScale, UniformModelScale] = ModelMatrixFactor[i];
			ImGui::Begin((Name+" Transform").c_str());
			ImGui::SliderFloat3(("Model Scale###" + std::to_string(i)).c_str(), (float*)&ModelScale, 0.01f, 10.0f);
			ImGui::SliderFloat(("Uniform Model Scale###" + std::to_string(i)).c_str(), &UniformModelScale, 0.01f, 10.0f);
			ImGui::End();
		}
		
		
	}

	void TestModelImport::SelectMaterial(int Index)
	{
		delete m_selectMaterial;
		switch (Index)
		{
		case 0:
			std::cout << "Change Material to 'Phong'" << std::endl;
			m_selectMaterial = new Phong();
			break;
		case 1:
			std::cout << "Change Material to 'Reflection'" << std::endl;
			m_selectMaterial = new Reflection();
			break;
		case 2:
			std::cout << "Change Material to 'Refraction'" << std::endl;
			m_selectMaterial = new Refraction();
			break;
		case 3:
			std::cout << "Change Material to 'Z-Depth'" << std::endl;
			m_selectMaterial = new ZDepth();
			break;
		default:
			std::cout << "Selected Material Error" << std::endl;
			return;
			break;
		}
	}

	void TestModelImport::MaterialToDraw(int& Index)
	{
		if (!m_selectMaterial)
		{
			Index = -1;
		}
		Phong* phong = dynamic_cast<Phong*>(m_selectMaterial);
		Reflection* reflection = dynamic_cast<Reflection*>(m_selectMaterial);
		Refraction* refraction = dynamic_cast<Refraction*>(m_selectMaterial);
		ZDepth* zDepth = dynamic_cast<ZDepth*>(m_selectMaterial);
		switch (Index)
		{
		case 0:
			/*Phong * phong = dynamic_cast<Phong*>(m_selectMaterial);*/

			//Pass 0: Generate Shadow Map 
			//m_Shadow.GenerateShadowMap(m_Camera, m_DistantLight, ModelMatrixFactor, DrawCalls);

			//Pass 1: Render Scenes
			
			for (int i = 0; i < DrawCalls.size(); i++)
			{
				auto& [Name ,ModelScale, UniformModelScale] = ModelMatrixFactor[i];
				phong->SetUniform(glm::scale(m_DefaultModelMatrix, ModelScale * UniformModelScale), m_Camera, m_DistantLight, m_PointLight, m_SpotLight);
				m_Shadow.BindShadowMap(5);
				DrawCalls[i](phong->GetShader());
			}
			
			break;
		case 1:
			//Reflection * reflection = dynamic_cast<Reflection*>(m_selectMaterial);
			reflection->SetUniform(glm::scale(m_DefaultModelMatrix, std::get<1>(ModelMatrixFactor[0]) * std::get<2>(ModelMatrixFactor[0])),m_Camera,m_SkyBox.GetEnvironmentMap());
			m_model.Draw(reflection->GetShader());
			break;
		case 2:
			//Refraction * refraction = dynamic_cast<Refraction*>(m_selectMaterial);
			refraction->SetUniform(glm::scale(m_DefaultModelMatrix, std::get<1>(ModelMatrixFactor[0]) * std::get<2>(ModelMatrixFactor[0])),m_Camera, m_SkyBox.GetEnvironmentMap());
			m_model.Draw(refraction->GetShader());
			break;
		case 3:
			glm::mat4 SpaceMatrix = (m_Camera.GetProjectionMatrix()) * (m_Camera.GetViewMatrix());
			zDepth->GenerateDepthMap(m_Camera, SpaceMatrix, ModelMatrixFactor, DrawCalls);
			zDepth->Draw();
			break;
		default:
			std::cout << "Draw Material Error" << std::endl;
			return;
			break;
		}
	}
}