#include "FrameBuffer.h"
#include <iostream>


FrameBuffer::FrameBuffer()
{
	GLCall(glGenFramebuffers(1, &m_RendererID));
}

FrameBuffer::~FrameBuffer()
{
	GLCall(glDeleteFramebuffers(1, &m_RendererID));
}

void FrameBuffer::Bind() const
{
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID));
}

void FrameBuffer::Unbind() const
{
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}

void FrameBuffer::AttachTexture(int Type,unsigned int texture) const
{
	GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, Type, GL_TEXTURE_2D, texture, 0));
}

void FrameBuffer::AttachRenderbufferObj(int Type, unsigned int rbo)const
{
	GLCall(glFramebufferRenderbuffer(GL_FRAMEBUFFER, Type, GL_RENDERBUFFER, rbo));
}

void FrameBuffer::CheckFramebufferStatus()
{
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "Framebuffer not complete!" << std::endl;
}