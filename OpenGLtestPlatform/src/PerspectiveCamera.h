#pragma once
#include "Camera.h"
class PerspectiveCamera :
	public Camera
{
public:
	using Camera::Camera;
	glm::mat4 GetProjectionMatrix() override;
	
private:
	
};

