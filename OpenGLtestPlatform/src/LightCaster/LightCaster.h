#pragma once
#include "tests/Test.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"


	enum LightType {
		Base,
		Distant,
		Point,
		Spot
	};

	struct BaseLight
	{
		BaseLight(glm::vec3 color, float ambientCoefficient, float diffuseCoefficient, float specularCoefficient);
		BaseLight();

		//static enum LightType lightType;
		glm::vec3 color;
		float ambientCoefficient;
		float diffuseCoefficient;
		float specularCoefficient;

		virtual inline int GetType() { return LightType::Base; }
		virtual void ImGuiRender() = 0;
	};


	struct DistantLight :
		public BaseLight
	{
		DistantLight(glm::vec3 direction, glm::vec3 color, float ambientCoefficient, float diffuseCoefficient, float specularCoefficient);

		//static const enum LightType lightType;
		//static enum LightType lightType;
		glm::vec3 direction;

		bool EnableDistantLight = true;

		inline int GetType() override { return LightType::Distant; }
		void ImGuiRender() override;
	};

	
	struct PointLight :
		public BaseLight
	{
		PointLight(glm::vec3 position, glm::vec3 color, float ambientCoefficient, float diffuseCoefficient, float specularCoefficient, float linear, float quadratic);

		//static const enum LightType lightType;
		//static enum LightType lightType;
		static const char* attenuationDistance[];
		glm::vec3 position;
		static const float K;
		float linear;
		float quadratic;
		float intensity = 1.0f;

		bool EnablePointLight = true;
		bool HidePointLight = false;
		int PointLightAttenuationNum = 6;

		void SetAttenuationDistance(int caseNum);

		inline int GetType() override { return LightType::Point; }
		void ImGuiRender() override;
	};

	struct SpotLight:
		public BaseLight
	{
		SpotLight(glm::vec3 position, glm::vec3 color, float ambientCoefficient, float diffuseCoefficient, float specularCoefficient, float cutoffAngle, float outerCutOffAngle,glm::vec3 spotDirection);

		//static const enum LightType lightType;
		//static enum LightType lightType;
		glm::vec3 position;
		float cutoffAngle;
		float outerCutOffAngle;
		glm::vec3 spotDirection;

		bool EnableSpotLight = true;
		bool SpotLightCutOffAngleSafeLock = true;

		inline int GetType() override { return LightType::Spot; }
		void ImGuiRender() override;
	};

