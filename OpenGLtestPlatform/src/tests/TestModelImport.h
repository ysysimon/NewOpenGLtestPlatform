#pragma once

#include "Test.h"
#include <memory>
#include <functional>
#include <tuple>
#include <string>

#include "Renderer.h"
#include "imgui/imgui.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "PerspectiveCamera.h"
#include "LightCaster/LightCaster.h"
#include "Model/DefaultModels.h"
#include "Model/Model.h"
#include "Model/Ground.h"
#include "Model/NormalTestGrid.h"
#include "Material/Phong.h"
#include "Material/Reflection.h"
#include "Material/Refraction.h"
#include "Material/ZDepth.h"
#include "Shadow.h"
#include "SkyBox.h"
#include "HDR.h"

namespace test {
		
	class TestModelImport : public Test
	{
	public:
		TestModelImport(GLFWwindow*& window, float WindowResolutionX, float WindowResolutionY);
		~TestModelImport();
		
		void OnUpdate(float deltaTime)  override;
		void OnRender() override;
		void OnImGuiRender() override;

	private:

		std::vector<std::string> faces = {
			"right.jpg",
			"left.jpg",
			"top.jpg",
			"bottom.jpg",
			"front.jpg",
			"back.jpg"
		};

		DefaultModels m_DefaultModels;
		Ground m_Ground;
		NormalTestGrid m_NormalTestGrid;

		Phong m_GroundMaterial;

		Model m_model;
		/*glm::vec3 m_modelScale;
		float m_UniformModelScale = 1.0f;*/
		glm::mat4 m_DefaultModelMatrix = glm::mat4(1.0f);

		std::unique_ptr<Shader> m_lightShader;
		
		Shadow m_Shadow;

		SkyBox m_SkyBox;

		PerspectiveCamera m_Camera;

		PointLight m_PointLight;
		DistantLight m_DistantLight;
		SpotLight m_SpotLight;

		HDR m_HDR;

		char Model_Path_TextBox_Buffer[60];

		static const char* MaterialType[];
		int m_MaterialTypeIndex = 0;
		int m_MaterialTypeIndex_LastTime = 0;
		Material* m_selectMaterial;

		//std::function<void(Shader&)> DrawCall;
		std::vector<std::function<void(Shader&)>> DrawCalls;
		std::vector<std::tuple<std::string,glm::vec3, float>> ModelMatrixFactor;

		template<typename T>
		void PushDrawCall(T& obj,std::string name)
		{
			DrawCalls.push_back([&](Shader& shader) {obj.Draw(shader); });
			ModelMatrixFactor.push_back(std::make_tuple(name ,glm::vec3(1.0f, 1.0f, 1.0f), 1.0f));
		}
		
		void MaterialToDraw(int& Index);
		void SelectMaterial(int Index);

	};

	
	
}