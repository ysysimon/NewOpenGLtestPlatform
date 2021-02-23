#pragma once
#include "Mesh.h"
#include "VertexArray.h"

class DefaultModels
{
public:
	DefaultModels(const DefaultModels & defaultModels) = delete;
	DefaultModels();
	Mesh Cube;
	Mesh DeBugViewGrid;
	Mesh Grid;
private:
	
};