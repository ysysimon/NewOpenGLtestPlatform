#pragma once
#include "imgui/imgui.h"
#include <memory>
#include <vector>

#include "Texture.h"
#include "Shader.h"
#include "Camera.h"
#include "LightCaster/LightCaster.h"

struct Material
{
	virtual void ImGuiRender() {};
	virtual void SetUniform() {};

	virtual Shader& GetShader() const = 0;
};
