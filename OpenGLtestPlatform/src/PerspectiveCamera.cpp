#include "PerspectiveCamera.h"

glm::mat4 PerspectiveCamera::GetProjectionMatrix()
{
	return glm::perspective(glm::radians(this->GetZoom()), this->GetWindowResolutionX() / this->GetWindowResolutionY(), this->GetNearPlane(), this->GetFarPlane());
}
