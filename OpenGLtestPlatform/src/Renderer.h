#pragma once
#include <glad\glad.h>
#include <GLFW/glfw3.h>

#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "Texture.h"

#define ASSERT(x) if (!(x)) __debugbreak();
#define GLCall(x) GLClearError();\
    x;\
    ASSERT( GLLogCall(#x,__FILE__,__LINE__))

void GLClearError();
bool GLLogCall(const char* function, const char* file, int line);

class Renderer
{
public:
    void Clear()
    {
        GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

    }

    static Renderer& Get()
    {
        static Renderer renderer;
        return renderer;
    }

    Renderer(const Renderer&) = delete;

    void Draw(const VertexArray& va,const IndexBuffer& ib,const Shader& shader) const;
    void DrawCube(const VertexArray& va, const Shader& shader) const;
    void DrawGrid(const VertexArray& va, const Shader& shader,const int& VertexNum)const;
    void DrawTRIANGLE(const VertexArray& va, const Shader& shader, const int& VertexNum)const;
private:
    Renderer() {};
};