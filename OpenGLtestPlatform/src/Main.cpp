#include <glad\glad.h>
#include <GLFW/glfw3.h>
#include "OpenGL.h"
#include <iostream>

#include "Renderer.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "tests/TestClearColor.h"
#include "tests/TestTexture2D.h"
#include "tests/TestCamera.h"
#include "tests/TestBasicLight.h"
#include "tests/TestBasicTexture.h"
#include "tests/TestLightCaster.h"
#include "tests/TestModelImport.h";


static inline void myImGuiInit(GLFWwindow*& const window);
static inline void myImGuiDestroy();
static inline void myImGuiNewFrame();

int main(void)
{
    OpenGL opengl(1600, 900, "HelloWorld");

    opengl.OpenGLInit();

    /* Create a windowed mode window and its OpenGL context */
    GLFWwindow* window;
    window = opengl.OpenGLCreateWindow();
    if (!window)
    {
        std::cout << "Failed to Create Window" << std::endl;
        glfwTerminate();
        return -1;
    }
    
    /* Make the window's context current */
    opengl.OpenGLmakeContextCurrent(window);

    

    
    //Enable function of OpenGL
    
    opengl.OpenGLEnableFunctions();

    //Init ImGUI
    myImGuiInit(window);
    

    //Create Tests
    test::Test* currentTest = nullptr;
    test::TestMenu* testMenu = new test::TestMenu(currentTest);
    currentTest = testMenu;

    testMenu->RegisterTest<test::TestClearColor>("Clear Color");
    testMenu->RegisterTest<test::TestTexture2D>("Test 2D Texture");
    testMenu->RegisterTest<test::TestCamera>("Test Camera",window,opengl.GetWindowResolutionX(),opengl.GetWindowResolutionY());
    testMenu->RegisterTest<test::TestBasicLight>("Test Basic Light", window, opengl.GetWindowResolutionX(), opengl.GetWindowResolutionY());
    testMenu->RegisterTest<test::TestBasicTexture>("Test Basic Texture", window, opengl.GetWindowResolutionX(), opengl.GetWindowResolutionY());
    testMenu->RegisterTest<test::TestLightCaster>("Test Light Caster", window, opengl.GetWindowResolutionX(), opengl.GetWindowResolutionY());
    testMenu->RegisterTest<test::TestModelImport>("Test Model Import", window, opengl.GetWindowResolutionX(), opengl.GetWindowResolutionY());

    // timing
    float deltaTime = 0.0f;	// time between current frame and last frame
    float lastFrame = 0.0f;

    /* Loop until the user closes the window */

    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        /* Render here */
        GLCall(glClearColor(0.0f,0.0f,0.0f,1.0f));
        Renderer::Get().Clear();

        myImGuiNewFrame();

        if (currentTest)
        {
            currentTest->OnUpdate(deltaTime);
            currentTest->OnRender();
            ImGui::Begin("Test Menu");
            if (currentTest != testMenu && ImGui::Button("<-"))
            {
                delete currentTest;
                currentTest = testMenu;
            }
            currentTest->OnImGuiRender();
            ImGui::End();
        }

        


        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        opengl.OpenGLAfterProcess(window);
        
    }

    delete currentTest;
    if (currentTest != testMenu)
    {
        delete testMenu;
    }


    myImGuiDestroy();

    
    return 0;
}

static inline void myImGuiInit(GLFWwindow*& const window)
{
    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 150");
    ImGui::StyleColorsDark();
}

static inline void myImGuiDestroy()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

static inline void myImGuiNewFrame()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}