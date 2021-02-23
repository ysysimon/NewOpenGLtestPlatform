#include "Camera.h"
#include <functional>


Camera::Camera(GLFWwindow*& window, float WindowResolutionX, float WindowResolutionY,glm::vec3 CameraPos, glm::vec3 Up, float Yaw, float Pitch)
	:m_CameraTarget(glm::vec3(0.0f,0.0f,-1.0f)),m_MovementSpeed(SPEED),m_MouseSensitivity(SENSITIVITY),m_Zoom(ZOOM),m_Window(window), m_FirstMouse(true),
	m_LastMouse_X(WindowResolutionX/2), m_LastMouse_Y(WindowResolutionY/2), m_WindowResolutionX(WindowResolutionX), m_WindowResolutionY(WindowResolutionY)
{
	m_CameraPos = CameraPos;
	m_WorldUp = Up;
	m_Yaw = Yaw;
	m_Pitch = Pitch;
	this->updateCameraVectors();

	// tell GLFW to capture our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	//Register Callback Functions
	glfwSetWindowUserPointer(m_Window, this);
	glfwSetCursorPosCallback(window, Mouse_Callback_Static);
	glfwSetScrollCallback(window, Scroll_Callback_Static);
}

Camera::~Camera()
{
	//Logout Callback Functions
	glfwSetCursorPosCallback(m_Window, NULL);
	glfwSetScrollCallback(m_Window, NULL);
}

glm::mat4 Camera::GetViewMatrix() const
{
	return glm::lookAt(m_CameraPos, m_CameraPos + m_CameraTarget, m_CameraUp);
}

void Camera::ProcessKeyboard(Camera_Movement direction, float deltaTime)
{
	float velocity = m_MovementSpeed * deltaTime;
	if (direction == FORWARD)
		m_CameraPos += m_CameraTarget * velocity;
	if (direction == BACKWARD)
		m_CameraPos -= m_CameraTarget * velocity;
	if (direction == LEFT)
		m_CameraPos -= m_CameraRight * velocity;
	if (direction == RIGHT)
		m_CameraPos += m_CameraRight * velocity;
}

void Camera::ProcessMouseMovement(float Xoffset, float Yoffset, GLboolean constrainPitch = true)
{
	Xoffset *= m_MouseSensitivity;
	Yoffset *= m_MouseSensitivity;

	m_Yaw += Xoffset;
	m_Pitch += Yoffset;

	// make sure that when pitch is out of bounds, screen doesn't get flipped
	if (constrainPitch)
	{
		if (m_Pitch > 89.0f)
			m_Pitch = 89.0f;
		if (m_Pitch < -89.0f)
			m_Pitch = -89.0f;
	}

	// update Front, Right and Up Vectors using the updated Euler angles
	this->updateCameraVectors();
}

void Camera::ProcessMouseScroll(float Yoffset)
{
	m_Zoom -= (float)Yoffset;
	if (m_Zoom < 1.0f)
		m_Zoom = 1.0f;
	if (m_Zoom > 45.0f)
		m_Zoom = 45.0f;
}

void Camera::ProcessKeybordInput(float deltaTime)
{
	if (glfwGetKey(m_Window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		m_CaptureFlag = false;
		glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		glfwSetCursorPosCallback(m_Window, NULL);
		//glfwSetWindowShouldClose(m_Window, true);
	}
		

	if (m_CaptureFlag && glfwGetKey(m_Window, GLFW_KEY_W) == GLFW_PRESS)
		this->ProcessKeyboard(FORWARD, deltaTime);
	if (m_CaptureFlag && glfwGetKey(m_Window, GLFW_KEY_S) == GLFW_PRESS)
		this->ProcessKeyboard(BACKWARD, deltaTime);
	if (m_CaptureFlag && glfwGetKey(m_Window, GLFW_KEY_A) == GLFW_PRESS)
		this->ProcessKeyboard(LEFT, deltaTime);
	if (m_CaptureFlag && glfwGetKey(m_Window, GLFW_KEY_D) == GLFW_PRESS)
		this->ProcessKeyboard(RIGHT, deltaTime);
	if (glfwGetKey(m_Window, GLFW_KEY_HOME) == GLFW_PRESS)
	{
		//Recapture the Mouse
		m_CaptureFlag = true;
		glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		glfwSetCursorPosCallback(m_Window, Mouse_Callback_Static);
		m_FirstMouse = true;
	}
		
}

void Camera::Mouse_Callback(GLFWwindow* window, double xpos, double ypos)
{
	if (m_FirstMouse)
	{
		m_LastMouse_X = xpos;
		m_LastMouse_Y = ypos;
		m_FirstMouse = false;
	}

	float xoffset = xpos - m_LastMouse_X;
	float yoffset = m_LastMouse_Y - ypos; // reversed since y-coordinates go from bottom to top

	m_LastMouse_X = xpos;
	m_LastMouse_Y = ypos;

	this->ProcessMouseMovement(xoffset, yoffset);
}

void Camera::Scroll_Callback(GLFWwindow* window, double xoffset, double yoffset)
{
	this->ProcessMouseScroll(yoffset);
}

void Camera::Mouse_Callback_Static(GLFWwindow* window, double xpos, double ypos)
{
	Camera* that = static_cast<Camera*>(glfwGetWindowUserPointer(window));
	that->Mouse_Callback(window, xpos, ypos);
}

void Camera::Scroll_Callback_Static(GLFWwindow* window, double xoffset, double yoffset)
{
	Camera* that = static_cast<Camera*>(glfwGetWindowUserPointer(window));
	that->Scroll_Callback(window, xoffset, yoffset);
}

void Camera::updateCameraVectors()
{
	// calculate the new Front vector
	glm::vec3 Target;
	Target.x = cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
	Target.y = sin(glm::radians(m_Pitch));
	Target.z = sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
	m_CameraTarget = glm::normalize(Target);

	// also re-calculate the Right and Up vector
	m_CameraRight = glm::normalize(glm::cross(m_CameraTarget, m_WorldUp));  
	m_CameraUp = glm::normalize(glm::cross(m_CameraRight, m_CameraTarget));
}
