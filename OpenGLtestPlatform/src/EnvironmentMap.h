#pragma once
#include <string>
#include <vector>
#include <iostream>

#include "Renderer.h"

class EnvironmentMap
{
public:
	EnvironmentMap(const std::string& path,const std::vector<std::string>& faces);
	~EnvironmentMap();

	void Bind() const;
	void UnBind() const;
private:
	unsigned int m_RendererID;

	std::string m_path;
	std::vector<std::string> m_faces;
};

