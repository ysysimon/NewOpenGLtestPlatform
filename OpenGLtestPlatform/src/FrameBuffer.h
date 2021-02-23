#pragma once
#include "Renderer.h"
class FrameBuffer
{
private:
	unsigned int m_RendererID;
public:
	FrameBuffer();
	~FrameBuffer();

	void Bind()const;
	void Unbind()const;
	inline unsigned int GetRendererID() {
		return m_RendererID;
	}

	void AttachTexture(int Type, unsigned int texture)const;
	void AttachRenderbufferObj(int Type, unsigned int rbo)const;
	void CheckFramebufferStatus();
};