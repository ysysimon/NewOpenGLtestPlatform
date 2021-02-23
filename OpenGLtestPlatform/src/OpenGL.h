#pragma once
#include "Renderer.h"
#include <string>
#include <iostream>
class OpenGL
{
public:
	OpenGL(const float& WindowResolutionX,const float& WindowResolutionY,const std::string& WindowName);
	~OpenGL();

	void OpenGLInit() const;
	GLFWwindow* OpenGLCreateWindow() const;
	void OpenGLEnableFunctions() const;
	static inline void OpenGLAfterProcess(GLFWwindow*& window)
	{
		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}
	static inline void OpenGLmakeContextCurrent(GLFWwindow*& window) 
	{
		glfwMakeContextCurrent(window);
		// glad: load all OpenGL function pointers
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			std::cout << "Failed to initialize GLAD" << std::endl;
		}
	}

	inline float GetWindowResolutionX() const { return m_WindowResolutionX; }
	inline float GetWindowResolutionY() const { return m_WindowResolutionY; }
private:
	float m_WindowResolutionX, m_WindowResolutionY;
	std::string m_WindowName;
};

