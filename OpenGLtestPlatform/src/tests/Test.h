#pragma once

#include <vector>
#include <functional>
#include <string>
#include <iostream>
#include "Renderer.h"

namespace test {

	struct testMaterial
	{
	};
	
	struct testLight
	{
	};

	class Test
	{
	public:
		Test() {}
		virtual	~Test() {}

		virtual void OnUpdate(float deltaTime) {}
		virtual void OnRender(){}
		virtual void OnImGuiRender(){}
	};

	class TestMenu : public Test
	{
	public:
		TestMenu(Test*& currentTestPointer);
		

	 
		void OnImGuiRender() override;


		template<typename T>
		void RegisterTest(const std::string& name)
		{
			std::cout << "Register test " << name << std::endl;

			m_Tests.push_back(std::make_pair(name, []() {return new T(); }));
		}

		template<typename T>
		void RegisterTest(const std::string& name,GLFWwindow*& window,float WindowResolutionX, float WindowResolutionY)
		{
			std::cout << "Register test " << name << std::endl;

			m_Tests.push_back(std::make_pair(name, [&]() {return new T(window,WindowResolutionX,WindowResolutionY); }));
		}

	private:
		Test*& m_CurrentTest;
		std::vector<std::pair<std::string, std::function<Test*()>>> m_Tests;

	};

}