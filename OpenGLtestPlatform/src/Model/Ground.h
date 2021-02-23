#pragma once
#include <memory>
#include"DefaultModels.h"

class Ground
{
public:
	Ground();
	~Ground();

	void Draw(Shader& shader);
private:
	DefaultModels m_DefaultModels;
	std::unique_ptr<Texture> m_Texture;

};

