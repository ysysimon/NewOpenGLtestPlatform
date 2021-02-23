#pragma once
#include "Renderer.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
enum Camera_Movement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};

// Default camera values
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;


class Camera
{
public:
	Camera(GLFWwindow*& window, float WindowResolutionX, float WindowResolutionY,glm::vec3 CameraPos = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 CameraUp = glm::vec3(0.0f, 1.0f, 0.0f),float Yaw = YAW,float Pitch = PITCH);
	~Camera();

	virtual glm::mat4 GetViewMatrix()const;
	virtual glm::mat4 GetProjectionMatrix() =0;
	virtual void ProcessKeyboard(Camera_Movement direction, float deltaTime);
	virtual void ProcessMouseMovement(float Xoffset, float Yoffset, GLboolean constrainPitch);
	virtual void ProcessMouseScroll(float Yoffset);
	virtual void ProcessKeybordInput(float deltaTime);
	virtual void Mouse_Callback(GLFWwindow* window, double xpos, double ypos);
	virtual void Scroll_Callback(GLFWwindow* window, double xoffset, double yoffset);
	static  void Mouse_Callback_Static(GLFWwindow* window, double xpos, double ypos);
	static  void Scroll_Callback_Static(GLFWwindow* window, double xoffset, double yoffset);

	inline float GetZoom() const { return m_Zoom; }
	inline float GetWindowResolutionX() const { return m_WindowResolutionX; }
	inline float GetWindowResolutionY() const { return m_WindowResolutionY; }
	inline glm::vec3 GetCameraPos() const { return m_CameraPos; }
	inline void SetCameraPos(const glm::vec3& CameraPos)  
	{ 
		this->m_CameraPos.x = CameraPos.x;
		this->m_CameraPos.y = CameraPos.y;
		this->m_CameraPos.z = CameraPos.z;
		
	}
	inline glm::vec3 GetCameraTarget() const { return m_CameraTarget; }
	inline void SetCameraTarget(const glm::vec3& CameraTarget)
	{
		this->m_CameraTarget.x = CameraTarget.x;
		this->m_CameraTarget.y = CameraTarget.y;
		this->m_CameraTarget.z = CameraTarget.z;
		
	}
	inline float GetNearPlane()
	{
		return m_NearPlane;
	}
	inline float GetFarPlane()
	{
		return m_FarPlane;
	}
private:
	GLFWwindow* m_Window;

	glm::vec3 m_CameraPos;
	glm::vec3 m_CameraUp;
	glm::vec3 m_CameraTarget;
	glm::vec3 m_CameraRight;
	glm::vec3 m_WorldUp;

	float m_Yaw;
	float m_Pitch;

	float m_MovementSpeed;
	float m_MouseSensitivity;
	float m_Zoom;

	float m_NearPlane = 0.1f, m_FarPlane = 100.0f;

	float m_WindowResolutionX, m_WindowResolutionY;
	float m_LastMouse_X, m_LastMouse_Y;
	bool m_FirstMouse;
	bool m_CaptureFlag = true;

	virtual void updateCameraVectors();
};

