#pragma once
#include <memory>
#include <functional>

#include"Model/DefaultModels.h"
#include "Shader.h"
#include "FrameBuffer.h"

class Bloom
{
public:
	Bloom(unsigned int BloomBufferRendererID, float SCR_WIDTH, float SCR_HEIGHT);
	void BloomGenerate(const VertexArray& vao,const int size);

	inline unsigned int GetBlurResultRenderID() {
		return SwitchColorBuffers[!horizontal];
	}

	inline int* GetAmountAddress() {
		return &amount;
	}

	void MakeAmountSafe();
private:
	std::unique_ptr<Shader> m_shaderBlur;
	std::function<void(void)> DrawCall;
	bool first_iteration;
	bool horizontal = true;
	int amount = 10;
	unsigned int m_BloomBufferRendererID; //bloom Buffer ID for first Blur iteration
	unsigned int SwitchFrameBuffers[2];
	unsigned int SwitchColorBuffers[2];

	float SCR_WIDTH, SCR_HEIGHT;
};
