#include "Bloom.h"

Bloom::Bloom(unsigned int BloomBufferRendererID, float SCR_WIDTH, float SCR_HEIGHT)
	:m_BloomBufferRendererID(BloomBufferRendererID),first_iteration(true),SCR_WIDTH(SCR_WIDTH), SCR_HEIGHT(SCR_HEIGHT)
{
	m_shaderBlur = std::make_unique<Shader>("res/shaders/Bloom.shader");
    m_shaderBlur->Bind();
    m_shaderBlur->SetUniform1i("image",0);

    glGenFramebuffers(2, SwitchFrameBuffers);
    glGenTextures(2, SwitchColorBuffers);
    for (GLuint i = 0; i < 2; i++)
    {
        glBindFramebuffer(GL_FRAMEBUFFER, SwitchFrameBuffers[i]);
        glBindTexture(GL_TEXTURE_2D, SwitchColorBuffers[i]);
        glTexImage2D(
            GL_TEXTURE_2D, 0, GL_RGB16F, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGB, GL_FLOAT, NULL
        );
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glFramebufferTexture2D(
            GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, SwitchColorBuffers[i], 0
        );
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            std::cout << "Framebuffer not complete!" << std::endl;
    }
}

void Bloom::BloomGenerate(const VertexArray& vao, const int size)
{
    m_shaderBlur->Bind();

    for (int i = 0; i < amount; i++)
    {
        GLCall(glBindFramebuffer(GL_FRAMEBUFFER, SwitchFrameBuffers[horizontal]));
        m_shaderBlur->SetUniform1i("horizontal", horizontal);
        GLCall(glActiveTexture(GL_TEXTURE0));
        GLCall(glBindTexture(
            GL_TEXTURE_2D, first_iteration ? m_BloomBufferRendererID : SwitchColorBuffers[!horizontal]
        ));
        Renderer::Get().DrawGrid(vao, *m_shaderBlur,size);
        horizontal = !horizontal;
        if (first_iteration)
            first_iteration = false;
    }
    first_iteration = true;
    GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
    GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
}

void Bloom::MakeAmountSafe()
{
    if (amount % 2 != 0)
    {
        amount++;
    }
}