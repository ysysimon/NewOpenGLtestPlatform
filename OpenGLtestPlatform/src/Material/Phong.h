#pragma once
#include "Material.h"

struct Phong : Material {
	Phong();

	void ImGuiRender()override;
	void SetUniform(const glm::mat4& model, Camera& camera,DistantLight& DistantLight,PointLight& PointLight,SpotLight& SpotLight);
	void SetLightUniform(BaseLight& Light, Camera& camera);
	//void MakeSureBufferSize();

	//static Shader PhongPrototype;

	/*std::string m_Name = "";*/

	/*std::unique_ptr<Texture> diffuseColorMap;
	std::unique_ptr<Texture> specularColorMap;
	std::string diffusePath = "";
	std::string specularPath = "";*/
	std::unique_ptr<Shader> PhongShader;
	float shininess = 32;

	bool transformColorSpace = true;

	Shader& GetShader() const;
	/*std::vector<char> buf1;
	std::vector<char> buf2;*/


};